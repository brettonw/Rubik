<%
//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------
var table_edge_color        = "ccccee";
var table_alt_row_color     = "eeeeff";
var height_row              = 20;
var width_date              = 70;
var width_method            = 90;
var width_description       = 200;
var width_dollar            = 10;
var width_amount            = 80;
var width_icon              = height_row;
var width_scrollbar         = 16;

//------------------------------------------------------------------------------
// functions
//------------------------------------------------------------------------------
function Build_Transaction_Totals (database)
{
    // compute the balances
    var cleared_credits = Get_Account_Credits (database, g_account_id, 1);
    var cleared_debits = Get_Account_Debits (database, g_account_id, 1);
    var pending_credits = Get_Account_Credits (database, g_account_id, 0);
    var pending_debits = Get_Account_Debits (database, g_account_id, 0);
    var total_credits = cleared_credits + pending_credits;
    var total_debits = cleared_debits + pending_debits;
    var balance = total_credits - total_debits;
    var credit_limit = Get_Credit_Limit (database, g_account_id);
    
    // format the table
    var transaction_totals = new String ();
    transaction_totals += "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n";
    transaction_totals += "<tr height=\"" + height_row + "\"><th></th><th></th><th></th><th>Debit</th><th></th><th></th><th>Credit</th><th></th><th></th><th>Balance</th><th></th><th></th><th>Available</th></tr>\n";
    transaction_totals += "<tr><td></td><td colspan=\"13\" height=\"1\" bgcolor=\"" + table_edge_color + "\"></td></tr>";
    if (credit_limit > 0)
    {
        transaction_totals += "<tr bgcolor=\"" + table_alt_row_color + "\" height=\"" + height_row + "\"><td width=\"" + width_amount + "\"><b>&nbsp;&nbsp;Cleared</b></td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (cleared_debits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (cleared_credits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (cleared_credits - cleared_debits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (credit_limit + (cleared_credits - cleared_debits)) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td></tr>\n";
        transaction_totals += "<tr height=\"" + height_row + "\"><td width=\"" + width_amount + "\"><b>&nbsp;Pending</b></td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (pending_debits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (pending_credits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (pending_credits - pending_debits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td></td><td></td><td width=1 bgcolor=\"" + table_edge_color + "\"></td></tr>\n";
        transaction_totals += "<tr bgcolor=\"" + table_alt_row_color + "\" height=\"" + height_row + "\"><td width=\"" + width_amount + "\"><b>&nbsp;Total</b></td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (total_debits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (total_credits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (total_credits - total_debits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (credit_limit + (total_credits - total_debits)) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td></tr>\n";
    }
    else
    {
        transaction_totals += "<tr bgcolor=\"" + table_alt_row_color + "\" height=\"" + height_row + "\"><td width=\"" + width_amount + "\"><b>&nbsp;&nbsp;Cleared</b></td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (cleared_debits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (cleared_credits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (cleared_credits - cleared_debits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\"></td><td width=\"" + width_amount + "\"></td><td width=1 bgcolor=\"" + table_edge_color + "\"></td></tr>\n";
        transaction_totals += "<tr height=\"" + height_row + "\"><td width=\"" + width_amount + "\"><b>&nbsp;Pending</b></td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (pending_debits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (pending_credits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (pending_credits - pending_debits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td></td><td></td><td width=1 bgcolor=\"" + table_edge_color + "\"></td></tr>\n";
        transaction_totals += "<tr bgcolor=\"" + table_alt_row_color + "\" height=\"" + height_row + "\"><td width=\"" + width_amount + "\"><b>&nbsp;Total</b></td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (total_debits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (total_credits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td width=\"" + width_dollar + "\" align=\"right\">$</td><td width=\"" + width_amount + "\" align=\"right\">" + Format_Money (total_credits - total_debits) + "</td><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td></td><td></td><td width=1 bgcolor=\"" + table_edge_color + "\"></td></tr>\n";
    }
    transaction_totals += "<tr><td></td><td colspan=\"13\" height=\"1\" bgcolor=\"" + table_edge_color + "\"></td></tr>";
    transaction_totals += "</table>\n";
    return transaction_totals;
}

