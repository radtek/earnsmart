/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

var bTrueValidity = 0;
var txtfirstname = $("<input class=\"reg_inputcontrols\" title=\"Enter first name (required)\" name=\"ppa_txtfirstname\" type=\"text\" />");
var txtlastname = $("<input class=\"reg_inputcontrols\" title=\"Enter last name (required)\" name=\"ppa_txtlastname\" type=\"text\" />");
var txtstreet1 = $("<input id=\"ppa_txtstreet1\" name=\"ppa_txtstreet1\" class=\"reg_inputcontrols\" title=\"Enter street1 (required)\" type=\"text\" />");
var txtstreet2 = $("<input id=\"ppa_txtstreet2\" name=\"ppa_txtstreet2\" class=\"reg_inputcontrols\" title=\"Enter street2\" type=\"text\" />");
var txtcity = $("<input id=\"ppa_txtcity\" class=\"reg_inputcontrols\" name=\"ppa_txtcity\" type=\"text\" title=\"Enter city (required)\" />");
var txtpcode = $("<input id=\"ppa_txtpostalcode\" class=\"reg_inputcontrols\" name=\"ppa_txtpostalcode\" type=\"text\" title=\"Select postal code (required). Leave a zero if no postal code available.\" style=\"width:50pt\" />");
var cmbCountries = $("<select id=\"ppa_cmbcountries\" name=\"ppa_cmbcountries\" class=\"reg_inputcontrols\" title=\"Select country (required)\" name=\"reg_cmbCountries\" ></select>");
var txtstate = $("<input id=\"ppa_txtstate\" class=\"reg_inputcontrols\" name=\"ppa_cmbstates\" type=\"text\" title=\"Enter state (required)\" />");
var cmbStates  = $("<select id=\"ppa_cmbstates\" name=\"ppa_cmbstates\" class=\"reg_inputcontrols\" title=\"Select state (required)\" name=\"reg_cmbStates\" ></select>");

var llfirstname = $("<label class=\"reg_inputcontrols\"/>");
var lllastname = $("<label class=\"reg_inputcontrols\"/>");
var llstreet1 = $("<label class=\"reg_inputcontrols\"/>");
var llstreet2 = $("<label class=\"reg_inputcontrols\"/>");
var llcity = $("<label class=\"reg_inputcontrols\"/>");
var llstate = $("<label class=\"reg_inputcontrols\"/>");
var llcountry = $("<label class=\"reg_inputcontrols\"/>");
var llpcode = $("<label class=\"reg_inputcontrols\"/>");

function setupvalidation()
{
    $(".reg_inputcontrols").each(function()
    {
        $(this).blur(function()
        {
            if ($(this) == txtfirstname)
            {
                bTrueValidity |= ($(this).val()!="") ? 1 : 0;
            }
            if ($(this) == txtlastname)
            {
                bTrueValidity |= ($(this).val()!="") ? 1<<1 : 0<<1;
            }
            if ($(this) == txtstreet1)
            {
                bTrueValidity |= ($(this).val()!="")? 1<<2 : 0<<2;
            }
            if ($(this) == txtcity)
            {
                bTrueValidity |= ($(this).val()!="")? 1<<3 : 0<<3;
            }
            if ($(this) == txtpcode)
            {
                bTrueValidity |= ($(this).val()!="")? 1<<4 : 0<<4;
            }
            var p = $(this).parent();
            if ($(this).val()=="")
                p.attr("class","cls_reg_dvInputRed");
            else
                p.attr("class","cls_reg_dvInput");
            ppa_btnNext.disabled = (bTrueValidity != 0x3F);
        });
    });
}

function popcountries()
{
    //var option = $("<option value=\"0\"></option>");
    //cmbCountries.append(option);
    $.getJSON(baseurl.concat("1"), function(data)
    {
        if (data["Status"] == 1)
        {
            if ($("#reg_hdedit").val()=="1")
                cmbCountries.remove();
            cmbCountries = $("<select id=\"ppa_cmbcountries\" name=\"ppa_cmbcountries\" class=\"reg_inputcontrols\" title=\"Select country (required)\" name=\"reg_cmbCountries\" ></select>");
            if ($("#reg_hdedit").val()=="1")
                $("#dv_reg_countries").append(cmbCountries);
            cmbCountries.change(function()
            {
                popstates();
            });
            var countries = data["Countries"];
            var len = countries["Count"];
            for (var i=0; i != len; ++i)
            {
                var country = countries[new String(i)];
                var str = "<option ";
                if (country.TwoCharCode == scountry)
                    str += "selected ";
                str += "value=\"" + country.TwoCharCode  + "\">" + country.Name + "</option>";
                var option = $(str);
                cmbCountries.append(option);	
            }
            popstates();
        }
    }).fail(function()
    {

    });
}

