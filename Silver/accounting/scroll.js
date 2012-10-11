<script type="text/javascript">
    //------------------------------------------------------------------------------
    function On_Transaction_Scroll ()
    {
        var scroll_pos = event.srcElement.scrollTop;   
        //document.getElementById ("ErrorMessage").innerHTML = "(" + scroll_pos + ")";
        document.cookie = "scroll_pos=" + scroll_pos;   
    }
    
    //------------------------------------------------------------------------------
    function GetCookie (name)
    {
        // cookies are separated by semicolons
        var cookie = document.cookie.split("; ");
        for (var i=0; i < cookie.length; i++)
        {
            // a name/value pair (a crumb) is separated by an equal sign
            var crumb = cookie[i].split ("=");
            if (name == crumb[0]) 
                return unescape (crumb[1]);
        }

        // a cookie with the requested name does not exist
        return null;
    }
    
    //------------------------------------------------------------------------------
    var g_tx_list_div = document.getElementById ("transaction_list_div");
    g_tx_list_div.scrollTop = g_tx_list_div.scrollHeight;
    var g_scroll_pos = GetCookie ("scroll_pos");
    if ((g_scroll_pos != null) && (g_scroll_pos >= 0))
    {   
        //document.getElementById ("ErrorMessage").innerHTML = "(" + g_scroll_pos + ")";
        g_tx_list_div.scrollTop = g_scroll_pos;
    }

    //------------------------------------------------------------------------------
</script>
