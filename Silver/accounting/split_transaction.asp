<div id="split_transaction_div" class="popup_dialog"></div>
<script type="text/javascript">
    //------------------------------------------------------------------------------
    // global variables
    //------------------------------------------------------------------------------
    var g_split_on_transaction_id;
    
    //------------------------------------------------------------------------------
    // functions
    //------------------------------------------------------------------------------
    function Display_Split_Popup (transaction_id) 
    {
        // popup the menu
        split_transaction_div.style.leftPos += 10;
        split_transaction_div.style.posLeft = event.clientX;
        split_transaction_div.style.posTop = event.clientY;
        split_transaction_div.style.visibility = "visible";
        //split_transaction_div.setCapture (true);
    }

    //------------------------------------------------------------------------------
</script>

