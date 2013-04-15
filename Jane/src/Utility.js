//------------------------------------------------------------------------------
// Utility functions for Jane.
//
//------------------------------------------------------------------------------
Jane.Utility = Object.create (null);

Jane.Utility.SortLexical = function(a, b, type, asc) {
	// XXX this might need to be more sophisticated if a sort field is not a
	// XXX string or number (like... an object)
	switch (type) {
		case "integer" :
		case "double" :
		case "string" : {
			var na = Number(a);
			var nb = Number(b);
			if ((na == a.toString ()) && (nb == b.toString ())) {
				return asc ? (na - nb) : (nb - na);
			}
			return asc ? a.localeCompare (b) : b.localeCompare (a);
		} break;
		case "temporal" : {
			var da = new Date(a).valueOf ();
			var db = new Date(b).valueOf ();
			return asc ? (da - db) : (db - da);
		} break;
	};
	return 0;
};
