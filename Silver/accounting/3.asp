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

var g_account_id = 3;    // Checking (WF Shared)
g_account_name = Get_Account_Name (database, g_account_id);

Handle_Form_Input (database);

var fso = new ActiveXObject ("Scripting.FileSystemObject");
var tx_file = fso.OpenTextFile (Server.MapPath ("Transactions") + "\\stmt_" + g_account_id + ".csv", 1);
var lines = new String ();
while ((! tx_file.AtEndOfStream) && g_should_continue)
{
    // "12/12/2011","-567.00","*","","ONLINE TRANSFER REF #IBETKF7BCV TO VISA XXXXXXXXXXXX0789 ON 12/10/11"
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
            if (entry_account.match (/INTEREST PAYMENT/i))
            {
                // "12/30/2011","0.04","*","","INTEREST PAYMENT"
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_account_nickname = "Wells Fargo";
                var tx_type_id = Get_Type_Id (database, "Interest");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/NSF RETURN ITEM FEE/i))
            {
                // "06/05/2012","-25.00","*","","NSF RETURN ITEM FEE FOR ITEM $3,051.55 06/04 WF HOME MTG AUTO PAY 060212 0058498932 BRETTON S WADE"
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_account_nickname = "Wells Fargo";
                var tx_type_id = Get_Type_Id (database, "Fee");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/ONLINE XFR/i))
            {
                // "12/30/2011","4000.00","*","","BANK OF AMERICA ONLINE XFR 111230 XXXXX2452 BRETTON S WADE"
                var date_match = entry_account.match (/XFR \d+ X*\d+ /);
                if (date_match != null)
                {
                    var tx_source = entry_account.substr (0, entry_account.search (/ ONLINE XFR/));
                    var tx_source_account = entry_account.substr (entry_account.search (/XFR \d+ X*\d+ /) + date_match[0].length);
                    var tx_account_nickname = tx_source + " - " + tx_source_account;
                    var tx_type_id = Get_Type_Id (database, "Transfer");
                    //Response.Write ("Online XFR = " + tx_account_nickname + "<br>\n");
                    lines = Build_Line (database, tx_account_nickname, tx_date, 0, 0, tx_amount, tx_type_id) + lines;
                }
                else
                {
                    Response.Write ("Unmatched date in ONLINE XFR: " + tx_date + ", " + entry_account + ", " + tx_amount + "<br>\n");
                    //lines += "<tr><td><table><tr><td>UNKNOWN</td><td>" + entry_date + "</td><td>" + entry_account + "</td><td>" + tx_amount + "</td></tr></table></td></tr>\n";
                }
            }
            else if (entry_account.match (/RECURRING PAYMENT REF/i))
            {
                // "12/15/2011","-750.00","*","","RECURRING PAYMENT REF #OPETKG86MJ TO EQUITYLINE XXXXXX31401998"
                var ref_match = entry_account.match (/\#\S+ TO /);
                if (ref_match != null)
                {
                    var tx_account_nickname = entry_account.substr (entry_account.search (/\#\S+ TO /) + ref_match[0].length);
                    var tx_method_id = Get_Method_Id (database, "EFT");
                    var tx_type_id = Get_Type_Id (database, "Mortgage/Rent");
                    //Response.Write ("Recurring Payment = " + tx_account_nickname + "<br>\n");
                    lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
                }
                else
                {
                    Response.Write ("Unmatched ref in RECURRING PAYMENT REF: " + tx_date + ", " + entry_account + ", " + tx_amount + "<br>\n");
                    //lines += "<tr><td><table><tr><td>UNKNOWN</td><td>" + entry_date + "</td><td>" + entry_account + "</td><td>" + tx_amount + "</td></tr></table></td></tr>\n";
                }
            }
            else if (entry_account.match (/WF HOME MTG AUTO PAY/i))
            {
                // "12/02/2011","-3079.79","*","","WF HOME MTG AUTO PAY 120211 0058498932 BRETTON S WADE"
                var ref_match = entry_account.match (/WF HOME MTG AUTO PAY (\d\d\d\d)(\d\d) /);
                if (ref_match != null)
                {
                    var tx_account_nickname = entry_account.substr (ref_match[0].length);
                    var tx_method_id = Get_Method_Id (database, "EFT");
                    var tx_type_id = Get_Type_Id (database, "Mortgage/Rent");
                    tx_date = Parse_Entry_Date (ref_match[1] + "20" + ref_match[2]); 
                    //Response.Write ("Recurring Payment = " + tx_account_nickname + "<br>\n");
                    lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
                }
                else
                {
                    Response.Write ("Unmatched ref in WF HOME MTG AUTO PAY: " + tx_date + ", " + entry_account + ", " + tx_amount + "<br>\n");
                    //lines += "<tr><td><table><tr><td>UNKNOWN</td><td>" + entry_date + "</td><td>" + entry_account + "</td><td>" + tx_amount + "</td></tr></table></td></tr>\n";
                }
            }
            else if (entry_account.match (/MORTGAGE PAYMENT REVERSAL/i))
            {
                // "06/08/2012","3051.55","*","","MORTGAGE PAYMENT REVERSAL"
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_account_nickname = "Mortgage (Wells Fargo)";
                var tx_type_id = Get_Type_Id (database, "Refund");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/ONLINE TRANSFER REF/i))
            {
                // "12/12/2011","-567.00","*","","ONLINE TRANSFER REF #IBETKF7BCV TO VISA XXXXXXXXXXXX0789 ON 12/10/11"
                var ref_match = entry_account.match (/ONLINE TRANSFER REF \#\S+ TO (.*) ON (\d\d\/\d\d\/)(\d\d)/);
                if (ref_match != null)
                {
                    var tx_account_nickname = ref_match[1];
                    var tx_method_id = Get_Method_Id (database, "Web");
                    var tx_type_id = Get_Type_Id (database, "Mortgage/Rent");
                    tx_date = Parse_Entry_Date (ref_match[2] + "20" + ref_match[3]); 
                    //Response.Write ("ONLINE TRANSFER REF = " + tx_account_nickname + "<br>\n");
                    //Response.Write ("----> : " + tx_date + ", " + entry_account + ", " + tx_amount + "<br>\n");
                    lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
                }
                else
                {
                    Response.Write ("Unmatched ref in WF HOME MTG AUTO PAY: " + tx_date + ", " + entry_account + ", " + tx_amount + "<br>\n");
                    lines += "<tr><td><table><tr><td>UNKNOWN</td><td>" + entry_date + "</td><td>" + entry_account + "</td><td>" + tx_amount + "</td></tr></table></td></tr>\n";
                }
            }
            else
            {
                Response.Write ("Unmatched Transaction: " + tx_date + ", " + entry_account + ", " + tx_amount + "<br>\n");
                //lines += "<tr><td><table><tr><td>UNKNOWN</td><td>" + entry_date + "</td><td>" + entry_account + "</td><td>" + tx_amount + "</td></tr></table></td></tr>\n";
            }
        }
    }
}
tx_file.Close ();

database.Close ();

//------------------------------------------------------------------------------
%>

<!-- #include file="loader_page.asp" -->