function popstates()
{
    //var option = $("<option value=\"0\"></option>");
    //cmbStates.append(option);
    $.getJSON(
        baseurl.concat("2/").concat(cmbCountries.val()), 
        function(data)
        {
            if (data["Status"] == 1)
            {
                if ($("#reg_hdedit").val()=="1")
                    cmbStates.remove();
                cmbStates  = $("<select id=\"ppa_cmbstates\" name=\"ppa_cmbstates\" class=\"reg_inputcontrols\" title=\"Select state (required)\" name=\"reg_cmbStates\" ></select>")
                if ($("#reg_hdedit").val()=="1")
                {
                    txtstate.remove();
                    $("#dv_reg_states").append(cmbStates);
                }
                var states = data["States"];
                var len = states["Count"];
                if (len>0)
                {
                    for (var i=0; i != len; ++i)
                    {
                        var state = states[new String(i)];
                        var str = "<option ";
                        if (state.Code == sstate || 
                                state.Id == sstate)
                                str += "selected ";
                        str += "value=\"" + state.Id  + "\">" + state.Name + "</option>";
                        var option = $(str);
                        cmbStates.append(option);	
                    }
                    return;
                }
            }
            stateedit();
        })
    .fail(function()
        {
            stateedit();
        });
}

function stateedit()
{
    if ($("#reg_hdedit").val()=="1")    
    {
        cmbStates.remove();
        txtstate.val("");
        $("#dv_reg_states").append(txtstate);
        txtstate.blur(function()
        {
            bTrueValidity |= ($(this).val()!="")? 1<<5 : 0<<5;
            ppa_btnNext.disabled = (bTrueValidity != 0x3F);
        });
    }
}

function labelcontrols()
{
    if ($("#reg_hdedit").val()=="0") return false;

    llfirstname.text(sfirstname);
    $("#dv_reg_firstname").append(llfirstname);

    lllastname.text(slastname);
    $("#dv_reg_lastname").append(lllastname);

    llstreet1.text(sstreet1);
    $("#dv_reg_street1").append(llstreet1);

    llstreet2.text(sstreet2);
    $("#dv_reg_street2").append(llstreet2);

    llcity.text(scity);
    $("#dv_reg_city").append(llcity);

    llcountry.text(scountry);
    $("#dv_reg_countries").append(llcountry);

    llstate.text(sstate);
    $("#dv_reg_states").append(llstate);

    llpcode.text(spcode);
    $("#dv_reg_pcode").append(llpcode);

    $("#reg_hdedit").val("0");
    
    ppa_btnNext.disabled = false;
    
}

function alloweditdetails()
{
    llfirstname.remove();
    lllastname.remove();
    llstreet1.remove();
    llstreet2.remove();
    llpcode.remove();
    llcity.remove();
    llstate.remove();
    llcountry.remove();            	

    txtfirstname.val(sfirstname);
    $("#dv_reg_firstname").append(txtfirstname);

    txtlastname.val(slastname);
    $("#dv_reg_lastname").append(txtlastname);

                    txtstreet1.val(sstreet1);                
    $("#dv_reg_street1").append(txtstreet1);

                    txtstreet2.val(sstreet2);                
    $("#dv_reg_street2").append(txtstreet2);

                    txtcity.val(scity);                
    $("#dv_reg_city").append(txtcity);

                    txtpcode.val(spcode);                
    $("#dv_reg_pcode").append(txtpcode);

    //$("#dv_reg_states").append(cmbStates);

    //$("#dv_reg_countries").append(cmbCountries);
    
    popcountries();

    $("#reg_hdedit").val("1");
    
    setupvalidation();
    

    return false;
}

$(document).ready(function () 
{

    baseurl = location.protocol.concat("//").concat(window.location.hostname).concat(":").concat(location.port).concat("/gj/");
    popcountries();
    if ($("#reg_hdedit").val()=="1")
        alloweditdetails();
    else
        labelcontrols();


    $("#ppapprove_form").submit(function (e) 
    {
        if (e.target.id == "ppa_btnCancel")
        {
            if ((!bValidForm || bNotAgreed))
            {
                e.preventDefault();
                return false;
            }
            return true;
        }
        else if (e.target.id == "ppa_btnCancel")
        {
            return true;
        }
    });
    
    //alert(sstate);

});