//------------------------------------------------------------------------------
// filter and transform plugins are javascript objects with a defined interface:
//  - property: name
//  - function: HandleRecord (record)
//  
//------------------------------------------------------------------------------
Jane.TransformExtract = function (base) {
    var TransformExtract = Object.create(base, {
        "name": {
            "value": "Extract",
            "enumerable": true,
            "writable": false
        },
        "extract": {
            "value": "xxx", // not undefined, but not *likely* to be in a record
            "enumerable": true,
            "writable": true
        }
    });

    TransformExtract.Init = function (params) {
        // copy some parameters
        COPY_PARAM(extract, params);
        return this;
    };

    TransformExtract.HandleRecord = function (record, readOnly) {
        // don't really need to check that the transform has been created correctly
        // since it will fail without...

        // "extract" uses one sub-element of the record passed in as the record
        if (this.extract in record) {
            return readOnly ? record[this.extract] : Object.create(record[this.extract]);
        } else {
            // couldn't do it, log the error and return the original
            DEBUGLOG("Can't extract '" + this.extract + "' from record");
            DEBUGGER;
        }
        return record;
    };

    return TransformExtract;
}(null);
