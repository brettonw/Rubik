<%@ Language= "JScript" %>
<%
    var asp_selectable_bg_color     = "fff8f0";
    var asp_selectable_text_color   = "4444ee";
    var asp_selected_bg_color       = "eebbbb";
    var asp_selected_text_color     = "0000cc";
    var asp_today_color             = "ff0000";
    var asp_grid_color              = "ccccee";
    
    if (Request.QueryString ("Selectable_BG").Count == 1)
        asp_selectable_bg_color = Request.QueryString ("Selectable_BG");
    if (Request.QueryString ("Selectable_Text").Count == 1)
        asp_selectable_text_color = Request.QueryString ("Selectable_Text");
        
    if (Request.QueryString ("Selected_BG").Count == 1)
        asp_selected_bg_color = Request.QueryString ("Selected_BG");
    if (Request.QueryString ("Selected_Text").Count == 1)
        asp_selected_text_color = Request.QueryString ("Selected_Text");
        
    if (Request.QueryString ("Today").Count == 1)
        asp_today_color = Request.QueryString ("Today");
    if (Request.QueryString ("Grid").Count == 1)
        asp_grid_color = Request.QueryString ("Grid");

    var today = new Date ();
    if (Request.QueryString ("Year").Count == 1)
        today.setYear (parseInt (Request.QueryString ("Year")));
    if (Request.QueryString ("Month").Count == 1)
        today.setMonth (parseInt (Request.QueryString ("Month")) - 1);
    if (Request.QueryString ("Day").Count == 1)
        today.setDate (parseInt (Request.QueryString ("Day")));
        
    var highlight_date = 0;
    if (Request.QueryString ("Highlight_Date").Count == 1)
        highlight_date = parseInt (Request.QueryString ("Highlight_Date"));
%>
<html>
<head>
<style>
    body
    {
        background-color    : #fff8f0;
        margin              : 0px 0px 0px 0px;
    }
    table
    {
        background-color    : <%=asp_selectable_bg_color%>;
        color               : <%=asp_selectable_text_color%>;
        font-family         : arial;
        font-size           : 11px;
        line-height		    : 15px;
    }
    .selectable
    {
        cursor              : hand;
        background-color    : <%=asp_selectable_bg_color%>;
        color               : <%=asp_selectable_text_color%>;
    }
    .selected
    {
        cursor              : hand;
        background-color    : <%=asp_selected_bg_color%>;
        color               : <%=asp_selected_text_color%>;
    }
</style>
</head>
<body><div id="the_calendar_div" align=center></div></body>
</html>

<script type="text/javascript" language="javascript">
//------------------------------------------------------------------------------
var g_cell_size = 20;

//------------------------------------------------------------------------------
function Is_Leap_Year (year)
{
    // check to see if it is exactly divisible by 4
    if ((year % 4) == 0)
    {
        // it might be a leap year, check if it is a century
		if ((year % 100) == 0)
		{
			// it is not a leap year unless it is every 4th century
			return ((year % 400) == 0);
		}
		else
		{
		    // it is a leap year
			return true;
		}
	}
	else
	{
	    // not a leap year
		return false;
	}
}

//------------------------------------------------------------------------------
function Get_Days_In_Month (month, is_leap_year)
{
    switch (month)
    {
        case 0:  return 31;                     // january
        case 1:  if (is_leap_year) return 29;   // february  in leap year
                 return 28;                     // february
        case 2:  return 31;                     // march
        case 3:  return 30;                     // april
        case 4:  return 31;                     // may
        case 5:  return 30;                     // june
        case 6:  return 31;                     // july
        case 7:  return 31;                     // august
        case 8:  return 30;                     // september
        case 9:  return 31;                     // october
        case 10: return 30;                     // november
        case 11: return 31;                     // december
    }
}

