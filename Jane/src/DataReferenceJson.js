//------------------------------------------------------------------------------
// A JSON JDR is a simple JDR that implements retrieval of JSON from a given URL
//  
//------------------------------------------------------------------------------

Jane.DataReferenceJson = Object.create(Jane.DataReference);

Jane.DataReferenceJson.Init = function (params) {
    // do the parental init, and then do my thing here
    SUPER.Init.call(this, params);
    
    // copy some parameters
    COPY_PARAM(url, params);
    COPY_PARAM(metaDataUrl, params);

    // try to populate the metaData
    this.PopulateMetaData();

    return this;
};

Jane.DataReferenceJson.PopulateMetaData = function () {
    if (NOT this.HasMetaData()) {
        // use jquery to fetch the JSON response
        var scope = this;
        $.getJSON(this.metaDataUrl, function (metaData) {
            scope.PopulateMetaDataResponse(metaData);
        });
    }
};

Jane.DataReferenceJson.PopulateMetaDataResponse = function (metaData) {
    // process the metaData format from the espace program
    DEBUGLOG (this.name + " populating metaData");

    // XXX can this be discoverable somehow

    // add the field meta data
    var fields = metaData.columns;
    for (var i = 0, count = fields.length; i < count; ++i) {
        var field = fields[i];
        this.AddFieldMetaData(field.accessName, field.displayName, field.types[0]);
    }

    // if we held off requesting data because we didn't have metaData yet...
    if ("populateRequested" in this) {
        delete this.populateRequested;
        this.PopulateData();
    }
};

Jane.DataReferenceJson.PopulateData = function () {
    // don't do this unless we already have metaData
    if (Object.getOwnPropertyNames(this.metaData).length > 0) {
        // use jquery to fetch the JSON response
        var scope = this;
        $.getJSON(this.url, function (data) {
            scope.PopulateDataResponse(data, true, Jane.formats.OBJECT, Jane.events.DATA_POPULATED);
        });
    } else {
        this.populateRequested = true;
    }
};