//------------------------------------------------------------------------------
function Build_Transaction_List (database, show_cleared, month, year, max_age)
{
    // compute the table width and height factors
    var tx_col_count            = 13;
    var width_column_dividers   = 5;
    var width_total             = width_date + width_method + width_description + width_dollar + width_amount + width_dollar + width_amount + width_icon + width_scrollbar + width_column_dividers;
    var max_rows                = 25;
    var height_total            = max_rows * height_row;
    
    // build the outer table
    var transaction_list = new String;
    transaction_list += "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n";
    transaction_list += "<tr><td colspan=\"16\" height=\"1\" bgcolor=\"" + table_edge_color + "\"></td></tr>";
    transaction_list += "<tr height=\"" + height_row + "\"><th width=\"1\" bgcolor=\"" + table_edge_color + "\"></th><th width=\"" + width_date + "\">Date</th><th width=1></th><th width=\"" + width_method + "\">Method</th><th width=1></th><th width=\"" + width_description + "\">Description</th><th width=1></th><th width=\"" + width_dollar + "\"></th><th width=\"" + width_amount + "\">Debit</th><th width=1></th><th width=\"" + width_dollar + "\"></th><th width=\"" + width_amount + "\">Credit</th><th width=1></th><th width=\"" + width_icon + "\"></th><th width=\"" + width_scrollbar + "\"></th><th width=\"1\" bgcolor=\"" + table_edge_color + "\"></th></tr>\n";
    transaction_list += "<tr><td colspan=\"16\" height=\"1\" bgcolor=\"" + table_edge_color + "\"></td></tr>";
    transaction_list += "<tr><td width=1 bgcolor=\"" + table_edge_color + "\"></td><td colspan=\"" + 14 + "\"><div id=\"transaction_list_div\" onscroll=\"On_Transaction_Scroll ();\" style=\"width:" + width_total + "px; height:" + height_total + "px; overflow-y:scroll;\">";
    
    // set up the sums
    var sum_credits = 0.0;
    var sum_debits = 0.0;

    // build the sql query for transactions
    var sql;
    sql =  "SELECT  T.ID AS ID, CONVERT(char(10), T.[Date], 102) AS Date, '0.0000' AS Debit, T.Amount AS Credit, A.Name AS Description, M.Name AS Method, T.[Method ID] AS CheckNum, T.[Cleared To] AS Cleared, P.[Name] AS Type, T.[Void] AS Void, T.[Escrow] AS Escrow, T.[Have Receipt] AS Receipt";
    sql += " FROM   Transactions AS T INNER JOIN Accounts AS A ON T.[From Account] = A.ID INNER JOIN Methods AS M ON T.Method = M.ID INNER JOIN Types AS P ON T.Type = P.ID";
    sql += " WHERE  (T.[To Account] = " + g_account_id + ")";
    if (month != 999)
        sql += " AND (MONTH(T.Date) = " + month + ") AND (YEAR(T.Date) = " + year + ")";
    else
        sql += " AND (DATEDIFF (month, [T].[Date], getdate ()) < " + max_age + ")";
    if (! show_cleared)
        sql += " AND (T.[Cleared To] = 0) AND (T.[Void] = 0)";
    sql += " UNION";
    sql += " SELECT T.ID AS ID, CONVERT(char(10), T.[Date], 102) AS Date, T.Amount AS Debit, '0.0000' AS Credit, A.Name AS Description, M.Name AS Method, T.[Method ID] AS CheckNum, T.[Cleared From] AS Cleared, P.[Name] AS Type, T.[Void] AS Void, T.[Escrow] AS Escrow, T.[Have Receipt] AS Receipt";
    sql += " FROM   Transactions AS T INNER JOIN Accounts AS A ON T.[To Account] = A.ID INNER JOIN Methods AS M ON T.Method = M.ID INNER JOIN Types AS P ON T.Type = P.ID";
    sql += " WHERE  (T.[From Account] = " + g_account_id + ")";
    if (month != 999)
        sql += " AND (MONTH(T.Date) = " + month + ") AND (YEAR(T.Date) = " + year + ")";
    else
        sql += " AND (DATEDIFF (month, [T].[Date], getdate ()) < " + max_age + ")";
    if (! show_cleared)
        sql += " AND (T.[Cleared From] = 0) AND (T.[Void] = 0)";
    sql += " ORDER BY Date, Method, CheckNum, Description, Debit, Credit";
    
    // do the query and build the transaction list
    var record_set = database.Query (sql);
    if (! record_set.EOF)
    {
        transaction_list += "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n";
        var row_count = 0;
        while (! record_set.EOF)
        {
            var transaction_id = parseInt (record_set.Fields (0));
            var re_true = /^true/i;
            var tx_pending = (re_true.exec (record_set.Fields (7)) == null);
            var tx_void = (re_true.exec (record_set.Fields (9)) != null);
            var tx_escrow = (re_true.exec (record_set.Fields (10)) != null);
            var tx_receipt = (re_true.exec (record_set.Fields (11)) != null);
            
            // color every other row a slight shade for readability
            transaction_list += "<tr height=\"" + height_row + "\"";
            transaction_list += " oncontextmenu=\"Display_Context_Menu (" + transaction_id + ", " + tx_pending + ", " + tx_escrow + ", " + tx_void + "); return false;\"";
            if (tx_void)
                transaction_list += " class=\"void\"";
            else if (tx_escrow)
                transaction_list += " class=\"escrow\"";
            else if (tx_pending)
                transaction_list += " class=\"pending\"";
            else
                transaction_list += " class=\"cleared\"";
            if (row_count & 1)
                transaction_list += " bgcolor=\"" + table_alt_row_color + "\"";
            //transaction_list += " onclick=\"Edit_Transaction (" + transaction_id + ");\"";
            transaction_list += " title=\"Transaction " + transaction_id + " for " + record_set.Fields (8) + "\">";
            row_count++;
            
            transaction_list += "<td align=\"center\" width=\"" + width_date + "\">" + record_set.Fields (1) + "</td>";                                 // Date
            transaction_list += "<td width=\"1\" bgcolor=\"" + table_edge_color + "\"></td>";
            transaction_list += "<td align=\"center\" width=\"" + width_method + "\">" + record_set.Fields (5);
            var re_check = /^check/i;
            if (re_check.exec (record_set.Fields (5)) != null)
                transaction_list += "  " + record_set.Fields (6);                                                                                   // method id (check number)
            transaction_list += "</td>";                                                                                                            // Method
            transaction_list += "<td width=\"1\" bgcolor=\"" + table_edge_color + "\"></td>";
            transaction_list += "<td align=\"left\" width=\"" + width_description + "\">&nbsp;&nbsp;" + record_set.Fields (4) + "</td>";                  // Description
            transaction_list += "<td width=\"1\" bgcolor=\"" + table_edge_color + "\"></td>";
            transaction_list += "<td align=\"right\" width=\"" + width_dollar + "\">$</td>";                                                            // Dollar Sign
            var debit = parseFloat (record_set.Fields (2));
            sum_debits += debit;
            transaction_list += "<td align=\"right\" width=\"" + width_amount + "\">" + Format_Money (debit) + "</td>";    // Debit
            transaction_list += "<td width=\"1\" bgcolor=\"" + table_edge_color + "\"></td>";
            transaction_list += "<td align=\"right\" width=\"" + width_dollar + "\">$</td>";                                                            // Dollar Sign
            var credit = parseFloat (record_set.Fields (3));
            sum_credits += credit;
            transaction_list += "<td align=\"right\" width=\"" + width_amount + "\">" + Format_Money (credit) + "</td>";    // Credit
            transaction_list += "<td width=\"1\" bgcolor=\"" + table_edge_color + "\"></td>";

            transaction_list += "<td bgcolor=\"ffffff\" width=\"" + width_icon + "\" align=\"center\">";
            if (tx_pending && (!tx_void))
            {
                //transaction_list += "<input title=\"Clear transaction [" + transaction_id + "]\" class=\"checkbox\" type=\"checkbox\" onClick=\"Clear_Transaction (" + transaction_id + ")\">";
                transaction_list += "<img src=\"/icons/heavy_ring.gif\" title=\"Clear transaction [" + transaction_id + "]\" border=\"0\" onClick=\"Clear_Transaction (" + transaction_id + ")\">";
            }
            else
                transaction_list += "<img src=\"/icons/light_ring.gif\" border=\"0\">";
            transaction_list += "</td>";
            transaction_list += "</tr>\n";
	        record_set.MoveNext ();
        }
        // add a summary row
        transaction_list += "<tr name=\"tx_summary_row\"><td colspan=\"12\" height=\"1\" bgcolor=\"" + table_edge_color + "\"></td></tr>";
        transaction_list += "<tr height=\"" + height_row + "\"";
        if (row_count & 1)
            transaction_list += " bgcolor=\"" + table_alt_row_color + "\"";
        transaction_list += ">";
        transaction_list += "<td colspan=\"5\" bgcolor=\"ffffff\" align=\"right\">Total&nbsp;&nbsp;</td>";
        transaction_list += "<td width=\"1\" bgcolor=\"" + table_edge_color + "\"></td>";
        transaction_list += "<td align=\"right\" width=\"" + width_dollar + "\">$</td>";                                    // Dollar Sign
        transaction_list += "<td align=\"right\" width=\"" + width_amount + "\">" + Format_Money (sum_debits) + "</td>";    // Debit
        transaction_list += "<td width=\"1\" bgcolor=\"" + table_edge_color + "\"></td>";
        transaction_list += "<td align=\"right\" width=\"" + width_dollar + "\">$</td>";                                    // Dollar Sign
        transaction_list += "<td align=\"right\" width=\"" + width_amount + "\">" + Format_Money (sum_credits) + "</td>";   // Credit
        transaction_list += "<td width=\"1\" bgcolor=\"" + table_edge_color + "\"></td>";
        transaction_list + "</tr>\n";

        row_count++;

        if (row_count < max_rows)
            transaction_list += "<tr><td colspan=\"5\"></td><td colspan=\"7\" height=\"1\" bgcolor=\"" + table_edge_color + "\"></td></tr>";
        transaction_list += "</table>\n";
    }
    record_set.Close ();
    transaction_list += "</div></td><td width=1 bgcolor=\"" + table_edge_color + "\"></td></tr>\n";
    transaction_list += "<tr><td colspan=16 height=1 bgcolor=\"" + table_edge_color + "\"></td></tr>\n";
    transaction_list += "</table>\n";
    return transaction_list;
}