//------------------------------------------------------------------------------
function Get_Month_Name (month)
{
    switch (month)
    {
        case 0:  return "January";
        case 1:  return "February";
        case 2:  return "March";
        case 3:  return "April";
        case 4:  return "May";
        case 5:  return "June";
        case 6:  return "July";
        case 7:  return "August";
        case 8:  return "September";
        case 9:  return "October";
        case 10: return "November";
        case 11: return "December";
    }
}

//------------------------------------------------------------------------------
function Draw_Day (day, today, month, year)
{
    var output = new String();
    output += "<td height=" + g_cell_size + " width=1 bgcolor=<%=asp_grid_color%>></td>";
    output += "<td align=center width=" + g_cell_size + " height=" + g_cell_size + ">";
    if (day == today)
    {
        output += "<table cellpadding=0 cellspacing=0><tr>";
        output += "<td width=" + (g_cell_size - 2) + " height=" + (g_cell_size - 2) + " bgcolor=<%=asp_today_color%> align=center>";
        output += "<table cellpadding=0 cellspacing=0><tr>";
        output += "<td width=" + (g_cell_size - 4) + " height=" + (g_cell_size - 4) + " class=\"selectable\"";
        output += " onmouseover=\"Highlight_Tag (this, 1);\" onmouseout=\"Highlight_Tag(this, 0);\"";
        output += " onclick=\"parent.Select_Date (" + year + ", " + month + ", " + day + ")\" align=center>";
        output += day;
        output += "</td></tr></table>";
        output += "</td></tr></table>";
    }
    else
    {
        output += "<table cellpadding=0 cellspacing=0><tr>";
        output += "<td width=" + (g_cell_size - 2) + " height=" + (g_cell_size - 2) + " align=center>";
        output += "<table cellpadding=0 cellspacing=0><tr>";
        output += "<td width=" + (g_cell_size - 4) + " height=" + (g_cell_size - 4) + " class=\"selectable\"";
        output += " onmouseover=\"Highlight_Tag (this, 1);\" onmouseout=\"Highlight_Tag(this, 0);\"";
        output += " onclick=\"parent.Select_Date (" + year + ", " + month + ", " + day + ")\" align=center>";
        output += day;
        output += "</td></tr></table>";
        output += "</td></tr></table>";
    }
    output += "</td>";
    return output;
}

//------------------------------------------------------------------------------
function Highlight_Tag (tag, is_highlighted)
{
    if (is_highlighted)
    {
        tag.className = "selected";
    }
    else
    {
        tag.className = "selectable";
    }
}

//------------------------------------------------------------------------------
function Go_Prev_Month (day, month, year)
{
    month--;
    if (month < 0)
    {
        month = 11;
        year--;
    }
    
    the_calendar_div.innerHTML = Calendar_Month (day - 30, month, year, "selected", "selectable");
}

//------------------------------------------------------------------------------
function Go_Next_Month (day, month, year)
{
    month++;
    if (month > 11)
    {
        month = 0;
        year++;
    }
    
    the_calendar_div.innerHTML = Calendar_Month (day + 30, month, year, "selectable", "selected");
}

