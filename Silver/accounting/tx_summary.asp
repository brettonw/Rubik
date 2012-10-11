<%@ Language= "JScript" %>
<!-- #include file="sql_database.asp" -->

<%
    var g_summary_year = 2011;
    var g_account_ids = "1490";
    var database = new SQL_Database ();
    var debit_records = "[";
    var sql = "SELECT CONVERT(char(10), Tx.[Date], 102) AS [Date], Ac.[Name] AS [To Account], Tx.[Amount] As [Amount], Tx.[Tax] AS [Tax], De.[Name] AS [Deduction], Ty.[Name] AS [Type] " + 
              "FROM [Transactions] AS Tx, [Accounts] AS Ac, [Types] AS Ty, [Deduction Types] AS De " +
              "WHERE (Tx.[From Account] IN (" + g_account_ids + ")) " + 
              "AND (Year(Tx.[Date]) = " + g_summary_year + ") " +
              "AND (Ac.[ID] = Tx.[To Account]) " + 
              "AND (Ty.[ID] = Tx.[Type]) " + 
              "AND (De.[ID] = Tx.[Tax Deduction]) " + 
              "ORDER BY [Date]";
    record_set = database.Query (sql);
    if (! record_set.EOF)
    {
        while (! record_set.EOF)
        {
            debit_records += "{"
                + "date:\"" + record_set.Fields (0) + "\", "
                + "to:\"" + record_set.Fields (1) + "\", "
                + "amount:" + record_set.Fields (2) + ", "
                + "tax:" + record_set.Fields (3) + ", "
                + "taxDeduction:\"" + record_set.Fields (4) + "\", "
                + "type:\"" + record_set.Fields (5) + "\""
                + "},";
	        record_set.MoveNext ();
        }
    }
    debit_records += "]";
    record_set.Close ();
    database.Close ();
    var lines = "";
%>
<html>
<head>
    <title>Transaction Summary</title>
    <link type="text/css" rel="stylesheet" href="style_report.css">
</head>
<body>
<center>
<b>Transactions for <%=g_summary_year %></b>
<div id="debit_summary"></div>
</center>
<script type="text/javascript">
    var commaDelimitMoney = function(amount) {
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

    var formatMoney = function(amount) {
        var html = new String;
        if (Math.abs (amount) < 0.01)
        {
            html = "<center>&nbsp&nbsp;-</center>";
        }
        else if (amount < 0.0)
        {
            html += commaDelimitMoney (-amount);
            html = new String ("(") + html + ")";
        }
        else
        {
            html += commaDelimitMoney (amount);
            html = new String () + html + "&nbsp;";
        }
        //return "<tt>" + html + "</tt>";
        return html;
    }
    
    var processDebits = function () {
        var debit_records = <%=debit_records %>;
        
        // sort the debit records into "by type" and "by account"
        var types = {};
        var deductions = {};
        var accounts = {}; 
        for (var i = 0; i < debit_records.length; ++i) {
            var record = debit_records[i];
            if (!(record.type in types)) {
                types[record.type] = [];
            }
            types[record.type].push (record);
            
            if (!(record.to in accounts)) {
                accounts[record.to] = [];
            }
            accounts[record.to].push (record);
            
            if (!(record.taxDeduction in deductions)) {
                deductions[record.taxDeduction] = [];
            }
            deductions[record.taxDeduction].push (record);
        }
        
        // now print out a report for each entry, including:
        // sum, average Tx, monthly average, and tx list
        var reportOn = function(name, records, doSummary) {
            var report = "<table class='report'>\n";
            report += "<tr class='header'><td colspan=4><b>" + name + "</b></td></tr>\n";
            report += "<tr><th width=80>Date</th><th width=120>For</th><th width=250>To</th><th width=100>Amount</th></tr>\n";
            var sum = 0;
            var count = records.length;
            for (var i = 0; i < count; ++i) {
                var record = records[i];
                sum += record.amount;
                report += (((i % 2) == 0) ? "<tr class='odd'>" : "<tr>") +
                    "<td align='center'>" + record.date + "</td>" +
                    "<td align='center'>" + record.type + "</td>" +
                    "<td align='left'>" + record.to + "</td>" +
                    "<td align='right'><table border=0 width='100%' class='amount'><tr><td align='left'>$</td><td align='right'>" + formatMoney(record.amount) + "</td></tr></table></td>" +
                    "</tr>\n";
            }
            report += "<tr class='summary'><th colspan=3 align='left'>SUM</td><td align='right'><table border=0 width='100%' class='amount'><tr><td align='left'>$</td><td align='right'>" + formatMoney(sum) + "</td></tr></table></td></tr>\n";
            if (doSummary) {
                report += "<tr class='summary'><th colspan=3 align='left'>AVERAGE</td><td align='right'><table border=0 width='100%' class='amount'><tr><td align='left'>$</td><td align='right'>" + formatMoney(sum / count) + "</td></tr></table></td></tr>\n";
                if (count != 12) {
                    report += "<tr class='summary'><th colspan=3 align='left'>MONTHLY</td><td align='right'><table border=0 width='100%' class='amount'><tr><td align='left'>$</td><td align='right'>" + formatMoney(sum / 12) + "</td></tr></table></td></tr>\n";
                }
            }
            report += "</table><br>\n";
            return report;
        };
        
        // types
        var divText = "<b>Types</b>\n";
        for (var type in types) {
            if(types.hasOwnProperty (type)) {
                divText += reportOn(type, types[type], true);
            }
        } 
        
        // deductions
        divText += "<hr>";
        divText += "<b>Deductions</b>\n";
        for (var deduction in deductions) {
            if(deductions.hasOwnProperty (deduction)) {
                divText += reportOn(deduction, deductions[deduction], false);
            }
        } 
        
        // accounts
        divText += "<hr>";
        divText += "<b>Accounts</b>\n";
        for (var account in accounts) {
            if(accounts.hasOwnProperty (account)) {
                divText += reportOn(account, accounts[account], true);
            }
        } 
        
        document.getElementById("debit_summary").innerHTML = divText;
    };
    processDebits ();
</script>
</body>
</html>