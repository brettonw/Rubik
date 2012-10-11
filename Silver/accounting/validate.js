<script type="text/javascript">
    //------------------------------------------------------------------------------
    function Validate_Form ()
    {
        // validate the form before submitting, then submit
        var error_string = document.getElementById ("ErrorMessage");

        // all the values to validate
        var the_method_id           = document.getElementById ("method_selector").value;
        var the_method_name         = document.getElementById ("the_method_value").value;
        var the_transaction_number  = document.getElementById ("the_transaction_number").value;
        var the_type_id             = document.getElementById ("type_selector").value;
        var the_type_name           = document.getElementById ("the_type_value").value;
        var the_from_id             = document.getElementById ("account_from_selector").value;
        var the_from_name           = document.getElementById ("the_from_value").value;
        var the_to_id               = document.getElementById ("account_to_selector").value;
        var the_to_name             = document.getElementById ("the_to_value").value;
        var the_amount              = document.getElementById ("the_amount_value").value;
        var the_tax                 = document.getElementById ("the_tax_value").value;
        
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
        
        // amount must be set
        if ((the_amount.length == 0) || (parseFloat (the_amount) <= 0))
        {
            error_string.innerHTML = "Enter the amount.";
            return false;
        }
        
        // tax must not be empty, make it 0 if it is
        if (the_tax == "")
            document.getElementById ("the_tax_value").value = "0";
        
        // XXX TODO enforce a state like attitude for cleared, void, and escrow

        return true;
    }

    //------------------------------------------------------------------------------
</script>
