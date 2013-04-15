//------------------------------------------------------------------------------
// A Jane Data Reference (JDR) Copy is an abstract, lightweight JDR that 
// describes a data source as a filter, sort, and/or transformation of one or
// more JDRs.
//
// object interface
//  - property: select - an array of fields to select, an empty selection is
//                       shorthand for all the fields
//  - property: sort - an array of objects that contain a name, and asc (boolean)
//  - property: filter - an object that implements the filter plugin interface
//  - property: transform - an object that implements the transform plugin interface
//
//  - function: Orphan - disconnect a populated data reference from its source
//                       data references
//
// filter and transform plugins are javascript objects with a defined interface:
//  - property: name
//  - function: HandleRecord (record)
//  
// we assume that all record formats implement the bracket notation to retrieve 
// fields within the record
//
//------------------------------------------------------------------------------

Jane.DataReferenceCopy = Object.create(Jane.DataReference);

Jane.DataReferenceCopy.Init = function (params) {
    // do the parental init, and then do my thing here
    Jane.DataReference.Init.call(this, params);

    // set up the default filter, transform, and sort options to be blank
    this.filter = null;
    this.select = null;
    this.transform = null;
    this.sort = null;

    // copy the sources
    COPY_PARAM(source, params);
    this.source.SubscribeReadOnly (this, this.ReceiveEvent);

    return this;
};

Jane.DataReferenceCopy.Validate = function () {
    // check to see if we have meta data
    if (NOT this.HasMetaData ()) {
        return false;
    }

    // get the meta data
    var metaData = this.GetMetaData ();

    // check that the select fields are valid
    if (this.select != null) {
        for (var i = 0, count = this.select.length; i < count; ++i) {
            var fieldName = this.select[i];
            if (NOT (fieldName in metaData.fields)) {
                DEBUGLOG (this.name + " - invalid selection field (" + fieldName + ")");
                return false;
            }
        }
    }

    // check that the sort fields are valid
    if (this.sort != null) {
        for (var i = 0, count = this.sort.length; i < count; ++i) {
            var fieldName = this.sort[i].name;
            if (NOT (fieldName in metaData.fields)) {
                DEBUGLOG (this.name + " - invalid sort field (" + fieldName + ")");
                return false;
            }
        }
    }

    // everything is good, say so
    return true;
};

Jane.DataReferenceCopy.CopyData = function (records, event) {
    // this is where we implement filtering, transformation, and sorting
    // XXX TODO - implement a SQL-like language for the implementation?
    if (this.Validate ()) {
        var format = this.GetDataFormat();
        var readOnly = this.GetDataIsReadOnly();

        // loop over all the records
        var newRecords = [];
        for (var i = 0, count = records.length; i < count; ++i) {
            var record = records[i];
            // filter
            if ((this.filter == null) OR (this.filter.HandleRecord (record))) {
                // select
                if (this.select == null) {
                    // if the result is read only, we'll just keep the one we have
                    // as that will be the lightest weight solution
                    if (NOT readOnly) {
                        // otherwise, we'll make it into an OBJECT_AS_PROTOTYPE
                        record = Object.create (record);
                        format = Jane.formats.OBJECT_AS_PROTOTYPE;
                    }
                } else {
                    // create a new object that contains the selected fields
                    var newRecord = {};
                    for (var j = 0, selectCount =  this.select.length; j < selectCount; ++j) {
                        var fieldName = this.select[j];
                        newRecord[fieldName] = record[fieldName];
                    }
                    record = newRecord;
                    format = Jane.formats.OBJECT;
                }

                // transform
                if (this.transform != null) {
                    record = this.transform.HandleRecord (record);
                    format = this.transform.GetFormat ();
                }

                // save it
                newRecords.push(record);
            }
        }

        // sort
        if (this.sort != null) {
            var scope = this;
            var metaData = this.GetMetaData ();
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
            newRecords.sort (sortFunc);
        }                
        this.PopulateDataResponse(newRecords, readOnly, format, event);
    }
};

Jane.DataReferenceCopy.ReceiveEvent = function (sender, event) {
    DEBUGLOG(this.name + " receives " + event + " from " + sender.name);
    switch (event) {
        case Jane.events.DATA_POPULATED:
            // we should only populate if we requested this
            if ("populateRequested" in this) {
                delete this.populateRequested;
                this.CopyData(sender.GetData(), event);
            }
            break;
        case Jane.events.DATA_CHANGED:
            if (this.HasData()) {
                this.CopyData(sender.GetData(), event);
            }
            break;
        case Jane.events.DATA_FLUSHED:
            if (this.HasData()) {
                this.Flush ();
            }
            break;
        default:
            break;
    }
};

Jane.DataReferenceCopy.PopulateData = function () {
    var sourceData = this.source.GetData();
    if (sourceData != null) {
        this.CopyData(sourceData, Jane.events.DATA_POPULATED);
    } else {
        this.populateRequested = true;
        this.source.Populate();
    }
};

Jane.DataReferenceCopy.HasMetaData = function () {
    return  (Jane.DataReference.HasMetaData.call (this)) ? true : this.source.HasMetaData ();
};

Jane.DataReferenceCopy.GetMetaData = function () {
    return  (Jane.DataReference.HasMetaData.call (this)) ? Jane.DataReference.GetMetaData.call (this) : this.source.GetMetaData ();
};

