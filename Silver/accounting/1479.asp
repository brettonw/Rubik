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

var g_account_id = 1479;    // Line of Credit (Clockwork 3)
g_account_name = Get_Account_Name (database, g_account_id);

Handle_Form_Input (database);

var fso = new ActiveXObject ("Scripting.FileSystemObject");
var tx_file = fso.OpenTextFile (Server.MapPath ("Transactions") + "\\stmt_" + g_account_id + ".csv", 1);
var headers = tx_file.ReadLine ();
var lines = new String ();
while ((! tx_file.AtEndOfStream) && g_should_continue)
{
    // "Company Name & Last 4","Cardholder Name & Last 4","Transaction Date","Posting Date","Merchant Name / Access Check Number","Transaction Amount","MCC","MCC Description","Transaction Type","Expense Category","Expense Sub Category","Memo"
    var line = tx_file.ReadLine ();
    if (line.match(/^\"Company/) == null)
    {
        // condition the line...
        var regexp = /\'/g;
        line = line.replace (regexp, "&#39");
        
        // field 1 - ignore
        var field = line.match (/\"([^\"]*)\"/);
        line = line.substr (field[0].length + 1);
        //Response.Write (field[1] + ", ");
        
        // field 2 - ignore
        field = line.match (/\"([^\"]*)\"/);
        line = line.substr (field[0].length + 1);
        //Response.Write (field[1] + ", ");
        
        // field 3 - tx date
        field = line.match (/\"([^\"]*)\"/);
        line = line.substr (field[0].length + 1);
        var tx_date = Parse_Entry_Date (field[1]); 
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
        
        // field 6 - amount
        field = line.match (/\"([^\"]*)\"/);
        var stripped_field = field[1].replace (/[\$\,\(\)]/g, ""); 
        line = line.substr (field[0].length + 1);
        var tx_amount = -parseFloat (stripped_field);
        if (field[1].match (/\)/))
        {
            tx_amount = -tx_amount;
            //Response.Write ("TX: " + tx_amount + "<br>\n");
        }
        //Response.Write (stripped_field + ", ");
        
        //Response.Write ("<br>\n");
        //Response.Write (tx_date + ", " + entry_account + ", " + tx_amount + "<br>\n");
        if (! isNaN (tx_amount))
        {
            if (entry_account.match (/FINANCE CHARGE/i))
            {
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_account_nickname = "Bank of America";
                var tx_type_id = Get_Type_Id (database, "Interest");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/PAYMENT - THANK YOU/i) || entry_account.match (/^Online payment/i))
            {
                //Response.Write (tx_date + ", " + entry_account + ", " + tx_amount + "<br>\n");
                var tx_account_nickname = "1479 PAYMENT SOURCE";
                var tx_type_id = Get_Type_Id (database, "Revolving Credit");
                lines = Build_Line (database, tx_account_nickname, tx_date, 0, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/LATE PAYMENT FEE/i))
            {
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_account_nickname = "Bank of America";
                var tx_type_id = Get_Type_Id (database, "Fee");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/INTERNATIONAL TRANSACTION/i))
            {
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_account_nickname = "Bank of America";
                var tx_type_id = Get_Type_Id (database, "Fee");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else
            {
                // "1/19/2012","REMINDERBAND","$111.27"
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
