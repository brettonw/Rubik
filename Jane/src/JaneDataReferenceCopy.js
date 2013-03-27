//------------------------------------------------------------------------------
// A Jane Data Reference (JDR) Copy is an abstract, lightweight JDR that 
// describes a data source as a filter, sort, and/or transformation of another 
// JDR. This object is used to implement the filter plugin architecture.
//
// public interface
//  - function: 
//
//------------------------------------------------------------------------------

var JaneDataReferenceCopy = Object.create(JaneDataReference);

JaneDataReferenceCopy.init = function(params) {
    // do the parental init, and then do my thing here
    this.prototype.init.apply(this, params);

    // one of the inputs should be the JDR this one is referencing, save it, and 
    // subscribe to events on it
    this.source = params.source;
    this.source.SubscribeReadOnly (this, this.ReceiveEvent);

    return this;
};

JaneDataReferenceCopy.CopyData = function (data) {
    // this is where we filter and sort the data, creating a lightweight copy
    // if appropos. The filtering is performed using a plugin architecture that
    // each record is passed through, and then the sort is applied.
    var copiedData = [];
    var readOnly = this.IsReadOnly();

    // if this "copy" is going to be read only, we will use a slightly lighter
    // weight variant

    this.PopulateDataResponse({ data : copiedData }, readOnly);
};

JaneDataReferenceCopy.ReceiveEvent = function (event, data) {
    switch (event) {
        case JaneDataReference.events.DATA_POPULATED:
            this.CopyData(data);
            break;
        default:
            break;
    }
};

JaneDataReferenceCopy.PopulateData = function () {
    var sourceData = this.source.GetData();
    if (sourceData != null) {
        this.CopyData(sourceData);
    } else {
        this.source.Populate();
    }
};

JaneDataReference.FetchData = function (responsePackage) {
    // call to our source JDR to get its data, and handle the response in our
    // own response handler to implement the filter, sort, and/or transform
    this.sourceJDR.FetchData({ "target": this, "callback": this.FetchDataResponse, "originalResponsePackage": responsePackage });
};

JaneDataReferenceCopy.FetchDataResponse = function (responsePackage, data) {
    // this is where we filter and sort the data, creating a lightweight copy
    // if appropos. The filtering is performed using a plugin architecture that
    // each record is passed through, and then the sort is applied.
    var processedData = [];

    if (this.IsReadOnly()) {
    }

    // call the parent version of the response function with our processed data
    this.prototype.FetchDataResponse.apply(this, [responsePackage.originalResponsePackage, processedData]);
};

