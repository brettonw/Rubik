//------------------------------------------------------------------------------
// A Jane Data Reference (JDR) Copy is an abstract, lightweight JDR that 
// describes a data source as a filter, sort, and/or transformation of another 
// JDR. This object is used to implement the filter plugin architecture.
//
// global interface
//  + function: AddFilterPlugin (plugin, params)
//  + function: AddTransformPlugin (plugin, params)
//
// object interface
//  - function: SetTransform (name)
//  - function: SetFilter (name, {params})
//  - function: ClearFilters ()
//  - function: SetSort ([fields])
//
// global values
//  + property: events
//      - DATA_POPULATED
//      - DATA_FLUSHED
//      - DATA_CHANGED
//
// filter and transform plugins are javascript objects with several defined
// interfaces:
// object interface
//  - property: name
//  - function: HandleRecord (record, format, readOnly, params)
//  - 
//------------------------------------------------------------------------------

Jane.DataReferenceCopy = Object.create(Jane.DataReference);

Jane.DataReferenceCopy.filterPlugins = {};
Jane.DataReferenceCopy.transformPlugins = {};

Jane.DataReferenceCopy.Init = function (params) {
    // do the parental init, and then do my thing here
    SUPER.Init.call(this, params);

    // set up the default filter, transform, and sort options to be blank
    this.filters = [];
    this.sort = [];
    this.transform = this.SetTransform ("default");

    // one of the inputs should be the JDR this one is referencing, save it, and 
    // subscribe to events on it
    COPY_PARAM(source, params);
    this.source.SubscribeReadOnly (this, this.ReceiveEvent);

    return this;
};

Jane.DataReferenceCopy.FilterRecord = function (record) {
    return true;
};

Jane.DataReferenceCopy.TransformRecord = function (record) {
    return record;
};

Jane.DataReferenceCopy.CopyData = function (data, event) {
    // this is where we filter and sort the data, creating a lightweight copy
    // if appropos. The filtering is performed using a plugin architecture that
    // each record is passed through, and then the sort is applied.
    var copiedData = [];
    var readOnly = this.GetDataIsReadOnly();
    var format;

    // if this "copy" is going to be read only, we will use the object stored
    // in the source, otherwise we will create a lightweight copy
    if (readOnly) {
        format = data.format;
        for (var i = 0, count = data.data.length; i < count; ++i) {
            var record = data.data[i];
            if (this.FilterRecord(record)) {
                var transformedRecord = this.TransformRecord(record);
                copiedData.push(transformedRecord);
            }
        }
    } else {
        format = this.formats.OBJECT_AS_PROTOTYPE;
        for (var i = 0, count = data.data.length; i < count; ++i) {
            copiedData.push(Object.create (data.data[i]));
        }
    }

    // implement the sort, we assume that all record formats implement the 
    // bracket notation to retrieve records

    this.PopulateDataResponse({ data : copiedData }, readOnly, format, event);
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

Jane.DataReferenceCopy.InstallFilterPlugin = function (plugin) {
    this.filterPlugins[plugin.name] = plugin;
};

Jane.DataReferenceCopy.InstallTransformPlugin = function (plugin) {
    this.transformPlugins[plugin.name] = plugin;
};

Jane.DataReferenceCopy.SetTransform = function (name, params) {
    //this.transform = 
};

Jane.DataReferenceCopy.AddFilter = function (name, params) {
};

Jane.DataReferenceCopy.ClearFilters = function () {
    this.filters = [];
};

Jane.DataReferenceCopy.AddSort = function (field, asc) {
};
