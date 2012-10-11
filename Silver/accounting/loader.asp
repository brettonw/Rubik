<!-- #include file="sql_database.asp" -->
<!-- #include file="account.asp" -->
<!-- #include file="format.asp" -->
<!-- #include file="build.asp" -->
<!-- #include file="package.asp" -->

<%
//------------------------------------------------------------------------------
// this tool loads transactions from accounts
//------------------------------------------------------------------------------
var g_account_name = null;
var g_line_counter = 0;
var g_should_continue = 1;
var g_page = 0;
var g_page_size = 15;
var g_prev_page_link = new String ();
var g_next_page_link = new String ("(Last Page)");
var g_tx_dictionary = new Object ();

//------------------------------------------------------------------------------
function Parse_Entry_Date (entry_date)
{
    // get the month, day, and year from the entry
    var entry_monthdayyear = entry_date.match (/(\d?\d)\/?(\d?\d)\/?(\d?\d?\d\d)/);
    var entry_month = (entry_monthdayyear[1] .length == 2) ? entry_monthdayyear[1] : ("0" + entry_monthdayyear[1]);
    var entry_day = (entry_monthdayyear[2].length == 2) ? entry_monthdayyear[2] : ("0" + entry_monthdayyear[2]);
    var tx_year = parseInt (entry_monthdayyear[3]) + ((entry_monthdayyear[3].length == 4) ? 0 : 2000);
    
    return tx_year + "." + entry_month + "." + entry_day;
}

//------------------------------------------------------------------------------
function Compute_Transaction_Date (entry_date, tx_date)
{
    // when a transaction date is given only as the month and day, we need to make sure to get the right year
    // associated with it. We do this by using the entry date on the transaction line (which does have the year)
    // and checking to see if the difference rolled over the new year on 1/1
    
    //Response.Write ("tx_date = " + tx_date + "<br>\n");
    // get the month and day (1126 or 11/26) from the tx
    var tx_monthday = tx_date.match (/(\d\d)\/?(\d\d)/);
    //Response.Write ("tx_monthday = " + tx_monthday + "<br>\n");
    var tx_month = tx_monthday[1];
    var tx_day = tx_monthday[2];
    
    // get the month, day, and year from the entry
    var entry_monthdayyear = entry_date.match (/(\d\d)\/?(\d\d)\/?(\d\d\d\d)/);
    var entry_month = entry_monthdayyear[1];
    var entry_day = entry_monthdayyear[2];
    var tx_year = parseInt (entry_monthdayyear[3]);
    
    // check that the year hasn't wrapped
    var tx_combo = parseInt (tx_month + tx_day, 10);
    var entry_combo = parseInt (entry_month + entry_day, 10);
    //Response.Write ("entry_date = " + entry_date + ", tx_date = " + tx_date + ", entry_combo = " + entry_combo + ", tx_combo = " + tx_combo + "<br>\n");
    if (tx_combo > entry_combo)
    {
        tx_year--;
    }
    return tx_year + "." + tx_month + "." + tx_day;
}

//------------------------------------------------------------------------------
function Get_Account_Id (database, tx_account)
{
    // the default is an invalid account specifier
    var tx_account_id = null;
    var sql = new String ();
    sql = "SELECT [ID] FROM [Accounts] WHERE [Name] = '" + tx_account + "'";
    record_set = database.Query (sql);
    if (! record_set.EOF)
    {
        tx_account_id = new String (record_set.Fields (0));
    }
    record_set.Close ();
    return tx_account_id;
}

//------------------------------------------------------------------------------
function Match_Account_Id (database, account_nickname)
{
    // the default is an invalid account specifier
    var account_id = 0;
    if (account_nickname != null)
    {
        // try to find the account in the account nicknames table
        var sql = "SELECT [Account]";
        sql += " FROM [Account Nicknames]";
        sql += " WHERE [Nickname] = '" + account_nickname + "'";

        var record_set = database.Query (sql);
        if (! record_set.EOF)
        {
            account_id = new String (record_set.Fields (0));
        }
        else
        {
            // no account found, we return null
        }
        record_set.Close ();
    }
    return account_id;
}

