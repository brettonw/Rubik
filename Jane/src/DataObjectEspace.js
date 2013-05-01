//------------------------------------------------------------------------------
// A JSON JDR is a simple JDR that implements retrieval of JSON from a given URL
//  
//------------------------------------------------------------------------------

Jane.DataObjectEspace = function (base) {
    var DataObjectEspace = Object.create(base);

    DataObjectEspace.Init = function (params) {
        // copy some parameters
        COPY_PARAM_AS(resultSetUrl, dataUrl, params);
        COPY_PARAM_AS(cdmMapUrl, metaDataUrl, params);
        COPY_PARAM_AS(dataSourceName, metaDataName, params);
        COPY_PARAM_AS(numRows, recordCount, params);

        // do the parental init
        params["name"] = params.resultSetName;
        base.Init.call(this, params);

        // try to populate the metaData
        this.PopulateMetaData();

        return this;
    };

    DataObjectEspace.PopulateMetaData = function () {
        if (!this.HasMetaData()) {
            // if the metaDataUrl is a relative URL...
            var parser = document.createElement('a');
            parser.href = this.dataUrl;
            var metaDataUrl = parser.protocol + "//" + parser.host + "/espace/rest/data/sources/" + this.metaDataName;

            // use jquery to fetch the JSON response
            var scope = this;
            $.getJSON(metaDataUrl, function (metaData) {
                // XXX what are the failure modes here?
                scope.PopulateMetaDataResponse(metaData);
            });
        }
    };

    DataObjectEspace.PopulateMetaDataResponse = function (espaceMetaData) {
        DEBUGLOG(this.name + " populate metaData");
        // save the metaData format from the espace program
        this.espaceMetaData = espaceMetaData;

        // add the field meta data
        var fields = espaceMetaData.columns;
        for (var i = 0, count = fields.length; i < count; ++i) {
            var field = fields[i];
            this.AddFieldMetaData(field.accessName, field.displayName, field.types[0], field.tags);
        }
        this.ValidateMetaData();

        // if we held off requesting data because we didn't have metaData yet...
        if ("populateRequested" in this) {
            delete this.populateRequested;
            this.PopulateData();
        }
    };

    DataObjectEspace.PopulateEspaceFields = function (rows) {
        // XXX interval columns
        // geo columns are all we handle right now
        // XXX timestep columns
        var espaceMetaData = this.espaceMetaData;
        var geoColumns = espaceMetaData.geoColumns;
        for (var i = 0, iCount = geoColumns.length; i < iCount; ++i) {
            var geoColumn = geoColumns[i];
            // the access name is where the field is...
            var findDisplaybyAccessName = function (accessName) {
                for (var k = 0, kCount = espaceMetaData.columns.length; k < kCount; ++k) {
                    var column = espaceMetaData.columns[k];
                    if (column.accessName == accessName) {
                        return column.displayName;
                    }
                }
            };
            var fieldName = findDisplaybyAccessName(geoColumn.geoColumn);
            var lonFieldName = findDisplaybyAccessName(geoColumn.lonColumn);
            var latFieldName = findDisplaybyAccessName(geoColumn.latColumn);

            // loop over all the rows to populate the field
            for (var j = 0, jCount = rows.length; j < jCount; ++j) {
                var rowData = rows[j].data;
                rowData[fieldName] = { "longitude": rowData[lonFieldName], "latitude": rowData[latFieldName] };
            }
        }
    };

    DataObjectEspace.PopulateData = function () {
        // don't do this unless we already have metaData
        if (this.HasMetaData()) {
            // use jquery to fetch the JSON response
            var scope = this;
            $.getJSON(this.dataUrl, function (data) {
                // XXX what are the failure modes here?
                // XXX I can also verify the numRecords value against the size of 
                // XXX the rows array returned
                scope.PopulateEspaceFields(data.rows);
                scope.PopulateDataResponse(data.rows, Jane.events.DATA_POPULATED);
            });
        } else {
            this.populateRequested = true;
        }
    };

    return DataObjectEspace;
}(Jane.DataObject);