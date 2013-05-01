//------------------------------------------------------------------------------
// A Jane Data Reference (JDR) Copy is an abstract, lightweight JDR that 
// describes a data source as a filter, sort, and/or transformation of one or
// more JDRs.
//
// object interface
//  - function: Orphan - disconnect a populated data reference from its source
//                       data references
//
// we assume that all record formats implement the bracket notation to retrieve 
// fields within the record
//
//------------------------------------------------------------------------------

Jane.DataObjectReference = function (base) {
    var DataObjectReference = Object.create(base);

    DataObjectReference.Init = function (params) {
        // copy the sources
        COPY_PARAM(source, params);

        // do the parental init, and then do my thing here
        base.Init.call(this, params);

        // look to receive events from my source
        this.source.SubscribeReadOnly(this, this.ReceiveEvent);

        return this;
    };

    DataObjectReference.ReceiveEvent = function (sender, event) {
        DEBUGLOG(this.name + " receives " + event + " from " + sender.name);
        switch (event) {
            case Jane.events.DATA_POPULATED:
                // we should only populate if we requested this
                if ("populateRequested" in this) {
                    delete this.populateRequested;
                    this.PopulateDataResponse(sender.GetData(), event);
                }
                break;
            case Jane.events.DATA_CHANGED:
                if (this.HasData()) {
                    this.PopulateDataResponse(sender.GetData(), event);
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

    DataObjectReference.PopulateData = function () {
        var sourceData = this.source.GetData();
        if (sourceData != null) {
            this.PopulateDataResponse(sourceData, Jane.events.DATA_POPULATED);
        } else {
            this.populateRequested = true;
            this.source.Populate();
        }
    };

    DataObjectReference.HasMetaData = function () {
        return  (base.HasMetaData.call (this)) ? true : this.source.HasMetaData ();
    };

    DataObjectReference.GetMetaData = function () {
        return  (base.HasMetaData.call (this)) ? base.GetMetaData.call (this) : this.source.GetMetaData ();
    };

    return DataObjectReference;

}(Jane.DataObject);