//------------------------------------------------------------------------------
function Get_Method_Id (database, tx_method)
{
    var tx_method_id = null;
    
    var sql = new String ();
    sql = "SELECT [ID] FROM [Methods] WHERE [Name] = '" + tx_method + "'";
    record_set = database.Query (sql);
    if (! record_set.EOF)
    {
        tx_method_id = new String (record_set.Fields (0));
    }
    record_set.Close ();
    
    return tx_method_id;
}

//------------------------------------------------------------------------------
function Get_Type_Id (database, tx_type)
{
    var tx_type_id = null;
    
    var sql = new String ();
    sql = "SELECT [ID] FROM [Types] WHERE [Name] = '" + tx_type + "'";
    record_set = database.Query (sql);
    if (! record_set.EOF)
    {
        tx_type_id = new String (record_set.Fields (0));
    }
    record_set.Close ();
    
    return tx_type_id;
}

//------------------------------------------------------------------------------
function Match_Transaction_Tolerance (database, tx_date, tx_account_id, tx_amount, tx_method_id, tx_number, tx_tolerance_days, tx_exclude_ids)
{
    var tx_id = null;
    
    // figure the accounts to query for
    var to_account_id = 0;
    var from_account_id = 0;
    if (tx_amount < 0)
    {
        // tx_account_id is the to account
        to_account_id = tx_account_id;
        from_account_id = g_account_id;
        tx_amount = -tx_amount;
    }
    else
    {
        // tx_account_id is the from account
        from_account_id = tx_account_id;
        to_account_id = g_account_id;
    }
    
    // set up for some queries
    var sql;
    var record_set;
    
    // look up the transaction by specific details - it might not be there for
    // the exact date - so we might consider looking at +/- a few days
    sql = "SELECT [ID] FROM [Transactions] WHERE ";
    sql += "([Amount] = " + tx_amount + ") ";
    if (tx_number == 0)
    {
        //sql += "AND (([Method ID] = 0) ";
        sql += "AND (ABS (DATEDIFF (day, [Date], CONVERT (smalldatetime, '" + tx_date + "', 102))) <= " + tx_tolerance_days + ") ";
    }
    else
    {
        // if this fails to match - it will fail multiple times in the repeated queries...
        sql += "AND (([Method ID] = " + tx_number + ") ";
        sql += "OR ";
        sql += "(([Method ID] = 0) AND (ABS (DATEDIFF (day, [Date], CONVERT (smalldatetime, '" + tx_date + "', 102))) <= " + tx_tolerance_days + "))) ";
        //Response.Write ("SQL = " + sql + "<br>\n");
    }
    if (from_account_id > 0)
    {
        sql += "AND ([From Account] = " + from_account_id + ") ";
    }
    if (to_account_id > 0)
    {
        sql += "AND ([To Account] = " + to_account_id + ") ";
    }
    sql += "AND ([Void] = 0) ";
    if (tx_method_id > 0)
    {
        sql += "AND ([Method] = " + tx_method_id + ") ";
    }
    if (tx_exclude_ids != null)
    {
        sql += "AND ([ID] NOT IN (" + tx_exclude_ids + ")) ";
        //Response.Write ("SQL = " + sql + "<br>\n");
    }
    sql += "ORDER BY [Date] ";
    
    //Response.Write ("SQL = " + sql + "<br>\n");
    
    record_set = database.Query (sql);
    if (! record_set.EOF)
    {
        tx_id = new String (record_set.Fields (0));
        //Response.Write ("tx_id = " + tx_id + "<br>\n");
        
        /*
        if (tx_id == 14660)
        {
            Response.Write ("SQL = " + sql + "<br>\n");
        }
        */
        
        record_set.MoveNext ();
        if (! record_set.EOF)
        {
            //Response.Write ("WARNING: multiple records matching<br>\n");
        }
    }
    record_set.Close ();
    
    return tx_id;
}

