<%
//------------------------------------------------------------------------------
function Comma_Delimit_Money (amount)
{
    var whole_amount = Math.floor (amount);
    var decimal_amount = Math.round ((amount - whole_amount) * 100);
    if (decimal_amount == 100)
    {
        decimal_amount = 0;
        whole_amount += 1;
    }
    var tenth_digit = Math.floor (decimal_amount / 10);
    var hundredth_digit = decimal_amount - (tenth_digit * 10);
    var html = new String (".") + tenth_digit + hundredth_digit;
    while (whole_amount >= 1000)
    {
        var thousands = Math.floor (whole_amount / 1000);
        var sub_thou = whole_amount - (thousands * 1000);
        whole_amount = thousands;
        var sub_thou_over_10 = Math.floor (sub_thou / 10);
        var one_digit = sub_thou - (sub_thou_over_10 * 10);
        sub_thou = sub_thou_over_10;
        sub_thou_over_10 = Math.floor (sub_thou / 10);
        var ten_digit = sub_thou - (sub_thou_over_10 * 10);
        var hundred_digit = sub_thou_over_10;
        html = new String (",") + hundred_digit + ten_digit + one_digit + html;
    }
    html = new String (Math.floor (whole_amount)) + html;
    return html;
}

//------------------------------------------------------------------------------
function Format_Money (amount)
{
    var html = new String;
    if (Math.abs (amount) < 0.01)
    {
        html = "<center>&nbsp&nbsp;-</center>";
    }
    else if (amount < 0.0)
    {
        html += Comma_Delimit_Money (-amount);
        html = new String ("(") + html + ")";
    }
    else
    {
        html += Comma_Delimit_Money (amount);
        html = new String () + html + "&nbsp;";
    }
    return "<tt>" + html + "</tt>";
}

//------------------------------------------------------------------------------
function UC_First (string)
{
    return string.substr (0, 1).toUpperCase () + string.substr (1, string.length - 1);
}

//------------------------------------------------------------------------------
%>
