/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

var checkuser = "";
var bInvalidLogin = true;
var bWeakPwd = true;
var bConfirmFailed = true;
var bNotAgreed = true;
var bValidForm = false;
 function setupcontrols()
 {
    $(".reg_inputcontrols").each(function()
    {
        $(this).blur(function(e)
        {
                var p = $(this).parent();
                if ($(this).val()=="")
                        p.attr("class","cls_reg_dvInputRed");
                else
                        p.attr("class","cls_reg_dvInput");
                bValidForm = true;
                $(".reg_inputcontrols").each(function(index)
                {
                        if ($(this).val()=="")
                                bValidForm = false;
                });
                if (bValidForm)
                {
                        bValidForm = (bWeakPwd || bConfirmFailed) ? false : true;
                }
                if (bValidForm)
                {
                                        $("#reg_lblPwdConfirm").text("");	                			
                        $("#reg_lblPwdMeter").text("");
                        p.attr("class","cls_reg_dvInput");
                }
               $("#reg_btnNext").prop("disabled",!(bValidForm && !bNotAgreed && !bInvalidLogin));
        });
    });
   $(".reg_inputcontrols[name='reg_txtemail']").blur(function(e)
   {
       var p = $(this).parent();
       $.getJSON(checkuser + $(this).val(), function(data)
       {
           if (data["Status"]==1)
           {
               p.attr("class","cls_reg_dvInputRed");
               $("#reg_lblEmail").text("This login already exists.");
               bInvalidLogin = true;
           }
           else
           {
               p.attr("class","cls_reg_dvInput");
               $("#reg_lblEmail").text("");
               bInvalidLogin = false;
           }
           $("#reg_btnNext").prop("disabled",!(bValidForm && !bNotAgreed && !bInvalidLogin));
   }).fail(function()
       {
           p.attr("class","cls_reg_dvInputRed");
           $("#reg_lblEmail").text("Incorrect email address format.");
           bInvalidLogin = true;
           $("#reg_btnNext").prop("disabled",!(bValidForm && !bNotAgreed && !bInvalidLogin));
       });
   });

    $(".reg_inputcontrols[name='reg_txtpassword']").keyup(function(e)
    {
            var strength = pwdmeter($(this).val());
            var p = $(this).parent();
            bWeakPwd = false;
            if (strength > 25 && strength <= 50)
            {
                    p.attr("class","cls_reg_dvInputYellow");
                    $("#reg_lblPwdMeter").text("Good password");
            }
            else if (strength > 50 && strength <= 75)
            {
                    p.attr("class","cls_reg_dvInputOrange");
                    $("#reg_lblPwdMeter").text("Strong password");
            }
            else if (strength > 75 && strength <= 100)
            {
                    p.attr("class","cls_reg_dvInputGreen");
                    $("#reg_lblPwdMeter").text("Secure password");
            }
            else
            {
                    p.attr("class","cls_reg_dvInputRed");
                    $("#reg_lblPwdMeter").text("Weak password");
                    bWeakPwd = true;
            }
    });
    $(".reg_inputcontrols[name='reg_txtpasswordagain']").keyup(function(e)
    {
                            bConfirmFailed = false;
            var p = $(this).parent();
            if ($(this).val() != $(".reg_inputcontrols[name='reg_txtpassword']").val())
            {
                    p.attr("class","cls_reg_dvInputRed");
                                    $("#reg_lblPwdConfirm").text("Confirm password doesn't match.");	                			
                                    bConfirmFailed = true;
            }
            else
            {
                    p.attr("class","cls_reg_dvInputGreen");
                                    $("#reg_lblPwdConfirm").text("Confirm password matched.");	                			
            }
                    });
                    $("input:radio[name='reg_rdaggree']").change(function(e)
                    {
                            bNotAgreed = ($(this).val()=="No");
            $("#reg_btnNext").prop("disabled",!(bValidForm && !bNotAgreed && !bInvalidLogin));
                    });
 }

 $(document).ready(function () 
 {
     checkuser = location.protocol.concat("//").concat(window.location.hostname).concat(":").concat(location.port).concat("/checkuser/");
     $("input:password").each(function()
     {
        $(this).prop("maxLength", "20");
     });
     setupcontrols();
     $("#reg_form").submit(function (e) 
     {
         if (e.target.id == "reg_btnCancel")
         {
                 if ((!bValidForm || bNotAgreed))
                 {
                         e.preventDefault();
                         return false;
                 }
                 $("input:hidden[name='reg_hdsubmittype']").value("next");
                 return true;
         }
         else if (e.target.id == "reg_btnCancel")
         {
                 $("input:hidden[name='reg_hdsubmittype']").value("cancel");
                 return true;
         }
     });
 });