//------------------------------------------------------------------------------
function Build_Query_Selector (database, sql, selected_id, name, call)
{
    var record_set = database.Query (sql);
    var query_selector = "<SELECT name=\"" + name + "\" onChange=\"" + call + " ()\">\n";
    //if (selected_id == 0)
        query_selector += "\t<OPTION value=\"0\">--</OPTION>\n";
    //else
    //    query_selector += "\t<OPTION value=\"0\">--</OPTION>\n";
    while (! record_set.EOF)
    {
        var record_id = parseInt (record_set.Fields (0));
        query_selector += "\t<OPTION value=\"" + record_id + "\" title=\"" + record_set.Fields (1) + "\"";
        if (record_id == selected_id)
            query_selector += " SELECTED";
        query_selector += ">" + record_set.Fields (1) + "</OPTION>\n";
	    record_set.MoveNext ();
    }
    query_selector += "</SELECT>\n";
    record_set.Close ();
    return query_selector;
}

//------------------------------------------------------------------------------
function Build_Account_Selector (database, preferred, selected_account_id, name, age)
{
    var account_selector;
    if (account_selector == undefined)
    {
        var sql = "SELECT [ID], [Name], [Preferred] FROM Accounts WHERE ([Closed] = 0) AND ";
        if (preferred)
            sql += "([Preferred] = 1)" ;
        else
            sql += "(DATEDIFF (month, [Last Update], getdate ()) <= " + age + ") ";
        sql += "ORDER BY [Name]";
        account_selector = Build_Query_Selector (database, sql, selected_account_id, "account_" + name + "_selector", "Select_Account_" + UC_First (name));
    }
    return account_selector;
}

