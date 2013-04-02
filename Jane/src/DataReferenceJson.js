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

    return this;
};

Jane.DataReferenceJson.PopulateData = function () {
    // use jquery to fetch the JSON response
    var scope = this;
    $.getJSON(this.url, function (data) {
        scope.PopulateDataResponse(data, true, Jane.formats.OBJECT, Jane.events.DATA_POPULATED);
    });
};
