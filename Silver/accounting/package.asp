<%
//------------------------------------------------------------------------------
// package
//------------------------------------------------------------------------------
var g_action                            = "Unknown";
var g_account_id                        = 0;
var g_month                             = 0;
var g_year                              = 0;
var g_max_age                           = 12;
var g_show_hide_cleared_transactions    = 0;
var g_show_preferred_accounts           = 1;
var g_now                               = new Date ();
var g_edit_transaction_id               = 0;

//------------------------------------------------------------------------------
// functions
//------------------------------------------------------------------------------
function Load_Session (database)
{
    // all variables are loaded from the session, don't even check for validity
    // if it's wrong it should crash so we can track it down
    g_account_id = Session ("Select_Account_Main");
    g_show_hide_cleared_transactions = Session ("Show_Hide_Cleared_Transactions");
    g_show_preferred_accounts = Session ("Show_Preferred_Accounts");
    g_now = Session ("Now");
    g_month = Session ("Select_Month");
    g_year = Session ("Year");
    g_edit_transaction_id = Session ("Edit_Transaction");
    g_max_age = Session ("Select_Age");
}

//------------------------------------------------------------------------------
function Load_Account (database)
{
    var account_id = Session ("Select_Account_Main");
	// change the main account list and store the result with the session
	// should it reset the date and tx hiding flags?
}

//------------------------------------------------------------------------------
function Unknown (database)
{
    // this function will get invoked on first load, and also if a reload occurs
    // the session variables are loaded in Load_Default, so we don't do anything
    // in the event of a reload, we tell because the session will be undefined 
    // if it is the first load
    if (Session ("Select_Account_Main") == undefined)
    {
        Session ("Select_Account_Main") = Get_Default_Account_ID (database);
        Load_Account (database);

        // set the default flag options
        Session ("Show_Hide_Cleared_Transactions") = 0;
        Session ("Show_Preferred_Accounts") = 1;
        
        // set the month and year so the default display is all transactions
        var now = new Date ();
        Session ("Now") = now;
        Session ("Select_Month") = 999;
        Session ("Year") = now.getFullYear ();
        
        // set the transaction id for new tx
        Session ("Edit_Transaction") = 0;
        
        // set the default max age to 3 months
        Session ("Select_Age") = 3;
    }
}

//------------------------------------------------------------------------------
function Select_Account_Main (database)
{
    // set g_account_id to the requested account id
    // rebuild the main account list and store it in the session
    if (Request.QueryString ("Select_Account_Main").Count == 1)
    {
        Session ("Select_Account_Main") = parseInt (Request.QueryString ("Select_Account_Main"));
        Load_Account (database);
    }
}

//------------------------------------------------------------------------------
function Show_Hide_Cleared_Transactions (database)
{
    if (Request.QueryString ("Show_Hide_Cleared_Transactions").Count == 1)
        Session ("Show_Hide_Cleared_Transactions") = parseInt (Request.QueryString ("Show_Hide_Cleared_Transactions"));
}

//------------------------------------------------------------------------------
function Show_Preferred_Accounts (database)
{
    if (Request.QueryString ("Show_Preferred_Accounts").Count == 1)
        Session ("Show_Preferred_Accounts") = parseInt (Request.QueryString ("Show_Preferred_Accounts"));
}

//------------------------------------------------------------------------------
function Select_Month (database)
{
    if (Request.QueryString ("Select_Month").Count == 1)
    {
        // month comes in as a delta from "now" (0..600)
        // 999 is a special code for "show all transactions"
        // calculate the actual month and year as a result
        var month = parseInt (Request.QueryString ("Select_Month"));
        if (month != 999)
        {
            var now = new Date ();
            var month = (now.getMonth () + 1) - month;
            var year = now.getFullYear ();
            while (month < 1)
            {
                month += 12;
                year--;
            }
            Session ("Year") = year;
        }
        Session ("Select_Month") = month;
    }
}

//------------------------------------------------------------------------------
function Select_Age (database)
{
    if (Request.QueryString ("Select_Age").Count == 1)
        Session ("Select_Age") = parseInt (Request.QueryString ("Select_Age"));
}

