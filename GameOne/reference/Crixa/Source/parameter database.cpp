//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/03/97
//
//  02/20/97	MK		Added DEBUG_SOFT statements to print error if parm not found
//	03/05/97	DS		Added termination if parm not found
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "parameter database.h"
#include "text buffer.h"
#include "debugger.h"

//------------------------------------------------------------------------------
ParmBase	*gParmBase;																														//	global parameter database

//------------------------------------------------------------------------------
class	ParmNode : public ListNode																								//	parameter list node class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				char				name[64];																										//	the name stored in the list
				char				value[64];																									//	the value associated with the name
				ParmNode (cstr name, cstr value);																				//	constructor
virtual	~ParmNode (void);																												//	destructor
};																																							//	end

//------------------------------------------------------------------------------
ParmNode::ParmNode (cstr nam, cstr val)																					//	constructor
{																																								//	begin
	strcpy (name, nam);																														//	copy the name string
	strcpy (value, val);																													//	copy the value string
}																																								//	end

//------------------------------------------------------------------------------
ParmNode::~ParmNode (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
ParmBase::ParmBase (void) : table (HashText, 128)																//	constructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
ParmBase::ParmBase (cstr database) : table (HashText, 128)											//	constructor
{																																								//	begin
	Load (database);																															//	load from the specified database
}																																								//	end

//------------------------------------------------------------------------------
ParmBase::~ParmBase (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	ParmBase::Load (cstr database)																						//	load from a specified database
{																																								//	begin
	TxtBuffer	buffer (database);																									//	the buffer for the database file
	if (buffer.Open ())																														//	if the buffer was successfully loaded
	{																																							//	begin
		cstr	line;																																	//	place to get the line
		for (int i = 0; i < 2; i++)																									//	do this twice
			do																																				//	loop
				buffer >> line;																													//	fetching a token
			while (strcmp (line, "#define"));																					//	until it is a #define statement
		while (strcmp (line, "#endif"))																							//	loop until we hit the #endif statement
		{																																						//	begin
			cstr			name,																														//	place to get a name
								value;																													//	place to get a value
			buffer >> name >> value >> line;																					//	fetch a name, value, and next line statement
			cstr			old = GetString (name);																					//	try to fetch the name from the database
			if (!old)																																	//	if the fetch was not successful
			{																																					//	begin
				ParmNode	*node = new ParmNode (name, value);														//	create a new parameter node
				List			*list = table.Fetch (name);																		//	get the list associated with the name
				list->InsertAfter (node);																								//	insert the parameter node into the list
			}																																					//	end
			else																																			//	otherwise, it was successful
				strcpy (old, value);																										//	so replace the value witht he new value
		}																																						//	end
		buffer.Close ();																														//	close the buffer
		return TRUE;																																//	return true to indicate that the file was loaded
	}																																							//	end
	return FALSE;																																	//	return false to indicate that the file was not loaded
}																																								//	end

//------------------------------------------------------------------------------
void	ParmBase::Dump (cstr database)																						//	dump the parameters to a specified database file
{																																								//	begin
	FILE	*fp = fopen (database, "w");																						//	attempt to open the file for writing
	if (fp)																																				//	if the open was successful
	{																																							//	begin
		fprintf (fp, "#ifndef\tPARAMETERS\n#define\tPARAMETERS\n\n");								//	print the header
		for (int i = 0; i < table.Size (); i++)																			//	loop over all of the lists in the table
		{																																						//	begin
			List	*list = table[i];																										//	get the ith list from the table
			ParmNode	*node = (ParmNode *) list->Head ();															//	get the head of the list
			while (node)																															//	loop over all of the nodes in the list
			{																																					//	begin
				int	alpha = FALSE;																											//	set the alphabetic flag to false, assume this is not a string
				for (int j = 0; node->value[j]; j++)																		//	loop over the length of the string
					if (isalpha (node->value[j]) || isspace (node->value[j]))							//	if the current character is a space or an alphabetic character
						if (node->value[j] != 'e')																					//	if the current character is not part of a numerical value
							alpha = TRUE;																											//	set the alphabetic flag
				if (alpha)																															//	if the value appears to be a string
					fprintf (fp, "#define\t%s\t\"%s\"\n", node->name, node->value);				//	dump the node name and stringized value to the file in database format
				else																																		//	otherwise
					fprintf (fp, "#define\t%s\t%s\n", node->name, node->value);						//	dump the node name and value to the file in database format
				node = (ParmNode *) list->Next (node);																	//	move on to the next name
			}																																					//	end
		}																																						//	end
		fprintf (fp, "\n#endif\t//\tPARAMETERS\n");																	//	print the footer
		fclose (fp);																																//	close the file
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
cstr	ParmBase::GetString (const char *name)																		//	get a string parameter
{																																								//  begin
	List	*list = table.Fetch (name);																							//	get the list associated with the name
	ParmNode	*node = (ParmNode *) list->Head ();																	//	get the head of the list
	while (node)																																	//	loop over all of the nodes in the list
	{																																							//	begin
		if (strcmp (name, node->name) == 0)																					//	if this is the name we are searching for
			return node->value;																												//	return the value
		node = (ParmNode *) list->Next (node);																			//	move on to the next name
	}																																							//	end
	return 0;
}																																								//	end

//------------------------------------------------------------------------------
Real	ParmBase::GetReal (const char *name)																			//	get a real valued parameter
{																																								//	begin
	cstr	string = GetString (name);																							//	get the string from the table
	if (string)																																		//	if the string is valid
		return Real (atof (GetString (name)));																			//	get the value string and convert it to a float
	DEBUG_SOFT ("*** ParmBase: Missing Parameter %s.\n",name);
	return ZERO;
}																																								//	end

//------------------------------------------------------------------------------
int		ParmBase::GetInteger (const char *name)																		//	get an integer valued parameter
{																																								//	begin
	cstr	string = GetString (name);																							//	get the string from the table
	if (string)																																		//	if the string is valid
		return atoi (GetString (name));																							//	get the value string and convert it to an integer
	DEBUG_SOFT ("*** ParmBase: Missing Parameter %s.\n",name);
	return 0;
}																																								//	end

//------------------------------------------------------------------------------
cstr	ParmBase::Assign (const char *name, const char *val)											//	assign a value to a name
{																																								//	begin
	cstr value = GetString (name);
	if (value)																																		//	if the value was successfully retrieved from the database
		strcpy (value, val);																												//	copy the new value string into the field
	return value;																																	//	return the string pointer to the value
}																																								//	end

//------------------------------------------------------------------------------
