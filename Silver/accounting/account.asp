<!-- #include file="user.asp" -->

<%
//------------------------------------------------------------------------------
function Get_Account_Credits (database, account_id, cleared)
{
    var sql = "SELECT SUM (Amount) AS Credits FROM Transactions";
        sql += " WHERE [To Account] = " + account_id + " AND Void = 0 AND Escrow = 0";
        sql += " AND [Cleared To] = " + cleared;
    return database.Query_Float (sql);
}

//------------------------------------------------------------------------------
function Get_Account_Debits (database, account_id, cleared)
{
    var sql = "SELECT SUM (Amount) AS Credits FROM Transactions";
        sql += " WHERE [From Account] = " + account_id + " AND Void = 0 AND Escrow = 0";
        sql += " AND [Cleared From] = " + cleared;
    return database.Query_Float (sql);
}

//------------------------------------------------------------------------------
function Get_Credit_Limit (database, account_id)
{
    var sql = "SELECT [Credit Limit] FROM [Accounts] WHERE [ID]=" + account_id;
    return database.Query_Float (sql);
}

//------------------------------------------------------------------------------
%>