//------------------------------------------------------------------------------
function Match_Account_Hint (database, age, hint, hint_length, selected_account_id)
{
    var hint_sql = "SELECT [ID], [Name] FROM Accounts WHERE ([Closed] = 0) AND [Name] like '" + hint.substr (0, hint_length) + "%' ORDER BY [Name]";
    var record_set = database.Query (hint_sql);
    if (! record_set.EOF)
    {
        selected_account_id = parseInt (record_set.Fields (0));
    }
    record_set.Close ();
    return selected_account_id;
}

//------------------------------------------------------------------------------
function Build_Account_Selector_With_Hint (database, name, age, hint)
{
    var account_selector;
    if (account_selector == undefined)
    {
        // strip "paypal *" and "sq *" off the front of the hint if it's there
        var matches = hint.match (/^paypal\W*/i);
        if (matches != null)
        {
            hint = hint.substr (matches[0].length);
            //Response.Write ("Hint becomes: " + hint + "<br>\n");
        }
        matches = hint.match (/^sq \W*/i);
        if (matches != null)
        {
            hint = hint.substr (matches[0].length);
            //Response.Write ("Hint becomes: " + hint + "<br>\n");
        }
    
        // try progressively longer matches to get the account hint
        var selected_account_id = Match_Account_Hint (database, age, hint, 4, 0);
        selected_account_id = Match_Account_Hint (database, age, hint, 6, selected_account_id);
        selected_account_id = Match_Account_Hint (database, age, hint, 9, selected_account_id);
        selected_account_id = Match_Account_Hint (database, age, hint, hint.length, selected_account_id);
        
        var sql = "SELECT [ID], [Name], [Preferred] FROM Accounts WHERE ([Closed] = 0) ";
        sql += " AND (DATEDIFF (month, [Last Update], getdate ()) <= " + age + ") ";
        sql += "ORDER BY [Name]";
        account_selector = Build_Query_Selector (database, sql, selected_account_id, "account_" + name + "_selector", "Select_Account_" + UC_First (name));
    }
    return account_selector;
}

