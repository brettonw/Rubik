//------------------------------------------------------------------------------
// filter and transform plugins are javascript objects with a defined interface:
//  - property: name
//  - function: HandleRecord (record)
//  
//------------------------------------------------------------------------------
Jane.TransformFlatten = function (base) {
    var TransformFlatten = Object.create(base, {
        "name": {
            "value": "Flatten",
            "enumerable": true,
            "writable": false
        }
    });

    TransformFlatten.EnumerateRecord = function (record, into) {
        // recursive function on objects
        for (var key in record) {
            if (record.hasOwnProperty(key)) {
                var value = record[key];
                var valueType = typeof (value);
                if (valueType == "object") {
                    this.EnumerateRecord(value, into);
                } else {
                    into[key] = value;
                }
            }
        }
    };

    TransformFlatten.HandleRecord = function (record, readOnly) {
        // flatten traverses a record and returns a new record with all sub-objects
        // flattened out into a single object
        var into = Object.create(null);
        this.EnumerateRecord(record, into);
        return into;
    };

    return TransformFlatten;
}(null);
