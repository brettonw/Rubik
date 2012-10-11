<!-- #include virtual="/server/javascript/Server_Tools.html" -->
<!-- #include virtual="/server/javascript/XML_Query.html" -->

<script language="javascript" runat="server">
	//--------------------------------------------------------------------------
	// why, I'm a little black rain cloud, of course

	// do the query and get the results in XML format
	var xml = XML_Connect_And_Query (Request.QueryString ("Catalog"), Request.QueryString ("Query"));

	// set the format
	Response.ContentType = "text/XML";

	// print the response
	Print ("<?xml version=\"1.0\" ?>\n" + xml);

	//--------------------------------------------------------------------------
</script>