//------------------------------------------------------------------------------
function Build_Method_Selector (database, selected_method_id, age)
{
    //var method_selector = Application ("method_selector");
    //if (method_selector == undefined)
    {
        var sql = "SELECT [ID], [Name] FROM Methods WHERE (DATEDIFF (month, [Last Update], getdate ()) <= " + age + ") ORDER BY [Name]";
        method_selector = Build_Query_Selector (database, sql, selected_method_id, "method_selector", "Select_Method");
        //Application ("method_selector") = method_selector;
    }
    return method_selector;
}

//------------------------------------------------------------------------------
function Build_Type_Selector (database, selected_type_id, age)
{
    //var type_selector = Application ("type_selector");
    //if (type_selector == undefined)
    {
        var sql = "SELECT [ID], [Name] FROM Types WHERE ([Parent Type] <> 9) AND (DATEDIFF (month, [Last Update], getdate ()) <= " + age + ") ORDER BY [Name]";
        type_selector = Build_Query_Selector (database, sql, selected_type_id, "type_selector", "Select_Type");
        //Application ("type_selector") = type_selector;
    }
    return type_selector;
}

//------------------------------------------------------------------------------
function Build_Tax_Deduction_Selector (database, selected_deduction_type_id, age)
{
    var tax_deduction_selector;
    if (tax_deduction_selector == undefined)
    {
        var sql = "SELECT [ID], [Name] FROM [Deduction Types] WHERE (DATEDIFF (month, [Last Update], getdate ()) <= " + age + ") ORDER BY [Name]";
        tax_deduction_selector = Build_Query_Selector (database, sql, selected_deduction_type_id, "deduction_type_selector", "Select_Deduction_Type");
    }
    return tax_deduction_selector;
}

//------------------------------------------------------------------------------
function Build_Tx_Age_Selector (max_age)
{
    var age_selector = "<SELECT name=\"age_selector\" onChange=\"Select_Age ()\">\n";
    age_selector += "\t<OPTION" + ((max_age == 3) ? " SELECTED" : "") + " value=\"3\">3 Months</OPTION>\n";
    age_selector += "\t<OPTION" + ((max_age == 6) ? " SELECTED" : "") + " value=\"6\">6 Months</OPTION>\n";
    age_selector += "\t<OPTION" + ((max_age == 9) ? " SELECTED" : "") + " value=\"9\">9 Months</OPTION>\n";
    age_selector += "\t<OPTION" + ((max_age == 12) ? " SELECTED" : "") + " value=\"12\">1 Year</OPTION>\n";
    age_selector += "\t<OPTION" + ((max_age == 18) ? " SELECTED" : "") + " value=\"18\">18 Months</OPTION>\n";
    age_selector += "\t<OPTION" + ((max_age == 24) ? " SELECTED" : "") + " value=\"24\">2 Years</OPTION>\n";
    age_selector += "\t<OPTION" + ((max_age == 60) ? " SELECTED" : "") + " value=\"60\">5 Years</OPTION>\n";
    age_selector += "\t<OPTION" + ((max_age == 120) ? " SELECTED" : "") + " value=\"120\">10 Years</OPTION>\n";
    age_selector += "\t<OPTION" + ((max_age == 600) ? " SELECTED" : "") + " value=\"600\">50 Years</OPTION>\n";
    age_selector += "</SELECT>\n";
    return age_selector;
}