//------------------------------------------------------------------------------
function Clear_Transaction (database)
{
    if (Request.QueryString ("Clear_Transaction").Count == 1)
    {
        var clear_transaction = parseInt (Request.QueryString ("Clear_Transaction"));
        
        // this should only happen if the page is otherwise already loaded, so
        // we'll try to grab the viewing account from the session
        var account_id = Session ("Select_Account_Main");
        if (account_id != undefined)
        {
            // two queries to capture all the cases - it would be two queries to 
            // figure out which to do and then do it anyway, so this is simpler
            // clear to
            var sql = "UPDATE [Transactions] SET [Cleared To] = 1, [Void] = 0, [Escrow] = 0 WHERE [ID] = " + clear_transaction + " AND [To Account] = " + account_id;
            database.Query (sql);  

            // clear from
            sql = "UPDATE [Transactions] SET [Cleared From] = 1, [Void] = 0, [Escrow] = 0 WHERE [ID] = " + clear_transaction + " AND [From Account] = " + account_id;
            database.Query (sql);  
        }
    }
}

//------------------------------------------------------------------------------
function Pending_Transaction (database)
{
    if (Request.QueryString ("Pending_Transaction").Count == 1)
    {
        var pending_transaction = parseInt (Request.QueryString ("Pending_Transaction"));
        
        // this should only happen if the page is otherwise already loaded, so
        // we'll try to grab the viewing account from the session
        var account_id = Session ("Select_Account_Main");
        if (account_id != undefined)
        {
            // two queries to capture all the cases - it would be two queries to 
            // figure out which to do and then do it anyway, so this is simpler
            // clear to
            var sql = "UPDATE [Transactions] SET [Cleared To] = 0, [Void] = 0, [Escrow] = 0 WHERE [ID] = " + pending_transaction + " AND [To Account] = " + account_id;
            database.Query (sql);  

            // clear from
            sql = "UPDATE [Transactions] SET [Cleared From] = 0, [Void] = 0, [Escrow] = 0 WHERE [ID] = " + pending_transaction + " AND [From Account] = " + account_id;
            database.Query (sql);  
        }
    }
}

//------------------------------------------------------------------------------
function Void_Transaction (database)
{
    if (Request.QueryString ("Void_Transaction").Count == 1)
    {
        var void_transaction = parseInt (Request.QueryString ("Void_Transaction"));
        var sql = "UPDATE [Transactions] SET [Void] = 1, [Escrow] = 0, [Cleared To] = 0, [Cleared From] = 0 WHERE [ID] = " + void_transaction;
        database.Query (sql);  
    }
}

//------------------------------------------------------------------------------
function Escrow_Transaction (database)
{
    if (Request.QueryString ("Escrow_Transaction").Count == 1)
    {
        var escrow_transaction = parseInt (Request.QueryString ("Escrow_Transaction"));
        var sql = "UPDATE [Transactions] SET [Escrow] = 1, [Void] = 0, [Cleared To] = 0, [Cleared From] = 0 WHERE [ID] = " + escrow_transaction;
        database.Query (sql);  
    }
}

