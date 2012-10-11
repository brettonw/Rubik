<html>
<head>
    <title>Transaction Loader</title>
    <link type="text/css" rel="stylesheet" href="style_tx.css">
</head>
<body>
<center>
<b>Transactions for <%=g_account_name %></b>
<table border="0" cellpadding="0" cellspacing="0">
<tr><td align="center"><div id="ErrorMessage" class="error"></div></td></tr>
<tr><td><hr /></td></tr>
<%=lines %>
<tr><td><hr /></td></tr>
<tr><td><table width="100%"><tr><td align="left"><%=g_prev_page_link %></td><td align="right"><%=g_next_page_link %></tr></table></td></tr>
<tr><td><hr /></td></tr>
</table>
</center>
</body>
</html>

<script type="text/javascript">
    function Calculate_Tax (theForm, taxAmount)
    {
        var tip = parseFloat (theForm.elements["the_tip_value"].value);
        var total = parseFloat (theForm.elements["the_amount_value"].value);
        var taxable_total = total - tip;
        var tax = Math.ceil ((taxable_total - (taxable_total / (1.0 + taxAmount))) * 100) / 100;
        theForm.elements["the_tax_value"].value = tax;
    }

    function Calculate_Tip (theForm, tipAmount)
    {
        var total = parseFloat (theForm.elements["the_amount_value"].value);
        var tip = Math.ceil (Math.round (total) * (tipAmount * 10.0)) / 10.0;
        theForm.elements["the_tip_value"].value = tip;
    }

    function Validate_Form (theForm)
    {
        // validate the form before submitting, then submit
        var error_string = document.getElementById ("ErrorMessage");

        // all the values to validate
        var the_method_id           = theForm.elements["method_selector"].value;
        var the_method_name         = theForm.elements["the_method_value"].value;
        var the_transaction_number  = theForm.elements["the_transaction_number"].value;
        var the_type_id             = theForm.elements["type_selector"].value;
        var the_type_name           = theForm.elements["the_type_value"].value;
        var the_from_id             = theForm.elements["account_from_selector"].value;
        var the_from_name           = theForm.elements["the_from_value"].value;
        var the_to_id               = theForm.elements["account_to_selector"].value;
        var the_to_name             = theForm.elements["the_to_value"].value;
        var the_amount              = theForm.elements["the_amount_value"].value;
        var the_tip                 = theForm.elements["the_tip_value"].value;
        var the_tax                 = theForm.elements["the_tax_value"].value;
        
        // method_id must be set
        if ((the_method_id == 0) && (the_method_name == ""))
        {
            error_string.innerHTML = "Select the Method from the list.";
            return false;
        }
        else if (parseInt (the_method_id) == 5)
        {
            if (the_transaction_number == "")
            {
                error_string.innerHTML = "Enter the check number.";
                return false;
            }
        }
    
        // type_id must be set
        if ((the_type_id == 0) && (the_type_name == ""))
        {
            error_string.innerHTML = "Select the Type from the list.";
            return false;
        }

        // either from name or from_id must be set
        if ((the_from_id == 0) && (the_from_name == ""))
        {
            error_string.innerHTML = "Select the \"From\" account from the list.";
            return false;
        }
        
        // either to name or to_id must be set
        if ((the_to_id == 0) && (the_to_name == ""))
        {
            error_string.innerHTML = "Select the \"To\" account from the list.";
            return false;
        }
        
        // tax and tip must not be empty, make it 0 if it is
        if (the_tip == "")
           theForm.elements["the_tip_value"].value = "0";
        if (the_tax == "")
           theForm.elements["the_tax_value"].value = "0";

        //theForm.elements["the_submit_button"].enabled = false;
        return true;
    }
</script>