//------------------------------------------------------------------------------
function Build_Month_Selector (max_age)
{
    var now = new Date ();
    var month = now.getMonth ();
    var year = now.getFullYear ();
    var month_selector = "<SELECT name=\"month_selector\" onChange=\"Select_Month ()\">\n";
    month_selector += "<OPTION value=\"999\""
    if (g_month == 999)
        month_selector += " SELECTED";
    month_selector += ">Unfiltered</OPTION>";
    var i;
    for (i = 0; i < max_age; i++)
    {
        month_selector += "\t<OPTION value=\"" + i + "\"";
        if (((month + 1) == g_month) && (year == g_year))
            month_selector += " SELECTED";
        month_selector += ">";
        switch (month)
        {
            case  0: month_selector += "January"; break;
            case  1: month_selector += "February"; break;
            case  2: month_selector += "March"; break;
            case  3: month_selector += "April"; break;
            case  4: month_selector += "May"; break;
            case  5: month_selector += "June"; break;
            case  6: month_selector += "July"; break;
            case  7: month_selector += "August"; break;
            case  8: month_selector += "September"; break;
            case  9: month_selector += "October"; break;
            case 10: month_selector += "November"; break;
            case 11: month_selector += "December"; break;
        }
        month_selector += " " + year + "</OPTION>\n";
        month--;
        if (month < 0)
        {
            month = 11;
            year--;
        }
    }
    month_selector += "</SELECT>\n";
    
    return month_selector;
}

//------------------------------------------------------------------------------
function Build_Cleared_Checkbox ()
{
    var show_hide_cleared_tx_checkbox = "<input type=\"checkbox\" class=\"checkbox\" name=\"Show_Hide_Cleared_Transactions\" onclick=\"Show_Hide_Cleared_Transactions ();\"";
    if (g_show_hide_cleared_transactions)
        show_hide_cleared_tx_checkbox += " CHECKED";
    show_hide_cleared_tx_checkbox += "/>";
    return show_hide_cleared_tx_checkbox;
}

//------------------------------------------------------------------------------
function Build_Preferred_Accounts_Checkbox ()
{
    var show_preferred_accounts_checkbox = "<input type=\"checkbox\" class=\"checkbox\" name=\"Show_Preferred_Accounts\" onclick=\"Show_Preferred_Accounts ();\"";
    if (g_show_preferred_accounts)
        show_preferred_accounts_checkbox += " CHECKED";
    show_preferred_accounts_checkbox += "/>";
    return show_preferred_accounts_checkbox;
}

