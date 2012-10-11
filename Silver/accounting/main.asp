<%@ Language= "JScript" %>

<!-- #include file="sql_database.asp" -->
<!-- #include file="account.asp" -->
<!-- #include file="format.asp" -->
<!-- #include file="build.asp" -->
<!-- #include file="package.asp" -->

<%
//------------------------------------------------------------------------------
// a little black raincloud, of course
//------------------------------------------------------------------------------
var database = new SQL_Database ();
Load_Package (database);

// XXX now might change if editing a transaction, it's used for the transaction
// XXX detail and for setting up the calendar
var account_selector = Build_Account_Selector (database, g_show_preferred_accounts, g_account_id, "main", g_max_age);
var month_selector = Build_Month_Selector (g_max_age);
var age_selector = Build_Tx_Age_Selector (g_max_age);
var show_hide_cleared_tx_checkbox = Build_Cleared_Checkbox ();
var show_preferred_accounts_checkbox = Build_Preferred_Accounts_Checkbox ();
var account_transactions = Build_Transaction_List (database, g_show_hide_cleared_transactions, g_month, g_year, g_max_age);
var account_totals = Build_Transaction_Totals (database);
var transaction_detail = Build_Transaction_Detail (database, g_now, g_edit_transaction_id, g_max_age);
var account_loader_link = Build_Account_Loader_Link (database, g_account_id);

database.Close ();

//------------------------------------------------------------------------------
%>

<html>
<head>
    <link type="text/css" rel="stylesheet" href="style.css">
</head>
<body>
<table border="0" cellpadding="0" cellspacing="0">
<tr>
    <td align="left" height="30"><%=month_selector %>&nbsp;<%=age_selector %>&nbsp;<%=show_hide_cleared_tx_checkbox %>&nbsp;Show All Transactions</td>
    <td width="10"></td><td><%=account_selector %>&nbsp;<%=show_preferred_accounts_checkbox %>&nbsp;Show Preferred Accounts</td>
</tr>
<tr><td align="left"><%=account_transactions %></td><td></td><td align=left><%=transaction_detail %></td></tr>
<tr><td align="center"><%=account_totals %></td><td></td><td><center><div id="ErrorMessage" class="error"></div><%=account_loader_link %></center></td></tr>
</table>
</body>
</html>

<!-- #include file="timeout.js" -->
<!-- #include file="event.js" -->
<!-- #include file="scroll.js" -->
<!-- #include file="validate.js" -->
<!-- #include file="context_menu.asp" -->
