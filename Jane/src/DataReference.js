//------------------------------------------------------------------------------
// A Jane Data Reference (JDR) is a base class for data sources. It supports
// lazy population via RESTful interfaces, and sharing of the data. This base 
// class implements a contract-based subscription event model.
//
// object interface
//  - function: Subscribe (target, receiver, contract)
//  - function: SubscribeReadOnly (target, receiver)
//  - function: Unsubscribe (target)
//
//  - function: HasData ()
//  - function: GetData ()
//  - function: GetDataIsReadOnly ()
//  - function: GetDataFormat ();
//  - function: Populate ()
//  - function: Flush ()
//  - function: Refresh ()
//
//  - function: HasMetaData ()
//  - function: GetMetaData ()
//  - function: AddFieldMetaData (fieldName, displayName, type)
//
//  - property: name
//  - property: metaData - object with fieldName as keys, contains objects with
//                         fieldName, displayName, and type
//  - property: allowFlushForSubscription
//  - property: primaryKey - name of the field to use for indexing
//
// global interface
//  + property: events
//      - DATA_POPULATED
//      - DATA_FLUSHED
//      - DATA_CHANGED
//
// Event subscribers are expected to have a "name" property.  
//  - property: name
//
// Events are posted to the subscriber's receiver, which is expected to have a 
// signature resembling this:
//  - function: ReceiveEvent (sender, event)
//
// A contract is a javascript object with names of values the subcriber can 
// modify. we assume that any modification is expected to be private data that
// would cause a collision.
// 
// NOTE - we assume that all sources will use the "_HIGHLIGHTED_" key to 
// indicate the highlighted status of a record.
//
// Cached data is an object with three properties:
//  - property: data
//  - property: readOnly
//  - property: format
//
// Indexes are built on fields tagged as "indexed", and "primary key" in the 
// meta data. Indexes are a simple array of objects containing the indexed field 
// and the array index of that record, then sorted on the indexed field to 
// facilitate fast binary searching and criteria matching (==, <>, <, and >).
// They are stored in the cached data segment.
//  
//------------------------------------------------------------------------------

Jane.DataReference = Object.create(EventSource);

Jane.DataReference.allowFlushForSubscription = false;

Jane.DataReference.Init = function(params) {
    // do the parental init, and then do my thing here
    EventSource.Init.call(this, params);

    // start by creating empty metaData
    this.metaData = {
        "fields" : {},
        "tags" : {}
    };
    this.primaryKey = Jane.constants.__IDENTIFIER__;
    
    // copy some parameters
    COPY_PARAM(allowFlushForSubscription, params);

    // store this object in the global ref
    Jane.AddDataReference (this);
    return this;
};

Jane.DataReference.CanSubscribe = function (contract) {
    // check if the contract indicates the subscriber will modify the data
    if (Object.getOwnPropertyNames (contract).length > 0) {
        // the proposed contract is checked against all previously subscribed 
        // contracts to see if there is any conflict, return false if any is found
        for (var contractElement in contract) {
            if (contract.hasOwnProperty (contractElement)) {
                for (var i = 0, count = this.subscriptions.length; i < count; ++i) {
                    var subscription = this.subscriptions[i];
                    if (contractElement in subscription.contract) {
                        return false;
                    }
                }
            }
        }

        // if the data has been populated, we can check to see if there is a 
        // conflict between the readOnly property and the contract
        if (this.HasData () AND this.chachedData.readOnly) {
            // we know there is no conflict with other contracts because we 
            // checked that above, but the data was already populated as a read-
            // only value. we *could* subscribe, but only if we flush first and 
            // repopulate with modifiable data - NOTE this is NOT the default 
            // behavior
            if (this.allowFlushForSubscription) {
                this.Flush ();
            } else {
                return false;
            }
        }
    }

    // no conflict found
    return true;
};

Jane.DataReference.Subscribe = function(target, receiver, contract) {
    if (this.CanSubscribe (contract)) {
        var subscription = EventSource.Subscribe.call (this, target, receiver);
        if (subscription != null) {
            subscription["contract"] = contract;

            // if the data is already populated, send a populate event to this
            // receiver so it can join in the fun
            if (this.HasData ()) {
                receiver.apply (target, [Jane.events.DATA_POPULATED, this.cachedData]);
            }
        }
        return subscription;
    }
    return null;
};

Jane.DataReference.SubscribeReadOnly = function(target, receiver) {
    return this.Subscribe (target, receiver, {});
};

Jane.DataReference.HasData = function () {
    return ("cachedData" in this);
};