//------------------------------------------------------------------------------
function Build_Transaction_Detail (database, now, transaction_id, max_age)
{
    // values
    var tx_method = 0;
    var tx_method_id = "";
    var tx_for = 0;
    var tx_deduction_type = 1;
    var tx_from_account = g_account_id;
    var tx_to_account = 0;
    var tx_amount = 0;
    var tx_tax = 0;
    var tx_tip = 0;
    var tx_cleared_to = 0;
    var tx_cleared_from = 0;
    var tx_void = 0;
    var tx_escrow = 0;
    var tx_have_receipt = 1;
    var tx_house_expense = 0;
    var tx_work_expense = 0;
    
    // fill the values
    var transaction_edit_type = "Edit";
    if (transaction_id <= 0)
    {
        transaction_id = database.Get_Next_ID ("Transactions");
        transaction_edit_type = "New";
    }
    else
    {
        // load the transaction for display
        var sql = "SELECT [Method], [Method ID], [Type], [Tax Deduction], [From Account], [To Account], [Amount], [Tax], [Tip], [Cleared To], [Cleared From], [Void], [Escrow], [Have Receipt]";
        sql += " FROM [Transactions]";
        sql += " WHERE [ID] = " + transaction_id;
        var record_set = database.Query (sql);
        if (! record_set.EOF)
        {
            var re_true = /true/i;
            var re_null = /null/i;
            tx_method = parseInt (record_set.Fields (0));
            tx_method_id = (re_null.exec (record_set.Fields (1)) == null) ? parseInt (record_set.Fields (1)) : "";
            tx_for = parseInt (record_set.Fields (2));
            tx_deduction_type = parseInt (record_set.Fields (3));
            tx_from_account = parseInt (record_set.Fields (4));
            tx_to_account = parseInt (record_set.Fields (5));
            tx_amount = parseFloat (record_set.Fields (6));
            tx_tax = parseFloat (record_set.Fields (7));
            tx_tip = parseFloat (record_set.Fields (8));
            tx_cleared_to = (re_true.exec (record_set.Fields (9)) != null);
            tx_cleared_from = (re_true.exec (record_set.Fields (10)) != null);
            tx_void = (re_true.exec (record_set.Fields (11)) != null);
            tx_escrow = (re_true.exec (record_set.Fields (12)) != null);
            tx_have_receipt = (re_true.exec (record_set.Fields (13)) != null);
            
            // XXX TODO enforce a state like attitude for cleared, void, and escrow
            
        }
        record_set.Close ();
    }

    var transaction_detail = new String ();
    var month = now.getMonth () + 1;
    var date_string = now.getFullYear () + "." + ((month < 10) ? "0" : "") + month + "." + ((now.getDate () < 10) ? "0" : "") + now.getDate ();
    
    transaction_detail += "<form name=\"TX_Detail\" action=\"main.asp?Action=Submit_" + transaction_edit_type + "_Transaction\" method=\"POST\" onsubmit=\"return Validate_Form ();\">\n";
    
    transaction_detail += "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n";
    transaction_detail += "<tr><td></td><td></td><td><iframe name=\"the_calendar_frame_last\" width=\"160px\" height=\"179px\" hspace=\"0\" marginHeight=\"0\" marginWidth=\"0\" vspace=\"0\" frameborder=\"0\" scrolling=\"no\"></iframe></td>\n";
    transaction_detail += "<td><iframe name=\"the_calendar_frame\" width=\"160px\" height=\"179px\" hspace=\"0\" marginHeight=\"0\" marginWidth=\"0\" vspace=\"0\" frameborder=\"0\" scrolling=\"no\"></iframe></tr>\n";
    transaction_detail += "<tr><td height=\"5\"></td><td width=\"5\"></td><td></td></tr>\n";
    transaction_detail += "<tr><td height=\"25\">Date:</td><td></td><td><input type=\"text\" class=\"text\" name=\"the_date_value\" value=\"" + date_string + "\"></td></tr>\n";
    transaction_detail += "<tr><td height=\"25\">ID:</td><td></td><td><input type=\"text\" class=\"text\" value=\"" + transaction_id + "\" DISABLED>";
    transaction_detail += "<input type=\"hidden\" name=\"the_id_value\" value=\"" + transaction_id + "\"></td></tr>\n";
    
    transaction_detail += "<tr><td height=\"25\">Method:</td><td></td><td>" + Build_Method_Selector (database, tx_method, max_age) + "</td>";
    transaction_detail += "<td><input type=\"text\" class=\"text\" name=\"the_method_value\"></td>";
    transaction_detail += "</tr>\n";
    transaction_detail += "<tr><td height=\"25\">Check #:</td><td></td><td><input type=\"text\" class=\"text\" name=\"the_transaction_number\" value=\"" + tx_method_id + "\"></td></tr>\n";
    
    transaction_detail += "<tr><td height=\"25\">Type:</td><td></td><td>" + Build_Type_Selector (database, tx_for, max_age) + "</td>";
    transaction_detail += "<td><input type=\"text\" class=\"text\" name=\"the_type_value\"></td>";
    transaction_detail += "</tr>\n";
    
    transaction_detail += "<tr><td height=\"25\">Deduction:</td><td></td><td>" + Build_Tax_Deduction_Selector (database, tx_deduction_type, max_age) + "</td>";
    transaction_detail += "<td><input type=\"text\" class=\"text\" name=\"the_tax_deduction_value\"></td>";
    transaction_detail += "</tr>\n";
    
    transaction_detail += "<tr><td height=\"25\">From:</td><td></td>";
    transaction_detail += "<td>" + Build_Account_Selector (database, g_show_preferred_accounts, tx_from_account, "from", max_age) + "&nbsp;</td>";
    transaction_detail += "<td><input type=\"text\" class=\"text\" name=\"the_from_value\"></td>";
    transaction_detail += "<td><input type=\"checkbox\" class=\"checkbox\" name=\"the_cleared_from\"" + (tx_cleared_from ? "CHECKED" : "") + ">&nbsp;Cleared</td>\n";
    transaction_detail += "</tr>\n";
    
    transaction_detail += "<tr><td height=\"25\">To:</td><td></td>";
    transaction_detail += "<td>" + Build_Account_Selector (database, false, tx_to_account, "to", max_age) + "&nbsp;</td>";
    transaction_detail += "<td><input type=\"text\" class=\"text\" name=\"the_to_value\"></td>";
    transaction_detail += "<td><input type=\"checkbox\" class=\"checkbox\" name=\"the_cleared_to\"" + (tx_cleared_to ? "CHECKED" : "") + ">&nbsp;Cleared</td>\n";
    transaction_detail += "</tr>\n";
    
    transaction_detail += "<tr><td height=\"25\">Amount:</td><td></td><td><input type=\"text\" class=\"text\" name=\"the_amount_value\" value=\"" + tx_amount + "\"></td></tr>\n";
    transaction_detail += "<tr><td height=\"25\">Tip:</td><td></td><td><input type=\"text\" class=\"text\" name=\"the_tip_value\" value=\"" + tx_tip + "\"></td>";
    transaction_detail += "<tr><td height=\"25\">Tax:</td><td></td><td><input type=\"text\" class=\"text\" name=\"the_tax_value\" value=\"" + tx_tax + "\"></td>";
    transaction_detail += "<td><input type=\"button\" onclick=\"CalculateTaxFromTotal (0.06);\" value=\"&nbsp;6.00%&nbsp;\"></td></tr>\n";

    
    transaction_detail += "<tr><td height=\"25\"></td><td></td><td align=\"center\">";
    transaction_detail += "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr>\n";
    transaction_detail += "<td align=\"center\" width=\"50\"><input type=\"checkbox\" class=\"checkbox\" name=\"the_void\"" + (tx_void ? "CHECKED" : "") + "></td>";
    transaction_detail += "<td align=\"center\" width=\"50\"><input type=\"checkbox\" class=\"checkbox\" name=\"the_escrow\"" + (tx_escrow ? "CHECKED" : "") + "></td>";
    transaction_detail += "<td align=\"center\" width=\"50\"><input type=\"checkbox\" class=\"checkbox\" name=\"the_have_receipt\"" + (tx_have_receipt ? "CHECKED" : "") + "></td>";
    transaction_detail += "</tr><tr><td align=\"center\">Void</td><td align=\"center\">Escrow</td><td align=\"center\">Receipt</td></table>\n";
    transaction_detail += "</td></tr>\n";
    
    transaction_detail += "<tr><td height=\"25\"></td><td></td><td align=\"center\"><input type=\"submit\" value=\"SUBMIT\">";
    if (transaction_edit_type == "Edit")
        transaction_detail += "&nbsp;<input type=\"button\" onclick=\"Edit_Transaction (0);\" value=\"CANCEL\">\n";
    transaction_detail += "</td></tr>\n";
    transaction_detail += "</table>\n";
    transaction_detail += "</form>";
    return transaction_detail;
}

//------------------------------------------------------------------------------
function Build_Account_Loader_Link (database, account_id)
{
    var account_loader_link = new String ();
    // find out if the account has a loader
    var sql = "SELECT [Has Loader], [Name]";
    sql += " FROM [Accounts]";
    sql += " WHERE [ID] = " + account_id;
    var record_set = database.Query (sql);
    if (! record_set.EOF)
    {
        var re_true = /true/i;
        var has_loader = (re_true.exec (record_set.Fields (0)) != null);
        if (has_loader)
        {
            account_loader_link = "<p><a target=\"_blank\" href=\"" + account_id + ".asp\">Transaction Loader (" + account_id + ")</a></p>";
        }
    }
    record_set.Close ();
    return account_loader_link;
}

//------------------------------------------------------------------------------
%>
