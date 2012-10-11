<%
	//--------------------------------------------------------------------------
	function SQL_Database_Close (query)
	{
		this.connection.Close ();
	}

	//--------------------------------------------------------------------------
	function SQL_Database_Query (query)
	{
		return this.connection.Execute (query);
	}

	//--------------------------------------------------------------------------
	function SQL_Database_Get_Next_ID (table_name)
	{
	    var sql = "SELECT [Next ID] FROM [Next] WHERE [Table Name] Like \'" + table_name + "\'";
		var	record_set = this.connection.Execute (sql);
		var id = 0;
		if (! record_set.EOF)
		    id = new Number (record_set.Fields.Item (0));
		record_set.Close ();
		return id;
	}
	
	//--------------------------------------------------------------------------
	function SQL_Database_Set_Next_ID (table_name, id)
	{
	    var sql = "UPDATE [Next] SET [Next ID] = " + id + " WHERE [Table Name] Like \'" + table_name + "\'";
		this.connection.Execute (sql);
	}
	
	//--------------------------------------------------------------------------
	function SQL_Database_Create_Next_ID (table_name)
	{
		
		// create and return the next id assigned (always 1)
	    var sql = "INSERT INTO [Next] ([Table Name], [Next ID]) VALUES (\'" + table_name + "\', 1)";
		this.connection.Execute (sql);
		return 1;
	}
	
	//--------------------------------------------------------------------------
	function SQL_Database_Query_Float (sql)
	{
	    var value = 0.0;
        var record_set = database.Query (sql);
        if (! record_set.EOF)
        {
            var re_null = /^null/i;
            if (re_null.exec (record_set.Fields (0)) == null)
                value = parseFloat (record_set.Fields (0));
        }
        record_set.Close ();
        return value;
	}
	
	//--------------------------------------------------------------------------
	function SQL_Database_Create_Name (table, _name)
	{
	    var name = new String (_name);
        // replace single quotes with the escape version of the character
        var regexp = /\'/g;
        name = name.replace (regexp, "&#39");

        // fetch the id the new record will have, and update the new id
        var id = this.Get_Next_ID (table);
        if (id == 0)
            id = this.Create_Next_ID (table);
        this.Set_Next_ID (table, id + 1);

        // build the query and execute it
        var sql = "INSERT INTO [" + table + "] ([ID], [Name]) VALUES (" + id + ", \'" + name + "\')";
		this.connection.Execute (sql);

        // return the found ID
        return id;
	}

	//--------------------------------------------------------------------------
    function SQL_Database_Touch_Name (table, date_string, id)
    {
        sql = "UPDATE [" + table + "] SET [Last Update] = CONVERT (smalldatetime, '" + date_string + "', 102) WHERE [ID] = " + id;
        database.Query (sql);
    }

	//--------------------------------------------------------------------------
	// SQL_Database constructor
	//--------------------------------------------------------------------------
	function SQL_Database ()
	{
        this.connection = Server.CreateObject ('ADODB.Connection');
        this.connection.Open (Application ("Connection String"));

		// set the methods of the clase
		this.Close = SQL_Database_Close;
		this.Query = SQL_Database_Query;
		this.Get_Next_ID = SQL_Database_Get_Next_ID;
		this.Set_Next_ID = SQL_Database_Set_Next_ID;
		this.Create_Next_ID = SQL_Database_Create_Next_ID;
		this.Query_Float = SQL_Database_Query_Float;
		this.Create_Name = SQL_Database_Create_Name;
		this.Touch_Name = SQL_Database_Touch_Name;
	}

	//--------------------------------------------------------------------------
%>