Jane.DataReference.GetData = function () {
    return (this.HasData ()) ? this.cachedData.records : null;
};

Jane.DataReference.GetDataIsReadOnly = function() {
    // if the data is populated, return its real property
    if (this.HasData ()) { return this.cachedData.readOnly; }

    // otherwise look at all the contracts to see if any of them will modify
    // the data
    for (var i = 0, count = this.subscriptions.length; i < count; ++i) {
        var subscription = this.subscriptions[i];
        if (Object.getOwnPropertyNames (subscription.contract).length > 0) {
            return false;
        }
    }

    // nobody says this will be modified, it's read only
    return true;
};

Jane.DataReference.GetDataFormat = function() {
    // if the data is populated, return its real property
    if (this.HasData ()) { return this.cachedData.format; }

    // otherwise it's empty
    return Jane.formats.EMPTY;
};

Jane.DataReference.BuildIndex = function (fieldName) {
	if (NOT (fieldName in this.cachedData.indexes)) {
		var index = [];
		
		// create the index
		var records = this.cachedData.records;
		for (var i = 0, count = records.length; i < count; ++i) {
			var record = records[i];
			index.push ({ "value" : record[fieldName], "index" : i});
		}
		
		// sort the index
		var sortFunc = function (a, b) {
			return Jane.Utility.SortLexical(a.value, b.value, metaData.fields[fieldName].type, true);
		};
		index.sort (sortFunc);

		// save the index
		this.cachedData.indexes[fieldName] = index;
	}
};

Jane.DataReference.ValidatePrimaryKey = function () {
	var primaryKey = this.primaryKey;
	var records = this.cachedData.records;
	if (NOT (primaryKey in records[0])) {
		// walk over the entire recordset to add the primary key
		var primaryKey = this.primaryKey;
		for (var i = 0, count = records.length; i < count; ++i) {
			records[i][primaryKey] = i;
		}
	} 
};

Jane.DataReference.PopulateDataResponse = function (records, readOnly, format, event) {
    // whatever mechanism populates the data is descendant classes, it should
    // call this method. 
    this.cachedData = { "records" : records, "readOnly" : readOnly, "format" : format, "indexes" : {} };
    this.ValidatePrimaryKey ();
    
    // build the indices, primary key first
	this.BuildIndex (this.primaryKey);
	var indexedFields = this.metaData.tags["indexed"];
    for (var i = 0, count = indexedFields.length; i < count; ++i) {
		this.BuildIndex (indexedFields[i]);
    }
    this.PostEvent (event);
};

Jane.DataReference.PopulateData = function () {
    // internal method to be overridden by descendant classes, this method will
    // only ever be called if the data source is not populated already
};

Jane.DataReference.Populate = function () {
    if (NOT this.HasData ()) { this.PopulateData (); }
};

Jane.DataReference.Flush = function () {
    if (this.HasData ()) { 
        delete this.cachedData;
        this.PostEvent (Jane.events.DATA_FLUSHED);
    }
};

Jane.DataReference.Refresh = function () {
    this.Flush ();
    this.Populate ();
};

Jane.DataReference.HasMetaData = function () {
    return (Object.getOwnPropertyNames(this.metaData.fields).length > 0);
};

Jane.DataReference.GetMetaData = function () {
    return this.metaData;
};

Jane.DataReference.AddTagMetaData = function (tag, fieldName) {
	DEBUGLOG (this.name + " tagging " + fieldName + " (" + tag + ")");
	if (NOT (tag in this.metaData.tags)) {
		this.metaData.tags[tag] = [];
	}
	this.metaData.tags[tag].push (field);
};

Jane.DataReference.AddFieldMetaData = function (fieldName, displayName, type, tags) {
    DEBUGLOG (this.name + " adding metaData for " + fieldName + " as " + type);
    var field = {
        "fieldName" : fieldName,
        "displayName" : displayName,
        "type" : type
    };
    this.metaData.fields[fieldName] = field;

    // tags is an array of values, we store the tags as their own keys, with 
    // references to the fields they are associated with
    for (var i = 0, count = tags.length; i < count; ++i) {
        this.AddTagMetaData (tags[i], fieldName);
    }
};

Jane.DataReference.ValidateMetaData = function () {
    // need a primary key, otherwise create one
    var metaData = this.GetMetaData ();
	if ("primary key" in metaData.tags) {
		this.primaryKey = metaData.tags["primary key"][0];
	} else {
        this.AddFieldMetaData (this.primaryKey, "Key", "integer", ["primary key"]);
	}
};

