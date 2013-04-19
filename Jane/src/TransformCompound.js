//------------------------------------------------------------------------------
// filter and transform plugins are javascript objects with a defined interface:
//  - property: name
//  - function: HandleRecord (record)
//  
//------------------------------------------------------------------------------
Jane.TransformCompound = function (base) {
    var TransformCompound = Object.create(base, {
        "name": {
            "value": "Compound",
            "enumerable": true,
            "writable": false
        }
    });

    TransformCompound.Init = function (params) {
        // pull all of the transforms out in an array
        COPY_PARAM(transforms, params);
        return this;
    };

    TransformCompound.HandleRecord = function (record, readOnly) {
        // loop over all of the transforms in order, and pass the transformed
        // record to each one in turn.
        for (var i = 0, count = this.transforms.length; i < count; ++i) {
            record = this.transforms[i].HandleRecord(record, readOnly);
        }
        return record;
    };

    return TransformCompound;
}(null);
