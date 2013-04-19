//------------------------------------------------------------------------------
// A Jane Data Object (JDO) is a base class for data sources. It supports 
// planned lazy population via RESTful interfaces, and sharing of the data. 
//
// object interface
//  - function: Subscribe (target, receiver, contract)
//  - function: SubscribeReadOnly (target, receiver)
//  - function: Unsubscribe (target)
//
//  - function: HasData ()
//  - function: GetData ()
//  - function: GetDataIsReadOnly ()
//  - function: Populate ()
//  - function: Flush ()
//  - function: Refresh ()
//
//  - function: HasMetaData ()
//  - function: GetMetaData ()
//  - function: AddFieldMetaData (fieldName, displayName, type)
//  - function: ValidateMetaData ()
//
//  - function: GetKey ()
//  - function: GetIndex (fieldName)
//  - function: GetRecord (primaryKeyValue)
//  - function: HighlightRecord (primaryKeyValue, highlight)
//
//  - property: name
//  - property: metaData - object with fieldName as keys, contains objects with
//                         fieldName, displayName, and type
//  - property: allowFlushForSubscription
//  - property: keyName - name of the field to use for building the key
//
//  - property: select - an array of fields to select, an empty selection is
//                       shorthand for all the fields
//  - property: sort - an array of objects that contain a name, and asc (boolean)
//  - property: filter - an object that implements the filter plugin interface
//  - property: transform - an object that implements the transform plugin interface
//
// The event model is extended to support contracts. A contract is a javascript 
// object with names of values the subcriber can modify. we assume that any 
// modification is expected to be private data that would cause a collision.
// 
// filter and transform plugins are javascript objects with a defined interface:
//  - property: name
//  - function: HandleRecord (record, readOnly)
//
// Cached data is an object with four properties:
//  - property: records
//  - property: readOnly
//  - property: key
//  - property: indexes
//
// The key is the hash of the keyName to array index object, it is built the
// first time it is used.
//
// Indexes are built on a lazy basis whenever they are requested, and then they
// are cached. Indexes are a simple array of objects containing the indexed 
// field and the array index of that record, then sorted on the indexed field 
// to facilitate fast binary searching and criteria matching (==, <>, <, and >).
//
// Highlight
// NOTE - we assume that all sources will use the "_HIGHLIGHTED_" key to 
// indicate the highlighted status of a record.
//
//------------------------------------------------------------------------------

