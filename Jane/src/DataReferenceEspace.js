//------------------------------------------------------------------------------
// A JSON JDR is a simple JDR that implements retrieval of JSON from a given URL
//  
//------------------------------------------------------------------------------

Jane.DataReferenceEspace = Object.create(Jane.DataReference);

Jane.DataReferenceEspace.Init = function (resultSet) {
    // the input is a result set
    /*
    {
        "resultSetName":"Stops",
        "resultSetId":"job@6d05e087-5995-4b58-876d-e255e97f69e4",
        "state":"FINISHED",
        "numRows":82,
        "dataSourceName":"ny_transit_stopsOracle",
        "resultSetUrl":"http://localhost:8080/espace/rest/store/resultset/job@6d05e087-5995-4b58-876d-e255e97f69e4",
        "cdmMapUrl":"/espace/rest/map/job@6d05e087-5995-4b58-876d-e255e97f69e4",
        "lastAccessTime":1365540277897,
        "submitTime":1365540070738
    }
    */
    
    // do the parental init, and then do my thing here
    Jane.DataReference.Init.call(this, { "name" : resultSet.resultSetName });

    // copy some parameters
    COPY_PARAM_AS(resultSetUrl, dataUrl, resultSet);
    COPY_PARAM_AS(cdmMapUrl, metaDataUrl, resultSet);
    COPY_PARAM_AS(numRows, rowCount, resultSet);

    // try to populate the metaData
    this.PopulateMetaData();

    return this;
};

Jane.DataReferenceEspace.PopulateMetaData = function () {
    if (NOT this.HasMetaData()) {
        // if the metaDataUrl is a relative URL...
        var dataUri = parseUri (this.dataUrl);
        var metaDataUrl = dataUri.protocol + "://" + dataUri.authority + this.metaDataUrl;

        // use jquery to fetch the JSON response
        var scope = this;
        $.getJSON(metaDataUrl, function (metaData) {
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
        $.getJSON(this.dataUrl, function (data) {
            // XXX what are the failure modes here?
            scope.PopulateDataResponse(data.rows, true, Jane.formats.OBJECT, Jane.events.DATA_POPULATED);
        });
    } else {
        this.populateRequested = true;
    }
};
