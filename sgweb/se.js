
////////////////////////// Window Management/////////////////////////////

/* m_header_section 
 * m_main_section 
 * m_main_content
 * m_main_sidesection
 * m_footer_section
 * global_cosearch
 * */

function resetLayout()
{
    //var s = $("#global_cosearch").width();
    $("#header_menu").width($(window).width() - $("#header_menu").offset().left - 10);
    $("#header_menu").offset({top: $("#header_logo").offset().top + $("#header_logo").height()});
    //var searchtop = 0;
    //var ls = $("#header_menu").offset().left + $("#header_menu").width() - s;
    //$("#global_cosearch").offset({left: ls, top: searchtop });
    $("#m_main_section").offset({top: $("#header_menu").offset().top + $("#header_menu").height() + 10 });
    $("#mtr_headerright").offset({left : $("#header_menu").offset().left + $("#header_menu").width() - $("#mtr_headerright").width()});
    //$("#m_main_content").offset({left: $("#header_menu").offset().left});
    //m_main_sidesection
    var sidesectionwidth = $("#m_main_sidesection").width();
    var mainsectionwidth = $("#m_main_section").width();
    var sidesectionleft = $("#m_main_section").offset().left + mainsectionwidth - sidesectionwidth;
    $("#m_main_sidesection").offset({left: sidesectionleft});
    $("#m_main_content").width(mainsectionwidth - sidesectionwidth - 30);   // 30 to adjust margin of maincontent.
}

$(window).resize(function()
{
    resetLayout();
});

$(window).load(function()
{
    resetLayout();
});