//------------------------------------------------------------------------------
function Submit_New_Transaction (database)
{
    // pull the form variables
    var the_date                = Request.Form ("the_date_value");
    var the_id                  = Request.Form ("the_id_value");
    var the_method_id           = Request.Form ("method_selector");
    var the_method_name         = Request.Form ("the_method_value");
    var the_transaction_number  = Request.Form ("the_transaction_number");
    var the_type_id             = Request.Form ("type_selector");
    var the_type_name           = Request.Form ("the_type_value");
    var the_deduction_type_id   = Request.Form ("deduction_type_selector");
    var the_deduction_type_name = Request.Form ("the_tax_deduction_value");
    var the_from_name           = Request.Form ("the_from_value");
    var the_from_id             = Request.Form ("account_from_selector");
    var the_to_name             = Request.Form ("the_to_value");
    var the_to_id               = Request.Form ("account_to_selector");
    var the_amount              = Request.Form ("the_amount_value");
    var the_tax                 = Request.Form ("the_tax_value");
    var the_tip                 = Request.Form ("the_tip_value");
    var the_cleared_from        = (Request.Form ("the_cleared_from").Count > 0) ? 1 : 0;
    var the_cleared_to          = (Request.Form ("the_cleared_to").Count > 0) ? 1 : 0;
    var the_void                = (Request.Form ("the_void").Count > 0) ? 1 : 0;
    var the_escrow              = (Request.Form ("the_escrow").Count > 0) ? 1 : 0;
    var the_have_receipt        = (Request.Form ("the_have_receipt").Count > 0) ? 1 : 0;
    
    // XXX This could be a transaction entry - to ensure that all of the queries succeed
    
    // check to see if we need to create a method
    if ((the_method_name != "") && (the_method_id == 0))
        the_method_id = database.Create_Name ("Methods", the_method_name);
    database.Touch_Name ("Methods", the_date, the_method_id);
    
    // check to see if we need to create a type
    if ((the_type_name != "") && (the_type_id == 0))
        the_type_id = database.Create_Name ("Types", the_type_name);
    database.Touch_Name ("Types", the_date, the_type_id);
    
    // check to see if we need to create a tax deduction type
    if ((the_deduction_type_name != "") && (the_deduction_type_id == 0))
        the_deduction_type_id = database.Create_Name ("Deduction Types", the_deduction_type_name);
    database.Touch_Name ("Deduction Types", the_date, the_deduction_type_id);
    
    // check to see if we need to create an account
    if ((the_from_name != "") && (the_from_id == 0))
        the_from_id = database.Create_Name ("Accounts", the_from_name);
    if ((the_to_name != "") && (the_to_id == 0))
        the_to_id = database.Create_Name ("Accounts", the_to_name);
    database.Touch_Name ("Accounts", the_date, the_from_id);
    database.Touch_Name ("Accounts", the_date, the_to_id);
        
    // create the next transaction id - we were pretending that the one shown
    // is the one we'll use, but that's only true when editing transactions
    // XXX can this step be made atomic? probably with a special query...
    var the_id = database.Get_Next_ID ("Transactions");
    if (the_id == 0)
        the_id = database.Create_Next_ID ("Transactions");
    database.Set_Next_ID ("Transactions", the_id + 1);
        
    // insert the record into the database
    var sql = "INSERT INTO [Transactions] ([ID], [Date], [Type], [Method], [Method ID], [Tax Deduction], [From Account], [To Account], [Amount], [Tax], [Tip], [Cleared From], [Cleared To], [Void], [Escrow], [Have Receipt])";
    sql += " VALUES (" + the_id + ", CONVERT (smalldatetime, '" + the_date + "', 102), " + the_type_id + ", " + the_method_id + ", '" + the_transaction_number + "', " + the_deduction_type_id + ", " + the_from_id + ", " + the_to_id + ", " + the_amount + ", " + the_tax + ", " + the_tip + ", " + the_cleared_from + ", " + the_cleared_to + ", " + the_void + ", " + the_escrow + ", " + the_have_receipt + ")";
    database.Query (sql);
}

//------------------------------------------------------------------------------
function Edit_Transaction (database)
{
    if (Request.QueryString ("Edit_Transaction").Count == 1)
    {
        var edit_transaction = parseInt (Request.QueryString ("Edit_Transaction"));
        Session ("Edit_Transaction") = edit_transaction;
        if (edit_transaction == 0)
        {
            // cancel was requested
            Session ("Now") = new Date ();
        }
        else
        {
            var sql = "SELECT CONVERT(char(10), [Date], 102) AS Date From [Transactions] WHERE [ID] = " + edit_transaction;
            var record_set = database.Query (sql);
            if (! record_set.EOF)
            {
                var re_date = /(\d+)\.0*(\d+)\.0*(\d+)/i;
                if (re_date.exec (record_set.Fields (0)) != null)
                {
                    var now = new Date();
                    now.setYear (RegExp.$1);
                    now.setMonth (parseInt (RegExp.$2) - 1);
                    now.setDate (RegExp.$3);
                    Session ("Now") = now;
                    //Response.Write (record_set.Fields (0) + " (" + RegExp.$1 + "." + RegExp.$2 + "." + RegExp.$3 + ")<p>\n");
                }
            }
            record_set.Close ();
        }
    }
}

