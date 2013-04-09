//------------------------------------------------------------------------------
// A JSON JDR is a simple JDR that implements retrieval of JSON from a given URL
//  
//------------------------------------------------------------------------------

Jane.DataReferenceEspace = Object.create(Jane.DataReference);

Jane.DataReferenceEspace.Init = function (params) {
    // do the parental init, and then do my thing here
    Jane.DataReference.Init.call(this, params);

    // the data source should come in as a result set with a link to metadata
    // and the source rows
    
    // copy some parameters
    COPY_PARAM(url, params);
    COPY_PARAM(metaDataUrl, params);

    // try to populate the metaData
    this.PopulateMetaData();

    return this;
};

Jane.DataReferenceEspace.PopulateMetaData = function () {
    if (NOT this.HasMetaData()) {
        // use jquery to fetch the JSON response
        var scope = this;
        $.getJSON(this.metaDataUrl, function (metaData) {
            // XXX what are the failure modes here?
            scope.PopulateMetaDataResponse(metaData);
        });
    }
};

Jane.DataReferenceEspace.PopulateMetaDataResponse = function (metaData) {
    DEBUGLOG (this.name + " populate metaData");
    // process the metaData format from the espace program
    // XXX can this be discoverable somehow

    // add the field meta data
    var fields = metaData.columns;
    for (var i = 0, count = fields.length; i < count; ++i) {
        var field = fields[i];
        this.AddFieldMetaData(field.accessName, field.displayName, field.types[0], field.tags);
    }

    // if we held off requesting data because we didn't have metaData yet...
    if ("populateRequested" in this) {
        delete this.populateRequested;
        this.PopulateData();
    }
};

Jane.DataReferenceEspace.PopulateData = function () {
    // don't do this unless we already have metaData
    if (this.HasMetaData ()) {
        // use jquery to fetch the JSON response
        var scope = this;
        $.getJSON(this.url, function (data) {
            // XXX what are the failure modes here?
            scope.PopulateDataResponse(data.rows, true, Jane.formats.OBJECT, Jane.events.DATA_POPULATED);
        });
    } else {
        this.populateRequested = true;
    }
};
