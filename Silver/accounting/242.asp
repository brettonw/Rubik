<%@ Language= "JScript" %>

<!-- #include file="loader.asp" -->

<%
//------------------------------------------------------------------------------
// this tool loads transaction lists from BofA into the main account
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// a little black raincloud, of course
//------------------------------------------------------------------------------
var database = new SQL_Database ();

var g_account_id = 242;    // Visa Card (US Bank)
g_account_name = Get_Account_Name (database, g_account_id);

Handle_Form_Input (database);

var fso = new ActiveXObject ("Scripting.FileSystemObject");
var tx_file = fso.OpenTextFile (Server.MapPath ("Transactions") + "\\stmt_" + g_account_id + ".csv", 1);
var lines = new String ();
while ((! tx_file.AtEndOfStream) && g_should_continue)
{
    var line = tx_file.ReadLine ();
    if (line.match(/^\d?\d/) != null)
    {
        // condition the line...
        var regexp = /\'/g;
        line = line.replace (regexp, "&#39");
        
        // Date,Transaction,Name,Memo,Amount
        // 12/22/2011,DEBIT,SQ *NEEDLE PLAY DESIGN BURLNGTN,24692161356000387578076; 05699;,-415.00
        var entries = line.split (",");
        var tx_date = Parse_Entry_Date (entries[0]);
        var entry_account = entries[2];
        var tx_amount = parseFloat (entries[4]);
        //Response.Write ("Transaction: " + tx_date + ", " + entry_account + ", " + tx_amount + "<br>\n");

        if ((! isNaN (tx_amount)) && (tx_amount != 0))
        {
            if (entry_account.match (/INTEREST CHARGE ON PURCHASES/i))
            {
                // 12/23/2011,DEBIT,INTEREST CHARGE ON PURCHASES,1; 00000;,-164.02
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_account_nickname = "US Bank";
                var tx_type_id = Get_Type_Id (database, "Interest");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
                //Response.Write ("Transaction: " + tx_date + ", " + entry_account + ", " + tx_amount + "<br>\n");
            }
            else if (entry_account.match (/PAYMENT THANK YOU/i))
            {
                // 1/18/2012,CREDIT,PAYMENT THANK YOU,FG 97800000; 00000;,1000.00
                var tx_account_nickname = "242 PAYMENT SOURCE";
                var tx_type_id = Get_Type_Id (database, "Revolving Credit");
                lines = Build_Line (database, tx_account_nickname, tx_date, 0, 0, tx_amount, tx_type_id) + lines;
                //Response.Write ("Transaction: " + tx_date + ", " + entry_account + ", " + tx_amount + "<br>\n");
            }
            else if (entry_account.match (/LATE FEE/i))
            {
                // 3/22/2012,DEBIT,LATE FEE - PAYMENT DUE ON 03/22,00000;,-35.00
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_account_nickname = "US Bank";
                var tx_type_id = Get_Type_Id (database, "Fee");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else
            {
                // "02/18/2012","-169.00","*","","SQ *SCOTT PAUL DESIGNS SAN FRANCISCODC"
                var tx_method_id = Get_Method_Id (database, "Visa Card");
                var tx_account_nickname = entry_account;
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, 0) + lines;
                //Response.Write ("Transaction: " + tx_date + ", " + entry_account + ", " + tx_amount + "<br>\n");
            }
        }
    } 
}
tx_file.Close ();

database.Close ();

//------------------------------------------------------------------------------
%>

<!-- #include file="loader_page.asp" -->