//------------------------------------------------------------------------------
function Match_Transaction (database, tx_date, tx_account_id, tx_amount, tx_method_id, tx_number, tx_exclude_ids)
{
    // try to match the transaction exactly, then move out if it doesn't match
    // NOTE this isn't perfect - if the same transaction happens multiple times
    // NOTE on the same day...
    //Response.Write ("Days Tolerance = " + tx_tolerance_days + "<br>\n");
    var tolerance = 6;
    var tx_id = null;
    for (var counter = 1; (counter <= tolerance) && (tx_id == null); ++counter)
    {
        tx_id = Match_Transaction_Tolerance (database, tx_date, tx_account_id, tx_amount, tx_method_id, tx_number, counter, tx_exclude_ids);
    }
    return tx_id;
}

//------------------------------------------------------------------------------
function Mark_Transaction_Cleared (database, tx_id, tx_amount)
{
    var sql;
    if (tx_amount < 0)
    {
        // clear to
        sql = "UPDATE [Transactions] SET [Cleared From] = 1, [Void] = 0, [Escrow] = 0 WHERE [ID] = " + tx_id;
    }
    else
    {
        // clear to
        sql = "UPDATE [Transactions] SET [Cleared To] = 1, [Void] = 0, [Escrow] = 0 WHERE [ID] = " + tx_id;
    }
    
    // XXX need to touch the accounts involved, here? ... but they get touched when
    // XXX transactions are inserted...
    //Response.Write ("SQL = " + sql + "<br>\n");
    database.Query (sql);  
}

//------------------------------------------------------------------------------
function Get_Default_Type_Id (database, tx_account)
{
    var tx_type_id = null;
    
    var sql = new String ();
    sql = "SELECT [Default Type] FROM [Default Types] WHERE [Account] = '" + tx_account + "'";
    record_set = database.Query (sql);
    if (! record_set.EOF)
    {
        tx_type_id = new String (record_set.Fields (0));
    }
    record_set.Close ();
    
    return tx_type_id;
}

//------------------------------------------------------------------------------
function Handle_Set_Default_Type (database)
{
    if (Request.Form ("make_default_type").Count > 0)
    {
        // get the type value to use
        var the_type_id = Request.Form ("type_selector");
        if (the_type_id == 0)
        {
            var the_type_name = Request.Form ("the_type_value");
            if (the_type_name != "")
            {
                the_type_id = Get_Type_Id (database, the_type_name);
            }
        }
        
        var the_account_id = null;
        var the_amount = parseFloat (Request.Form ("the_amount_value"));
        if (the_amount > 0)
        {
            // get the from account id to use
            the_account_id = Request.Form ("account_to_selector");
            if (the_account_id == 0)
            {
                var the_to_name = Request.Form ("the_to_value");
                if (the_to_name != "")
                {
                    the_account_id = Get_Account_Id (database, the_to_name);
                }
            }
        }
        else
        {
            // get the from account id to use
            the_account_id = Request.Form ("account_from_selector");
            if (the_account_id == 0)
            {
                var the_from_name = Request.Form ("the_from_value");
                if (the_from_name != "")
                {
                    the_account_id = Get_Account_Id (database, the_from_name);
                }
            }
        }
        
        if ((the_type_id > 0) && (the_account_id > 0))
        {
            // make sure there isn't already an entry like this one
            if (Get_Default_Type_Id (database, the_account_id) != null)
            {
                // update the record in the database
                var sql = "UPDATE [Default Types] SET [Default Type] = " + the_type_id + " WHERE [Account] = " + the_account_id;
                database.Query (sql);
            }
            else
            {
                // add a default type to the default types database
                var the_id = database.Get_Next_ID ("Default Types");
                if (the_id == 0)
                    the_id = database.Create_Next_ID ("Default Types");

                // insert the record into the database
                var sql = "INSERT INTO [Default Types] ([ID], [Account], [Default Type])";
                sql += " VALUES (" + the_id + ", " + the_account_id + ", '" + the_type_id + "')";
                //Response.Write ("SQL = " + sql);
                database.Query (sql);
                
                database.Set_Next_ID ("Default Types", the_id + 1);
            }
        }
    }
}