//------------------------------------------------------------------------------
function Submit_Edit_Transaction (database)
{
    // pull the form variables
    var the_date                = Request.Form ("the_date_value");
    var the_id                  = Request.Form ("the_id_value");
    var the_method_id           = Request.Form ("method_selector");
    var the_method_name         = Request.Form ("the_method_value");
    var the_transaction_number  = parseInt (Request.Form ("the_transaction_number"));
    if (isNaN (the_transaction_number))
        the_transaction_number = "null";
    var the_type_id             = Request.Form ("type_selector");
    var the_type_name           = Request.Form ("the_type_value");
    var the_deduction_type_id   = Request.Form ("deduction_type_selector");
    var the_deduction_type_name = Request.Form ("the_tax_deduction_value");
    var the_from_name           = Request.Form ("the_from_value");
    var the_from_id             = Request.Form ("account_from_selector");
    var the_to_name             = Request.Form ("the_to_value");
    var the_to_id               = Request.Form ("account_to_selector");
    var the_amount              = Request.Form ("the_amount_value");
    var the_tax                 = Request.Form ("the_tax_value");
    var the_tip                 = Request.Form ("the_tip_value");
    var the_cleared_from        = (Request.Form ("the_cleared_from").Count > 0) ? 1 : 0;
    var the_cleared_to          = (Request.Form ("the_cleared_to").Count > 0) ? 1 : 0;
    var the_void                = (Request.Form ("the_void").Count > 0) ? 1 : 0;
    var the_escrow              = (Request.Form ("the_escrow").Count > 0) ? 1 : 0;
    var the_have_receipt        = (Request.Form ("the_have_receipt").Count > 0) ? 1 : 0;

    // XXX This could be a transaction entry - to ensure that all of the queries succeed
    
    // check to see if we need to create a method
    if ((the_method_name != "") && (the_method_id == 0))
        the_method_id = database.Create_Name ("Methods", the_method_name);
    database.Touch_Name ("Methods", the_date, the_method_id);
    
    // check to see if we need to create a type
    if ((the_type_name != "") && (the_type_id == 0))
        the_type_id = database.Create_Name ("Types", the_type_name);
    database.Touch_Name ("Types", the_date, the_type_id);
    
    // check to see if we need to create a tax deduction type
    if ((the_deduction_type_name != "") && (the_deduction_type_id == 0))
        the_deduction_type_id = database.Create_Name ("Deduction Types", the_deduction_type_name);
    database.Touch_Name ("Deduction Types", the_date, the_deduction_type_id);
    
    // check to see if we need to create an account
    if ((the_from_name != "") && (the_from_id == 0))
        the_from_id = database.Create_Name ("Accounts", the_from_name);
    if ((the_to_name != "") && (the_to_id == 0))
        the_to_id = database.Create_Name ("Accounts", the_to_name);
    database.Touch_Name ("Accounts", the_date, the_from_id);
    database.Touch_Name ("Accounts", the_date, the_to_id);
        
    // update the record in the database
    var sql = "UPDATE [Transactions] SET [Date] = CONVERT (smalldatetime, '" + the_date + "', 102), [Type] = " + the_type_id + 
        ", [Method] = " + the_method_id + 
        ", [Method ID] = " + the_transaction_number + 
        ", [Tax Deduction] = " + the_deduction_type_id + 
        ", [From Account] = " + the_from_id + 
        ", [To Account] = " + the_to_id + 
        ", [Amount] = " + the_amount + 
        ", [Tax] = " + the_tax + 
        ", [Tip] = " + the_tip + 
        ", [Cleared From] = " + the_cleared_from + 
        ", [Cleared To] = " + the_cleared_to + 
        ", [Void] = " + the_void + 
        ", [Escrow] = " + the_escrow + 
        ", [Have Receipt] = " + the_have_receipt + 
        " WHERE [ID] = " + the_id;
    database.Query (sql);
    
    // clear the edit transaction
    Session ("Edit_Transaction") = 0;
    Session ("Now") = new Date ();
}

//------------------------------------------------------------------------------
function Split_Transaction (database)
{
    if (Request.QueryString ("Split_Transaction").Count == 1)
    {
        var split_transaction = parseInt (Request.QueryString ("Split_Transaction"));
        var clear_amount = parseInt (Request.QueryString ("Clear_Amount"));
        
        // duplicate the transaction with a new tx id, and the clear amount, 
        // then set it to be clear
        
        // subtract the clear amount from the current transaction and update it
    }
}

//------------------------------------------------------------------------------
function Load_Package (database)
{
    // load the action code, but only if the session has been initialized
    if (Session ("Select_Account_Main") != undefined)
        if (Request.QueryString ("Action").Count == 1)
            g_action = Request.QueryString ("Action");
        
    // the action code is the name of the function we want to call now, so 
    // adding new actions is easy - these functions should store values into
    // the session so they can get loaded below
    eval (g_action + " (database);");
    
    // load all settings from the session
    Load_Session ();
}

//------------------------------------------------------------------------------
%>