/////////////////////// misc funcs ///////////////////////////////////////
function pwdmeter(p) 
{
    var intScore = 0;
    intScore += p.length;
    if(p.length > 0 && p.length <= 4) {                    // length 4 or less
            intScore += p.length;
    }
    else if (p.length >= 5 && p.length <= 7) {	// length between 5 and 7
            intScore += 6;
    }
    else if (p.length >= 8 && p.length <= 15) {	// length between 8 and 15
            intScore += 12;
            //alert(intScore);
    }
    else if (p.length >= 16) {               // length 16 or more
            intScore += 18;
            //alert(intScore);
    }
    if (p.match(/[a-z]/)) {              // [verified] at least one lower case letter
            intScore += 1;
    }
    if (p.match(/[A-Z]/)) {              // [verified] at least one upper case letter
            intScore += 5;
    }
    // NUMBERS
    if (p.match(/\d/)) {             	// [verified] at least one number
            intScore += 5;
    }
    if (p.match(/.*\d.*\d.*\d/)) {            // [verified] at least three numbers
            intScore += 5;
    }

    // SPECIAL CHAR
    if (p.match(/[!,@,#,$,%,^,&,*,?,_,~]/)) {           // [verified] at least one special character
            intScore += 5;
    }
    // [verified] at least two special characters
    if (p.match(/.*[!,@,#,$,%,^,&,*,?,_,~].*[!,@,#,$,%,^,&,*,?,_,~]/)) {
            intScore += 5;
    }

    // COMBOS
    if (p.match(/(?=.*[a-z])(?=.*[A-Z])/)) {        // [verified] both upper and lower case
            intScore += 2;
    }
    if (p.match(/(?=.*\d)(?=.*[a-z])(?=.*[A-Z])/)) { // [verified] both letters and numbers
            intScore += 2;
    }
    // [verified] letters, numbers, and special characters
    if (p.match(/(?=.*\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[!,@,#,$,%,^,&,*,?,_,~])/)) {
            intScore += 2;
    }
    var nRound = Math.round(intScore * 2);
    if (nRound > 100) 
    {
        nRound = 100;
    }
    return nRound;    
}

//////////////////// JQUERIES ////////////////////////////////////

$(document).ready(function()
{
    $("#clist").hide();
    $("#cl2").click(function()
    {
        $("#clist").slideToggle("slow");
    });
  
    $("#basic").hide();
    $("#rbasic").click(function()
    {
        $("#basic").slideToggle("slow");
        $("#sub").hide();
    });

    $("#sub").hide();
    $("#rsub").click(function()
    {
        $("#sub").slideToggle("slow");
        $("#basic").hide();
    });
  

    $("#basic").hide();
    $("#rbasic").click(function()
    {
        $("#basic").slideToggle("slow");
        $("#sub").hide();
    });

  $("#stmtValues tr:even").css("background-color", "#d3d3d3");
  $("#stmtValues tr:odd").css("background-color", "#ffffff");
  
  $("#bt").click(function()
  {
        $('#ddStatements').empty().append('whatever');
        $('#ddStatements').hide();
  });  

    //$("#m_footer_section").offset({top : 10 + Math.max($(document).height(), $(window).height()) - $("#m_footer_section").height() });
    
});


////////////////////////////////// Visualization ///////////////////////////////////////
//////////////// Company Screener View /////////////////

var sgweb = {};

sgweb.DualSlider = function(container)
{
    this.data = null;
    this.container = container;
    this.chart = null;
    this.control = null;
    this.chartPlaceHolder = null;
    this.controlPlaceHolder = null;
    this.eventHandle = null;
    this.tag = null;
    this.state = null;
};

sgweb.DualSlider.prototype.getState = function()
{
    return this.state;
};

sgweb.DualSlider.prototype.setState = function(stateobj)
{
    this.state = stateobj;
};

sgweb.DualSlider.prototype.draw = function(data, options)
{
   
    this.data = data;
    
    var lbl = document.createElement("LABEL");
    lbl.setAttribute("class", "DualSliderCaption");
    lbl.innerText = options.caption;
    this.container.appendChild(lbl);
    
    var din = document.createElement("DIV");
    din.setAttribute("class", "DualSliderConfig");
    din.id = this.container.id + "dashboard";
    this.container.appendChild(din);
    
    var tbl = document.createElement("TABLE");
    din.appendChild(tbl);
    var r = tbl.insertRow(0);
    var c1 = r.insertCell(0);
    c1.setAttribute("width", "100%");
    this.chartPlaceHolder = document.createElement("DIV");
    this.chartPlaceHolder.setAttribute("style", "position:absolute;visibility:hidden;");
    //this.chartPlaceHolder.setAttribute("style", "width:200px;height:100px;");
    this.chartPlaceHolder.id = this.container.id + 'cchart';
    c1.appendChild(this.chartPlaceHolder);

    r = tbl.insertRow(1);
    c2 = r.insertCell(0);
    this.controlPlaceHolder = document.createElement("DIV");
    var s = "width:" + (options.width==null ? "200px" : options.width) + ";";
    s += "height:" + (options.height==null ? "50px" : options.height) + ";";
    this.controlPlaceHolder.setAttribute("style", s);
    
    this.controlPlaceHolder.id = this.container.id + 'ccontrol';
    c2.appendChild(this.controlPlaceHolder);
    
    if (options.Description)
    {
        r = tbl.insertRow(2);
        var c = r.insertCell(0);
        var txt = document.createElement("LABEL");
        c.appendChild(txt);
        txt.setAttribute("class", "DualSliderDescription");
        txt.innerText = options.Description;
    }
    
    var dashboard = new google.visualization.Dashboard(this.container);
    this.control = new google.visualization.ControlWrapper(
        {
            'controlType': 'ChartRangeFilter',
            'containerId': this.container.id + 'ccontrol',
            'options': 
            {
                // Filter by the date axis.
                'filterColumnIndex': 0,
                'ui': 
                {
                    'chartType': 'AreaChart',
                    'chartOptions': 
                    {
                        'chartArea': 
                        {
                            'width': '90%'
                        },
                        'hAxis': 
                        {
                            'baselineColor': 'none'
                        },
                        'vAxis' : 
                        {
                            'logScale' : true
                        }
                    },
                    'chartView': 
                    {
                        'columns': [0, 1]
                    },
                }
            }
        });
    
    this.chart = new google.visualization.ChartWrapper({
        'chartType': 'LineChart',
            'containerId': this.container.id + 'cchart',
            'options': {
            // Use the same chart area width as the control for axis alignment.
            'chartArea': {
                'height': '80%',
                    'width': '90%'
            },
                'hAxis': {
                'slantedText': false
            },
                'vAxis': 
                {
                    'viewWindow': 
                    {
                        'min': 0,
                        'max': 2000
                    },
                    'logScale' : false
                },
                'legend': {
                'position': 'none'
            }
        }
        ,
        'view': {'columns': [0, 1]}
    });    
    
    
    dashboard.bind(this.control, this.chart);
    dashboard.draw(this.data);    
    
    if (this.state!=null)
        this.control.setState(this.state);
    
    this.state = this.control.getState();
    
    var me = this;
    
    this.eventHandle = google.visualization.events.addListener(
        this.control, 
        'statechange', 
        function() 
        {
            //me.control.getState().start;
            //me.control.getState().end;
            google.visualization.events.trigger(me, "statechange", me.state = me.control.getState());
        }
    );    
    
};

sgweb.DualSlider.prototype.clear = function()
{
    google.visualization.events.removeListener(this.eventHandle);
    this.data = null;
    this.container = container;
    this.chart = null;
    this.control = null;
    this.chartPlaceHolder = null;
    this.controlPlaceHolder = null;
};