//------------------------------------------------------------------------------
function Handle_Form_Input_Nickname (database)
{
    if (Request.Form ("tx_loader_account_nickname").Count > 0)
    {
        var tx_loader_account_nickname  = new String (Request.Form ("tx_loader_account_nickname"));
        var tx_loader_account_id = Request.Form ("account_tx_loader_selector");
        var tx_loader_account_name  = new String (Request.Form ("tx_loader_account_value"));

        // condition the nickname...
        var regexp = /\'/g;
        tx_loader_account_nickname = tx_loader_account_nickname.replace (regexp, "&#39");
            
        // check to see if we need to create an account
        if ((tx_loader_account_name != "") && (tx_loader_account_id == 0))
        {
            tx_loader_account_id = database.Create_Name ("Accounts", tx_loader_account_name);
        }
        
        // check for multiples?
        if ((tx_loader_account_id == 0) && (Request.Form ("the_allow_multiple").Count > 0))
        {
            tx_loader_account_id = -1;
        }

        // check for multiples?
        if ((tx_loader_account_id > 0) || (Request.Form ("the_allow_multiple").Count > 0))
        {
            // create a new nickname linked to the selected account
            var the_id = database.Get_Next_ID ("Account Nicknames");
            if (the_id == 0)
                the_id = database.Create_Next_ID ("Account Nicknames");
                
            // insert the record into the database
            var sql = "INSERT INTO [Account Nicknames] ([ID], [Account], [Nickname])";
            sql += " VALUES (" + the_id + ", " + tx_loader_account_id + ", '" + tx_loader_account_nickname + "')";
            //Response.Write ("SQL = " + sql);
            database.Query (sql);

            var now = new Date ();
            var month = now.getMonth () + 1;
            var date_string = now.getFullYear () + "." + ((month < 10) ? "0" : "") + month + "." + ((now.getDate () < 10) ? "0" : "") + now.getDate ();
            database.Touch_Name ("Accounts", date_string, tx_loader_account_id);

            database.Set_Next_ID ("Account Nicknames", the_id + 1);
        }
    }
}

//------------------------------------------------------------------------------
function Handle_Form_Input (database)
{
    // handle a missing nickname request
    Handle_Form_Input_Nickname (database);
    
    // handle a transaction submission
    if (Request.Form ("the_date_value").Count > 0)
    {
        Submit_New_Transaction (database);
        Handle_Set_Default_Type (database);
    }
    
    // figure the page
    if (Request.QueryString ("Page").Count == 1)
    {
        g_page = parseInt (Request.QueryString ("Page"));
        //Response.Write ("Page: " + g_page + "<br>\n");
    }
}

//------------------------------------------------------------------------------
function Get_Account_Name (database, account_id)
{
    var account_name = "UNKNOWN";
    
    var sql = "SELECT [Name]";
    sql += " FROM [Accounts]";
    sql += " WHERE [ID] = " + account_id;
    //Response.Write ("SQL = " + sql + "<br>\n");
    
    var record_set = database.Query (sql);
    if (! record_set.EOF)
    {
        account_name = new String (record_set.Fields (0));
    }
    record_set.Close ();
    
    return account_name;
}

