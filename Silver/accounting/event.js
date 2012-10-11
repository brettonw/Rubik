<script type="text/javascript">
    //------------------------------------------------------------------------------
    function Handle_Selector_Event (selector_name, msg)
    {
        var selector = document.getElementById (selector_name);
        var url = "main.asp?Action=" + msg + "&" + msg + "=" + selector.value;
        self.location = url;
    }
    
    //------------------------------------------------------------------------------
    function Select_Account_Main ()
    {
        // clear the scroll cookie
        document.cookie = "scroll_pos=-1";   
        
        Handle_Selector_Event ("account_main_selector", "Select_Account_Main");
    }
    
    //------------------------------------------------------------------------------
    function Select_Month ()
    {
        // clear the scroll cookie
        document.cookie = "scroll_pos=-1";   
        
        Handle_Selector_Event ("month_selector", "Select_Month");
    }
    
    //------------------------------------------------------------------------------
    function Select_Age ()
    {
        // clear the scroll cookie
        document.cookie = "scroll_pos=-1";   
        
        Handle_Selector_Event ("age_selector", "Select_Age");
    }
    
    //------------------------------------------------------------------------------
    function Clear_Transaction (transaction_id)
    {
        var url = "main.asp?Action=Clear_Transaction&Clear_Transaction=" + transaction_id;
        self.location = url;
    }

    //------------------------------------------------------------------------------
    function Pending_Transaction (transaction_id)
    {
        var url = "main.asp?Action=Pending_Transaction&Pending_Transaction=" + transaction_id;
        self.location = url;
    }

    //------------------------------------------------------------------------------
    function Escrow_Transaction (transaction_id)
    {
        var url = "main.asp?Action=Escrow_Transaction&Escrow_Transaction=" + transaction_id;
        self.location = url;
    }

    //------------------------------------------------------------------------------
    function Void_Transaction (transaction_id)
    {
        var url = "main.asp?Action=Void_Transaction&Void_Transaction=" + transaction_id;
        self.location = url;
    }

    //------------------------------------------------------------------------------
    function Edit_Transaction (transaction_id)
    {
        var url = "main.asp?Action=Edit_Transaction&Edit_Transaction=" + transaction_id;
        self.location = url;
    }
    
    //------------------------------------------------------------------------------
    function Split_Transaction (transaction_id)
    {
        var url = "main.asp?Action=Split_Transaction&Split_Transaction=" + transaction_id;
        self.location = url;
    }
    
    //------------------------------------------------------------------------------
    function Show_Hide_Cleared_Transactions ()
    {
        // clear the scroll cookie
        document.cookie = "scroll_pos=-1";   
        
        var checkbox = document.getElementById ("Show_Hide_Cleared_Transactions");
        var show = 0;
        if (checkbox.checked)   // this is the state after the click
            show = 1;
        var url = "main.asp?Action=Show_Hide_Cleared_Transactions&Show_Hide_Cleared_Transactions=" + show;
        self.location = url;
    }
    
    //------------------------------------------------------------------------------
    function Show_Preferred_Accounts ()
    {
        var checkbox = document.getElementById ("Show_Preferred_Accounts");
        var show = 0;
        if (checkbox.checked)   // this is the state after the click
            show = 1;
        var url = "main.asp?Action=Show_Preferred_Accounts&Show_Preferred_Accounts=" + show;
        self.location = url;
    }
    
    //------------------------------------------------------------------------------
    function Select_Method ()
    {
        var focus_element;
        var selector = document.getElementById ("method_selector");
        if (selector.value == 5)    // check
            focus_element = document.getElementById ("the_transaction_number");
        else
            focus_element = document.getElementById ("type_selector");
        focus_element.focus ();
    }
    
    //------------------------------------------------------------------------------
    function Select_Type ()
    {
        document.getElementById ("deduction_type_selector").focus ();
    }
    
    //------------------------------------------------------------------------------
    function Select_Deduction_Type ()
    {
        document.getElementById ("account_from_selector").focus ();
    }
    
    //------------------------------------------------------------------------------
    function Select_Account_From ()
    {
        document.getElementById ("account_to_selector").focus ();
    }
    
    //------------------------------------------------------------------------------
    function Select_Account_To () 
    {
        document.getElementById ("the_amount_value").focus ();
        document.getElementById ("the_amount_value").select ();
    }
    
    //------------------------------------------------------------------------------
    function Select_Date (year, month, day)
    {
        now.setYear (year);
        now.setMonth (month);
        now.setDate (day);
        document.getElementById ("the_calendar_frame").src = "Calendar.asp?Year=" + now.getFullYear () +"&Month=" + (now.getMonth () + 1) + "&Day=" + now.getDate () + "&Highlight_Date=1";
        
        var mz = new String ();
        var dz = new String ();
        if ((month + 1) < 10)
            mz = "0";
        if (day < 10)
            dz = "0";
        document.getElementById ("the_date_value").value = " " + year + "." + mz + (month + 1) + "." + dz + day;

        --month;
        if (month < 0)
        {
            month += 12;
            --year;
        }
        var then = new Date ();
        then.setYear (year);
        then.setMonth (month);
        then.setDate (day);
        document.getElementById ("the_calendar_frame_last").src = "Calendar.asp?Year=" + then.getFullYear () +"&Month=" + (then.getMonth () + 1) + "&Day=" + then.getDate () + "&Highlight_Date=0";
        
        document.getElementById ("method_selector").focus ();
    }
    
    //------------------------------------------------------------------------------
    function CalculateTaxFromTotal (amount)
    {
        var tip = parseFloat (document.getElementById ("the_tip_value").value);
        var total = parseFloat (document.getElementById ("the_amount_value").value);
        var taxable_total = total - tip;
        var tax = Math.ceil ((taxable_total - (taxable_total / (1.0 + amount))) * 100) / 100;
        document.getElementById ("the_tax_value").value = tax;
    }
    
    //------------------------------------------------------------------------------
    var now = new Date ();
    Select_Date (<%=g_now.getFullYear () %>, <%=g_now.getMonth () %>, <%=g_now.getDate () %>);   
    
    //------------------------------------------------------------------------------
</script>
