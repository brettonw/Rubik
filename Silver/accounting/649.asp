<%@ Language= "JScript" %>

<!-- #include file="loader.asp" -->

<%
//------------------------------------------------------------------------------
// this tool loads transaction lists from BofA - Visa into the main account
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// a little black raincloud, of course
//------------------------------------------------------------------------------
var database = new SQL_Database ();

var g_account_id = 649;    // Visa Card (Wells Fargo)
g_account_name = Get_Account_Name (database, g_account_id);

Handle_Form_Input (database);

var fso = new ActiveXObject ("Scripting.FileSystemObject");
var tx_file = fso.OpenTextFile (Server.MapPath ("Transactions") + "\\stmt_" + g_account_id + ".csv", 1);
var lines = new String ();
while ((! tx_file.AtEndOfStream) && g_should_continue)
{
    "02/18/2012","-169.00","*","","SQ *SCOTT PAUL DESIGNS SAN FRANCISCODC"
    var line = tx_file.ReadLine ();
    if (line.length > 1)
    {
        // condition the line...
        var regexp = /\'/g;
        line = line.replace (regexp, "&#39");
        
        // field 1 - date
        var field = line.match (/\"([^\"]*)\"/);
        line = line.substr (field[0].length + 1);
        var tx_date = Parse_Entry_Date (field[1]); 
        //Response.Write (field[1] + ", ");
        
        // field 2 - amount
        field = line.match (/\"([^\"]*)\"/);
        line = line.substr (field[0].length + 1);
        var tx_amount = parseFloat (field[1]);
        //Response.Write (field[1] + ", ");
        
        // field 3 - ignore
        field = line.match (/\"([^\"]*)\"/);
        line = line.substr (field[0].length + 1);
        //Response.Write (field[1] + ", ");
        
        // field 4 - ignore
        field = line.match (/\"([^\"]*)\"/);
        line = line.substr (field[0].length + 1);
        //Response.Write (field[1] + ", ");
        
        // field 5 - entry
        field = line.match (/\"([^\"]*)\"/);
        line = line.substr (field[0].length + 1);
        var entry_account = field[1];
        //Response.Write (field[1] + ", ");

        if (! isNaN (tx_amount))
        {
            if (entry_account.match (/INTEREST CHARGE ON PURCHASES/i))
            {
                // "02/22/2012","-310.31","*","","INTEREST CHARGE ON PURCHASES"
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_account_nickname = "Wells Fargo";
                var tx_type_id = Get_Type_Id (database, "Interest");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/LATE FEE/i))
            {
                // "02/22/2012","-35.00","*","","LATE FEE"
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_account_nickname = "Wells Fargo";
                var tx_type_id = Get_Type_Id (database, "Fee");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/PAYMENT - THANK YOU/i) || entry_account.match (/ONLINE PAYMENT/i))
            {
                // "03/20/2012","1000.00","*","","ONLINE PAYMENT"
                // "02/21/2012","1000.00","*","","PAYMENT - THANK YOU"
                var tx_account_nickname = "649 PAYMENT SOURCE";
                var tx_type_id = Get_Type_Id (database, "Revolving Credit");
                lines = Build_Line (database, tx_account_nickname, tx_date, 0, 0, tx_amount, tx_type_id) + lines;
            }
            else
            {
                // "02/18/2012","-169.00","*","","SQ *SCOTT PAUL DESIGNS SAN FRANCISCODC"
                var tx_method_id = Get_Method_Id (database, "Visa Card");
                var tx_account_nickname = entry_account;
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, 0) + lines;
            }
        }
    }
}
tx_file.Close ();

database.Close ();

//------------------------------------------------------------------------------
%>

<!-- #include file="loader_page.asp" -->