//------------------------------------------------------------------------------
function Build_Line (database, tx_account_nickname, tx_date, tx_method_id, tx_number, tx_amount, tx_type_id)
{
    var line = new String ();
    
    // check the line counter
    var first_line = g_page * g_page_size;
    var last_line = first_line + g_page_size;
    if ((g_line_counter >= first_line) && (g_line_counter < last_line))
    {
        // try to match the account - but it may not be possible if we 
        // don't have account info
        var tx_account_id = Match_Account_Id (database, tx_account_nickname);
        if ((tx_account_id == 0) && (tx_account_nickname != null))
        {
            line = Build_Account_Form (database, tx_date, tx_account_nickname, tx_amount);
            g_should_continue = 0;
        }

        // if we have account info, or know that account info is not determinable, we 
        // can still proceed
        if (g_should_continue)
        {
            // look at the transaction ids for this amount
            var tx_amount_string = "" + tx_amount;
            var tx_exclude_ids = g_tx_dictionary[tx_amount_string];
            
            // see if the transaction already exists
            var tx_id = Match_Transaction (database, tx_date, tx_account_id, tx_amount, tx_method_id, tx_number, tx_exclude_ids);
            if (tx_id != null)
            {
                // check to see if we've already matched this transaction
                if (tx_exclude_ids == null)
                {
                    g_tx_dictionary[tx_amount_string] = tx_id;
                }
                else
                {
                    var tx_ids = g_tx_dictionary[tx_amount_string].split (",");
                    var tx_ids_length = tx_ids.length;
                    for (var i = 0; i < tx_ids_length; i++)
                    {
                        var tx_id_item = tx_ids[i];
                        if (tx_id_item == tx_id)
                        {
                            Response.Write ("WARNING: Duplicate transaction (" + tx_id + ")<br>\n");
                        }
                    }
                    g_tx_dictionary[tx_amount_string] = tx_exclude_ids + "," + tx_id;
                }
                
                // mark this transaction as cleared
                Mark_Transaction_Cleared (database, tx_id, tx_amount);
                tx_amount = (tx_amount < 0) ? -tx_amount : tx_amount;
                var on_account = (tx_account_id > 0) ? (" on account " + Get_Account_Name (database, tx_account_id)) : "";
                var check_number = (tx_number > 0) ? (" Tx # " + tx_number) : "";
                line = "<tr><td colspan=\"14\">Clear tx id = " + tx_id + " for $" + Format_Money (tx_amount) + "on " + tx_date + on_account + check_number + "</td></tr>\n";
            }
            else
            {
                // add a form to submit the unknown transaction
                line = Build_Transaction_Form (database, tx_date, tx_method_id, tx_number, g_account_id, tx_account_id, tx_amount, tx_type_id);
                ++g_line_counter; 
            }
        }
    }
    else
    {
        if (g_line_counter < first_line)
        {
            g_prev_page_link = "<a target=\"_top\" href=\"" + g_account_id + ".asp?Page=" + (g_page - 1) + "\">Prev Page</a> ";
        }
        if (g_line_counter >= last_line)
        {
            g_next_page_link = "<a target=\"_top\" href=\"" + g_account_id + ".asp?Page=" + (g_page + 1) + "\">Next Page</a>";
        }
    }
    return line;
}

//------------------------------------------------------------------------------
function Build_Account_Form (database, tx_date, tx_account_nickname, tx_amount)
{
    var form_entry = "<tr><td colspan=\"14\">";
    form_entry += "<form name=\"TX_Loader_Unknown_Account\" action=\"" + g_account_id + ".asp?Page=" + g_page + "\" method=\"POST\">\n<table>\n";
    form_entry += "<tr>\n";
    form_entry += "<td>NEED ACCOUNT</td>\n";
    form_entry += "</tr>\n";
    form_entry += "<tr>\n";
    form_entry += "<td align=\"center\">Date</td>\n";
    form_entry += "<td align=\"center\">Amount</td>\n";
    form_entry += "<td align=\"center\">Nickname</td>\n";
    form_entry += "<td align=\"center\">Account</td>\n";
    form_entry += "<td align=\"center\">Multiple</td>\n";
    form_entry += "</tr>\n";
    form_entry += "<tr>\n";
    form_entry += "<td>" + tx_date + "</td>\n";
    form_entry += "<td>" + tx_amount + "</td>\n";
    form_entry += "<td><input type=\"text\" class=\"text\" name=\"tx_loader_account_nickname\" value=\"" + tx_account_nickname + "\"></td>\n";
    form_entry += "<td>" + Build_Account_Selector_With_Hint (database, "tx_loader", 120, tx_account_nickname) + "</td>\n";
    form_entry += "<td><input type=\"checkbox\" width=\"40\" name=\"the_allow_multiple\"></td>\n";
    form_entry += "<td height=\"25\"><td align=\"center\"><input type=\"submit\" value=\"SUBMIT\"></td>";
    form_entry += "</tr>\n";
    form_entry += "<tr>\n";
    form_entry += "<td></td>\n";
    form_entry += "<td></td>\n";
    form_entry += "<td></td>\n";
    form_entry += "<td><input type=\"text\" class=\"text\" name=\"tx_loader_account_value\"></td>\n";
    form_entry += "<td></td>\n";
    form_entry += "<td></td>\n";
    form_entry += "</tr>\n";
    form_entry += "</table>\n</form>\n</td></tr>\n";
    return form_entry;
}

