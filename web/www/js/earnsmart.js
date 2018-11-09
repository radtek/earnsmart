/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

'use strict';

var urlpfx = location.protocol.concat("//").concat(window.location.hostname).concat(":").concat(location.port);


WebFontConfig = 
{
    google: 
    { 
        families: [ 'Playfair+Display::latin', 'Open+Sans:300italic,400italic,600italic,700italic,800italic,400,600,800,700,300:latin' ] 
    }
};



function gfontload() 
{
    var wf = document.createElement('script');
    wf.src = ('https:' == document.location.protocol ? 'https' : 'http') + '://ajax.googleapis.com/ajax/libs/webfont/1/webfont.js';
    wf.type = 'text/javascript';
    wf.async = 'true';
    var s = document.getElementsByTagName('script')[0];
    s.parentNode.insertBefore(wf, s);
};


$.widget( "custom.catcomplete", $.ui.autocomplete, 
{
    _create: function() 
    {

      this._super();

      this.widget().menu( "option", "items", "> :not(.ui-autocomplete-category)" );

    },

    _renderMenu: function( ul, items ) 
    {
        ul.empty();
        var that = this;
        var cat = "";
        $.each(items, function(index, item)
        {
           if (item.category !=cat)
           {
                ul.append( "<li class='ui-autocomplete-category'>" + item.category + "</li>" );
                cat = item.category;
           }
           li = that._renderItemData(ul, item);
           li.attr("aria-label", item.label);
        });
    },
});
  
$(function() 
{
    $( "#txtAppSearch" ).catcomplete(
    {
        minLength : 1,
        delay: 50,
        autoFocus : true,
        source: function(request, response)
        {
            /*
            oboe({ url : urlpfx.concat("/appsearch"), method: "POST", headers : request })
                .node("{label category value}", function(data)
                {
                    //console.log(data);
                })
                .done(function(data)
                {
                    console.log("completed");
                    response(data);
                });
            */
           ///*
            $.post(urlpfx.concat("/appsearch"), request, function(data, status, xhr)
            {
                if (status === "success")
                {
                    response(data);
                }
            });
            //*/
        },
        select : function(event, ui)
        {
            event.preventDefault();
            $("#txtAppSearch").val(ui.item.label.split("|")[0].trim());
            $("#txtAppSearch").select();
        },
        position : { my : "right top", at: "right bottom" }
    });
    
    $("#txtAppSearch").focus(function()
    {
        $("#txtAppSearch").catcomplete("search", $("#txtAppSearch").val()) ;
    });
    
    var activeMenu = null;
    
    for (var i=2; i <= 4; ++i)
    {
        $("#topsubmenu" + i).menu();
        $("#topsubmenu"+i).position({ my: "left top", at: "left bottom", of: "#topmenu" + i,collision: "fit"} );
        $("#topsubmenu"+i).on("menuselect", function(event, ui)
        {
           $(this).css("visibility", "hidden");
            activeMenu=null;
        });
        $("#topmenu" + i).click(function(e) {
            e.preventDefault();
            if (activeMenu)
               activeMenu.css("visibility", "hidden");
            var submenu = $(this).attr("submenu");
            activeMenu = $("#" + submenu);
            activeMenu.css("visibility", "visible");
            console.log(activeMenu);
        });
    }
  
    $(document).click(function(e)
    {
       if (e.button == 0)
       {
           if (activeMenu && e.target.id.indexOf("topmenu") == -1 && e.target.id.indexOf("topsubmenu") == -1)
           {
               activeMenu.css("visibility", "hidden");
               activeMenu=null;
           }
       }
    });
  
    $("input[name='regUsertype']").click(function()
    {
        $("#formMain").submit();
    });
    
    $("input[name='regEmail']").blur(
        function()
        {
            if ($(this).val() == "")
                return ;
            
            var req = { 
                method : "checkloginid",
                params : [ $(this).val() ],
                id : 3
            };
            console.info(req);
            $.post(urlpfx.concat("/rpc"), "cdata=" + JSON.stringify(req),
            function(data)
            {
                console.info(data.result);
                $("#btnRegSteps").prop("disabled", data.result);
                if (data.result)
                {
                    console.info("Writing to the label.");
                    $("#lblRegEmail").text("Email address already in use. Click Log on link above to sign in.");
                }
            });
        }
    );
 
    $("#btnRegSteps").click(
        function()
        {
            /*
            var stages = { 
                'UserTypeAndAddress': 0,
                'SendEmail'         : 1,
                'EmailConfirmation' : 2,
                'Payment'           : 3,
                'Completed'         : 4
            };
            */
            // check stages
            var hdStage = $("input[name='regStage'][type='hidden']");
            var regType = parseInt(hdStage.val());
            console.info("RegType : " + regType);
            switch(regType)
            {
                case 0:
                    // after page is successfully validated
                    hdStage.val("1");
                    break;
                case 1:
                    // after page is successfully validated
                    hdStage.val("2");
                    break;
                case 2:
                    // after page is successfully validated
                    hdStage.val("3");
                    break;
            }
            $("#formMain").submit();
        }
    );
  
});