//------------------------------------------------------------------------------
// parameters are the day to hilight (today, 0 if none), month (0=jan),
// and year to show
function Calendar_Month (today, month, year, prev_class, next_class)
{
    var is_leap_year = Is_Leap_Year (year);
    var first_of_month = new Date (year, month, 1);
    var days_in_month = Get_Days_In_Month (month, is_leap_year);
    var first_day_of_month = first_of_month.getDay ();
    
    // open the table, add a border
    var output = "<table border=0 cellspacing=0 cellpadding=0>\n";
    output += "<tr height=1><td width=1 bgcolor=<%=asp_grid_color%>></td><td bgcolor=<%=asp_grid_color%>></td><td width=1 bgcolor=<%=asp_grid_color%>></td></tr>\n";
    output += "<tr><td bgcolor=<%=asp_grid_color%>></td><td>\n";
    
    // an interior table, display the month, the year, and the DOW headings
    output += "<table border=0 cellspacing=0 cellpadding=0>\n";
    output += "<tr height=5><td></td></tr>";
    output += "<tr><td width=5></td><td></td><td align=center class=\"" + prev_class + "\" onmouseover=\"Highlight_Tag (this, 1);\" onmouseout=\"Highlight_Tag(this, 0);\" onclick=\"Go_Prev_Month(" + today + ", " + month + ", " + year + ");\">&lt;</td>";
    output += "<td colspan=11 align=center height=" + g_cell_size + ">" + Get_Month_Name (month) + " " + year + "</td>";
    output += "<td align=center class=\"" + next_class + "\" onmouseover=\"Highlight_Tag (this, 1);\" onmouseout=\"Highlight_Tag(this, 0);\" onclick=\"Go_Next_Month(" + today + ", " + month + ", " + year + ");\">&gt;</td><td></td><td width=5></td></tr>\n";
    output += "<tr><td></td><td></td><td height=" + g_cell_size + " align=center>Su</td><td></td><td align=center>Mo</td><td></td>";
    output += "<td align=center>Tu</td><td></td><td align=center>We</td><td></td><td align=center>Th</td>";
    output += "<td></td><td align=center>Fr</td><td></td><td align=center>Sa</td><td></td><td></td></tr>\n";
    output += "<tr><td></td><td colspan=15 height=1 bgcolor=<%=asp_grid_color%>></td><td></td></tr>\n";
    output += "<tr><td></td>";
    
    // output the days up to the first day
    var rows = 0;
    var cur = 0;
    while (cur < first_day_of_month)
    {
        output += "<td height=" + g_cell_size + " width=1 bgcolor=<%=asp_grid_color%>></td>";
        output += "<td></td>";
        cur++;
    }
    
    // output all of the days of the month
    var day = 1;
    while (day <= days_in_month)
    {
        if (cur < 7)
        {
            output += Draw_Day (day, today, month, year);
            day++;
            cur++;
        }
        else
        {
            cur = 0;
            output += "<td height=" + g_cell_size + " width=1 bgcolor=<%=asp_grid_color%>></td><td></td></tr>\n";
            output += "<tr><td></td><td colspan=15 height=1 bgcolor=<%=asp_grid_color%>></td><td></td></tr>\n";
            output += "<tr><td></td>";
            rows++;
        }
    }
    
    // finish out the table with the days after the end of the month
    if (cur != 0)
        while (cur < 7)
        {
            output += "<td height=" + g_cell_size + " width=1 bgcolor=<%=asp_grid_color%>></td>";
            output += "<td></td>";
            cur++;
        }
        
    // close the table and return the result
    output += "<td height=" + g_cell_size + " width=1 bgcolor=<%=asp_grid_color%>></td><td></td></tr>\n";
    output += "<tr><td></td><td colspan=15 height=1 bgcolor=<%=asp_grid_color%>></td><td></td></tr>\n";
    rows++;

    while (rows < 6)
    {
        output += "<tr height=" + g_cell_size + "><td></td></tr>\n";
        output += "<tr height=1><td></td></tr>\n";
        rows++;
    }
    output += "<tr height=5><td></td></tr>";
    output += "</table>\n";
    
    output += "</td><td bgcolor=<%=asp_grid_color%>></td></tr>\n";
    output += "<tr height=1><td bgcolor=<%=asp_grid_color%>></td><td bgcolor=<%=asp_grid_color%>></td><td bgcolor=<%=asp_grid_color%>></td></tr>\n";
    output += "</table>\n";
    
    return output;
}

//------------------------------------------------------------------------------

var today = new Date (<%=today.getFullYear () %>, <%=today.getMonth () %>, <%=today.getDate () %>);
var highlight = <%=highlight_date%>;//<% highlight_date > 0 ? "1" : "0" %>;
document.getElementById ("the_calendar_div").innerHTML = Calendar_Month ((highlight > 0) ? today.getDate () : 0, today.getMonth (), today.getYear (), "selectable", "selectable");
</script>