//------------------------------------------------------------------------------
function Build_Transaction_Form (database, tx_date, tx_method_id, tx_number, tx_from_id, tx_to_id, tx_amount, tx_type_id)
{
    var ageForSelector = 36;
    // add a form to submit the unknown transaction
    var is_from_tx = (tx_amount < 0);
    tx_amount = is_from_tx ? -tx_amount : tx_amount;
    var form_entry = "<tr><td>\n";
    form_entry += "<form name=\"TX_Loader_Unknown_Transaction\" action=\"" + g_account_id + ".asp?Page=" + g_page + "\" method=\"POST\"  onsubmit=\"return Validate_Form (this);\">\n";
    form_entry += "<input type=\"hidden\" name=\"the_date_value\" value=\"" + tx_date + "\">\n";
    form_entry += "<input type=\"hidden\" name=\"the_id_value\" value=\"" + 0 + "\">\n";
    form_entry += "<input type=\"hidden\" name=\"the_amount_value\" value=\"" + tx_amount + "\">\n";
    if (is_from_tx)
    {
        form_entry += "<input type=\"hidden\" name=\"the_cleared_from\" value=\"1\">\n";
        //form_entry += "<input type=\"hidden\" name=\"the_cleared_to\" value=\"0\">\n";
    }
    else
    {
        //form_entry += "<input type=\"hidden\" name=\"the_cleared_from\" value=\"0\">\n";
        form_entry += "<input type=\"hidden\" name=\"the_cleared_to\" value=\"1\">\n";
    }
    
    // these two are omitted because the loader checks for their presence by counting
    //form_entry += "<input type=\"hidden\" name=\"the_void\" value=\"0\">\n";
    //form_entry += "<input type=\"hidden\" name=\"the_escrow\" value=\"0\">\n";

    form_entry += "<table>\n";
    form_entry += "<tr>\n";
    form_entry += "<td align=\"center\">#</td>\n";
    form_entry += "<td align=\"center\">Date</td>\n";
    form_entry += "<td align=\"center\">Method</td>\n";
    form_entry += "<td align=\"center\">Tx #</td>\n";
    form_entry += "<td align=\"center\">From</td>\n";
    form_entry += "<td align=\"center\">To</td>\n";
    form_entry += "<td colspan=\"2\" align=\"center\">Amount</td>\n";
    form_entry += "<td align=\"center\">Tip</td>\n";
    form_entry += "<td align=\"center\">Tax</td>\n";
    form_entry += "<td align=\"center\">Type</td>\n";
    form_entry += "<td align=\"center\">Deduction</td>\n";
    form_entry += "<td align=\"center\">Receipt</td>\n";
    form_entry += "</tr>\n";
    form_entry += "<tr>\n";
    form_entry += "<td width=\"20\">" + (g_line_counter + 1) + "</td>\n";
    form_entry += "<td width=\"60\">" + tx_date + "</td>\n";
    form_entry += "<td>" + Build_Method_Selector (database, tx_method_id, ageForSelector) + "</td>\n";
    form_entry += "<td><input type=\"text\" name=\"the_transaction_number\" value=\"" + tx_number + "\"></td>\n";

    if (is_from_tx)
    {
        form_entry += "<td>" + Build_Account_Selector (database, true, tx_from_id, "from", 3) + "</td>\n";
        form_entry += "<td>" + Build_Account_Selector (database, false, tx_to_id, "to", ageForSelector) + "</td>\n";
    }
    else
    {
        form_entry += "<td>" + Build_Account_Selector (database, false, tx_to_id, "from", ageForSelector) + "</td>\n";
        form_entry += "<td>" + Build_Account_Selector (database, true, tx_from_id, "to", 3) + "</td>\n";
    }

    form_entry += "<td>$</td><td width=\"55\" align=\"right\">" + Format_Money (tx_amount) + "</td>\n";

    form_entry += "<td><input type=\"text\" name=\"the_tip_value\" value=\"" + 0 + "\"></td>\n";
    form_entry += "<td><input type=\"text\" name=\"the_tax_value\" value=\"" + 0 + "\"></td>\n";
    
    if (tx_type_id == 0)
    {
        // look up the default type for the account
        var default_type = Get_Default_Type_Id (database, is_from_tx ? tx_to_id : tx_from_id);
        if (default_type != null)
        {
            tx_type_id = default_type;
        }
    }
    form_entry += "<td>" + Build_Type_Selector (database, tx_type_id, ageForSelector) + "</td>\n";
    form_entry += "<td>" + Build_Tax_Deduction_Selector (database, 1, ageForSelector) + "</td>\n";
    form_entry += "<td><input type=\"checkbox\" width=\"40\" name=\"the_have_receipt\" CHECKED></td>\n";

    form_entry += "<td align=\"center\"><input type=\"submit\" name=\"the_submit_button\" value=\"SUBMIT\"></td></tr>\n";
    
    form_entry += "<tr>\n";
    form_entry += "<td></td>\n";
    form_entry += "<td></td>\n";
    form_entry += "<td><input type=\"text\" class=\"text\" name=\"the_method_value\"></td>\n";    // method
    form_entry += "<td></td>\n";
    form_entry += "<td><input type=\"text\" class=\"text\" name=\"the_from_value\"></td>\n";    // from
    form_entry += "<td><input type=\"text\" class=\"text\" name=\"the_to_value\"></td>\n";    // to
    form_entry += "<td colspan=\"2\"></td>\n";
    form_entry += "<td><input type=\"button\" onclick=\"Calculate_Tip(this.form, 0.15);\" value=\"15%\"></td>\n";    // tip
    form_entry += "<td><input type=\"button\" onclick=\"Calculate_Tax(this.form, 0.06);\" value=\"6.00%\"></td>\n";
    form_entry += "<td><input type=\"text\" class=\"text\" name=\"the_type_value\"></td>\n";   // type
    form_entry += "<td><input type=\"text\" class=\"text\" name=\"the_tax_deduction_value\"></td>\n";  // deduction type
    form_entry += "<td></td>\n";
    form_entry += "</tr>\n";
    
    // make_default_type
    form_entry += "<tr>\n";
    form_entry += "<td></td>\n";
    form_entry += "<td></td>\n";
    form_entry += "<td></td>\n";
    form_entry += "<td></td>\n";
    form_entry += "<td></td>\n";
    form_entry += "<td></td>\n";
    form_entry += "<td colspan=\"2\"></td>\n";
    form_entry += "<td></td>\n";
    form_entry += "<td></td>\n";
    form_entry += "<td><input type=\"checkbox\" width=\"40\" name=\"make_default_type\">Default</td>\n";
    form_entry += "<td></td>\n";
    form_entry += "<td></td>\n";
    form_entry += "</tr>\n";
    
    form_entry += "</table>\n";
    form_entry += "</form>\n";
    form_entry += "</td></tr>\n";
    return form_entry;
}

//------------------------------------------------------------------------------
// a little black raincloud, of course
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
%>
