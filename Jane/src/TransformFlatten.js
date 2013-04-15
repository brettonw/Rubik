//------------------------------------------------------------------------------
// filter and transform plugins are javascript objects with a defined interface:
//  - property: name
//  - function: HandleRecord (record)
//  
//------------------------------------------------------------------------------

Jane.TransformFlatten = Object.create(null);

Jane.TransformFlatten.name = "Flatten";

Jane.TransformFlatten.EnumerateRecord = function (record, into) {
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

Jane.TransformFlatten.HandleRecord = function (record) {
    // flatten traverses a record and returns a new record with all sub-objects
    // flattened out into a single object
    var into = Object.create(null);
    this.EnumerateRecord(record, into);
    return into;
};

Jane.TransformFlatten.GetFormat = function () {
    return Jane.formats.OBJECT;
};