Jane.DataObject = function (base) {

    var DataObject = Object.create(base, {
        "allowFlushForSubscription": {
            "value": false,
            "enumerable": true,
            "writable": true
        }
    });

    DataObject.Init = function (params) {
        // do the parental init, and then do my thing here
        base.Init.call(this, params);

        // start by creating empty metaData
        this.metaData = {
            "fields": {},
            "tags": {}
        };
        this.keyName = Jane.constants.__IDENTIFIER__;

        // copy some parameters
        // set up the default filter, transform, and sort options
        COPY_PARAM_NULL(filter, params);
        COPY_PARAM_NULL(select, params);
        COPY_PARAM_NULL(transform, params);
        COPY_PARAM_NULL(sort, params);

        COPY_PARAM(allowFlushForSubscription, params);

        // store this object in the global ref
        Jane.AddDataReference(this);

        return this;
    };

    DataObject.CanSubscribe = function (contract) {
        // check if the contract indicates the subscriber will modify the data
        if (! Jane.Utility.ObjectIsEmpty (contract)) {
            // the proposed contract is checked against all previously subscribed 
            // contracts to see if there is any conflict, return false if any is found
            for (var contractElement in contract) {
                if (contract.hasOwnProperty(contractElement)) {
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
            if (this.HasData() && this.cachedData.readOnly) {
                // we know there is no conflict with other contracts because we 
                // checked that above, but the data was already populated as a read-
                // only value. we *could* subscribe, but only if we flush first and 
                // repopulate with modifiable data - NOTE this is NOT the default 
                // behavior
                if (this.allowFlushForSubscription) {
                    this.Flush();
                } else {
                    return false;
                }
            }
        }

        // no conflict found
        return true;
    };

    DataObject.Subscribe = function (target, receiver, contract) {
        if (this.CanSubscribe(contract)) {
            var subscription = EventSource.Subscribe.call(this, target, receiver);
            if (subscription != null) {
                subscription["contract"] = contract;

                // if the data is already populated, send a populate event to this
                // receiver so it can join in the fun
                if (this.HasData()) {
                    receiver.apply(target, [Jane.events.DATA_POPULATED, this.cachedData]);
                }
            }
            return subscription;
        }
        return null;
    };

    DataObject.SubscribeReadOnly = function (target, receiver) {
        return this.Subscribe(target, receiver, {});
    };

    DataObject.HasData = function () {
        return ("cachedData" in this);
    };

    DataObject.GetData = function () {
        return (this.HasData()) ? this.cachedData.records : null;
    };

    DataObject.GetDataIsReadOnly = function () {
        // if the data is populated, return its real property
        if (this.HasData()) { return this.cachedData.readOnly; }

        // otherwise look at all the contracts to see if any of them will modify
        // the data
        for (var i = 0, count = this.subscriptions.length; i < count; ++i) {
            var subscription = this.subscriptions[i];
            if (! Jane.Utility.ObjectIsEmpty (subscription.contract)) {
                return false;
            }
        }

        // nobody says this will be modified, it's read only
        return true;
    };

    DataObject.Validate = function () {
        // check to see if we have meta data
        if (!this.HasMetaData()) {
            return false;
        }

        // get the meta data
        var metaData = this.GetMetaData();

        // check that the select fields are valid
        if (this.select != null) {
            for (var i = 0, count = this.select.length; i < count; ++i) {
                var fieldName = this.select[i];
                if (!(fieldName in metaData.fields)) {
                    DEBUGLOG(this.name + " - invalid selection field (" + fieldName + ")");
                    return false;
                }
            }
        }

        // check that the sort fields are valid
        if (this.sort != null) {
            for (var i = 0, count = this.sort.length; i < count; ++i) {
                var fieldName = this.sort[i].name;
                if (!(fieldName in metaData.fields)) {
                    DEBUGLOG(this.name + " - invalid sort field (" + fieldName + ")");
                    return false;
                }
            }
        }

        // everything is good, say so
        return true;
    };

    DataObject.CopyData = function (records, readOnly) {
        // this is where we implement filtering, transformation, and sorting
        // XXX TODO - implement a SQL-like language for the implementation?
        if (this.Validate()) {
            // loop over all the records
            var newRecords = [];
            for (var i = 0, count = records.length; i < count; ++i) {
                var record = records[i];
                // filter
                if ((this.filter == null) || (this.filter.HandleRecord(record))) {
                    // select
                    if (this.select == null) {
                        // if the result is read only, we'll just keep the one we have
                        // as that will be the lightest weight solution
                        if (!readOnly) {
                            record = Object.create(record);
                        }
                    } else {
                        // create a new object that contains the selected fields
                        var newRecord = {};
                        for (var j = 0, selectCount = this.select.length; j < selectCount; ++j) {
                            var fieldName = this.select[j];
                            newRecord[fieldName] = record[fieldName];
                        }
                        record = newRecord;
                    }

                    // transform
                    if (this.transform != null) {
                        record = this.transform.HandleRecord(record, readOnly);
                    }

                    // save it
                    newRecords.push(record);
                }
            }

            // sort
            if (this.sort != null) {
                var scope = this;
                var metaData = this.GetMetaData();
                var sortCount = this.sort.length;
                var sortFunc = function (a, b) {
                    // loop over all of the sort fields, checking that they are valid first
                    for (var i = 0, count = scope.sort.length; i < count; ++i) {
                        var sortField = scope.sort[i];
                        var sortResult = Jane.Utility.SortLexical(a[sortField.name], b[sortField.name], metaData.fields[sortField.name].type, sortField.asc);
                        if (sortResult != 0) {
                            return sortResult;
                        }
                    }

                    return 0;
                };
                newRecords.sort(sortFunc);
            }

            // the populate response
            return newRecords;
        }
        return null;
    };

    DataObject.PopulateDataResponse = function (records, event) {
        var readOnly = this.GetDataIsReadOnly();
        if ((this.sort != null) || (this.select != null) || (this.filter != null) || (this.transform != null) || (!readOnly)) {
            records = this.CopyData(records, readOnly);
        }
        if (records != null) {
            this.cachedData = { "records": records, "readOnly": readOnly, "key": null, "indexes": {} };
            this.PostEvent(event);
        }
    };

    DataObject.PopulateData = function () {
        // internal method to be overridden by descendant classes, this method will
        // only ever be called if the data source is not populated already - 
        // the result should call PopulateDataResponse with the new data
    };

    DataObject.Populate = function () {
        if (!this.HasData()) { this.PopulateData(); }
    };

    DataObject.Flush = function () {
        if (this.HasData()) {
            delete this.cachedData;
            this.PostEvent(Jane.events.DATA_FLUSHED);
        }
    };

    DataObject.Refresh = function () {
        this.Flush();
        this.Populate();
    };

    DataObject.HasMetaData = function () {
        return ! (Jane.Utility.ObjectIsEmpty(this.metaData.fields)); 
    };

    DataObject.GetMetaData = function () {
        return this.metaData;
    };

    DataObject.AddTagMetaData = function (tag, fieldName) {
        DEBUGLOG(this.name + " tagging " + fieldName + " (" + tag + ")");
        if (!(tag in this.metaData.tags)) {
            this.metaData.tags[tag] = [];
        }
        this.metaData.tags[tag].push(fieldName);
    };

    DataObject.AddFieldMetaData = function (fieldName, displayName, type, tags) {
        DEBUGLOG(this.name + " adding metaData for " + fieldName + " as " + type);
        var field = {
            "fieldName": fieldName,
            "displayName": displayName,
            "type": type
        };
        //this.metaData.fields[fieldName] = field;
        this.metaData.fields[displayName] = field;

        // tags is an array of values, we store the tags as their own keys, with 
        // references to the fields they are associated with
        for (var i = 0, count = tags.length; i < count; ++i) {
            //this.AddTagMetaData (tags[i], fieldName);
            this.AddTagMetaData(tags[i], displayName);
        }
    };

    DataObject.ValidateMetaData = function () {
        // need a primary key, otherwise create one
        var metaData = this.GetMetaData();
        if ("primary key" in metaData.tags) {
            this.keyName = metaData.tags["primary key"][0];
        } else {
            this.AddFieldMetaData(this.keyName, "Key", "integer", ["primary key"]);
        }
    };

    DataObject.GetKey = function () {
        var key = null;
        if (this.HasData()) {
            key = this.cachedData.key;
            if (key == null) {
                // need to build the key
                var fieldName = this.keyName;
                // check if the field is a valid one
                if (fieldName in this.metaData.fields) {
                    var records = this.cachedData.records;

                    // check if the field is not in the records, and add it
                    if (!(fieldName in records[0])) {
                        for (var i = 0, count = records.length; i < count; ++i) {
                            records[i][fieldName] = i;
                        }
                    }

                    // create the key
                    key = {};
                    for (var i = 0, count = records.length; i < count; ++i) {
                        var record = records[i];
                        key[record[fieldName]] = i;
                    }
                }
                this.cachedData.key = key;
            }
        }
        return key;
    };

    DataObject.GetIndex = function (fieldName) {
        var index = null;
        if (this.HasData()) {
            // check if we have already indexed this field
            if (fieldName in this.cachedData.indexes) {
                index = this.cachedData.indexes[fieldName];
            } else {
                // check if the field is a valid one
                if (fieldName in this.metaData.fields) {
                    var records = this.cachedData.records;

                    // check if the field is not in the records
                    if (!(fieldName in records[0])) {
                        // check to see if it's our primary key... if so, add it
                        if (fieldName == this.keyName) {
                            for (var i = 0, count = records.length; i < count; ++i) {
                                records[i][fieldName] = i;
                            }
                        }
                    }

                    // create the index
                    index = [];
                    for (var i = 0, count = records.length; i < count; ++i) {
                        var record = records[i];
                        index.push({ "value": record[fieldName], "index": i });
                    }

                    // sort the index
                    var metaDataFields = this.metaData.fields;
                    var sortFunc = function (a, b) {
                        return Jane.Utility.SortLexical(a.value, b.value, metaDataFields[fieldName].type, true);
                    };
                    index.sort(sortFunc);

                    // save the index
                    this.cachedData.indexes[fieldName] = index;
                }
            }
        }
        return index;
    };
    //  - function: GetRecord (primaryKeyValue)
    //  - function: HighlightRecord (primaryKeyValue, highlight)

    return DataObject;
}(EventSource);