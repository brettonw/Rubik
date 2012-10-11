<div id="context_menu_div" onclick="Click_Context_Menu ();" onmouseover="Switch_Context_Menu ();" onmouseout="Switch_Context_Menu ();" class="context_menu"></div>
<script type="text/javascript">
    // oncontextmenu="Display_Context_Menu (); return false;"
    //------------------------------------------------------------------------------
    // global variables
    //------------------------------------------------------------------------------
    var g_context_on_transaction_id;
    
    //------------------------------------------------------------------------------
    // functions
    //------------------------------------------------------------------------------
    function Display_Context_Menu (transaction_id, tx_pending, tx_escrow, tx_void) 
    {
        g_context_on_transaction_id = transaction_id;
        
        // build the menu text
        var menu_content = "<center>\n<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"50\">\n";
        if (tx_void || tx_escrow || (!tx_pending))
            menu_content += "<tr><td><div class=\"menu_item\" id=\"menuPending\">Pending</div></td></tr>\n";
        if (tx_pending)
            menu_content += "<tr><td><div class=\"menu_item\" id=\"menuClear\">Clear</div></td></tr>\n";
        if (! tx_escrow)
            menu_content += "<tr><td><div class=\"menu_item\" id=\"menuEscrow\">Escrow</div></td></tr>\n";
        if (! tx_void)
            menu_content += "<tr><td><div class=\"menu_item\" id=\"menuVoid\">Void</div></td></tr>\n";
        menu_content += "<tr><td class=\"menu_background\" height=\"3\"></td></tr>\n";
        menu_content += "<tr><td height=\"1\" class=\"menu_border\"></td></tr>\n";
        menu_content += "<tr><td class=\"menu_background\" height=\"3\"></td></tr>\n";
        if (tx_pending)
            menu_content += "<tr><td><div class=\"menu_item\" id=\"menuSplit\">Split</div></td></tr>\n";
        menu_content += "<tr><td><div class=\"menu_item\" id=\"menuEdit\">Edit</div></td></tr>\n";
        menu_content += "</table>\n";
        context_menu_div.innerHTML = menu_content;
        
        // popup the menu
        context_menu_div.style.leftPos += 10;
        context_menu_div.style.posLeft = event.clientX;
        context_menu_div.style.posTop = event.clientY;
        context_menu_div.style.visibility = "visible";
        context_menu_div.setCapture (true);
    }

    //------------------------------------------------------------------------------
    function Switch_Context_Menu () 
    {   
        var el = event.srcElement;
        if (el.className == "menu_item") 
            el.className = "menu_highlight";
        else if (el.className == "menu_highlight") 
            el.className = "menu_item";
    }

    //------------------------------------------------------------------------------
    function Click_Context_Menu () 
    {
        // XXX need the transaction menu
        
        context_menu_div.releaseCapture();
        context_menu_div.style.visibility = "hidden";
        var el=event.srcElement;
        if (el.id=="menuClear") 
            Clear_Transaction (g_context_on_transaction_id);
        if (el.id=="menuPending") 
            Pending_Transaction (g_context_on_transaction_id);
        else if (el.id=="menuEscrow") 
            Escrow_Transaction (g_context_on_transaction_id);
        else if (el.id=="menuVoid") 
            Void_Transaction (g_context_on_transaction_id);
        else if (el.id=="menuEdit") 
            Edit_Transaction (g_context_on_transaction_id);
        else if (el.id=="menuSplit") 
            Split_Transaction (g_context_on_transaction_id);
    }

    //------------------------------------------------------------------------------
</script>

