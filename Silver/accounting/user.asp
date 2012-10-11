<%
//------------------------------------------------------------------------------
/*
    Permissions:
        - Access Pages
        - View Account - needs clarity, on account by account basis - a separate table?
        - View Cleared Transactions
        - View Pending Transactions
        - Submit New Transactions
        - Clear Transactions (including Split)
        - Edit Transactions
        
    Users
        - Permissions
        - Default Account
*/
//------------------------------------------------------------------------------
var g_default_account_id = 0;

//------------------------------------------------------------------------------
function Get_Default_Account_ID (database)
{
    if (g_default_account_id == 0)
    {
        var domain_user = Request.ServerVariables ("LOGON_USER");
        //Response.Write (Request.ServerVariables ("LOGON_USER") + "<p>\n");
        var sql = "SELECT [Default Account] FROM Users WHERE [Username] LIKE '" + domain_user + "'";
        var record_set = database.Query (sql);
        if (! record_set.EOF)
        {
            var re_null = /^null/i;
            if (re_null.exec (record_set.Fields (0)) == null)
                g_default_account_id = parseInt (record_set.Fields (0));
        }
        record_set.Close ();
    }
    return g_default_account_id;
}

//------------------------------------------------------------------------------
%>
