//------------------------------------------------------------------------------
// transform plugins are javascript objects with a defined format used to 
// process records during the transformation phase
// object interface
//  - property: name
//  - property: params
//  - function: HandleRecord (record, format, readOnly, params)
//  - function: GetTransformedFormat (format, readOnly, params)
//
//------------------------------------------------------------------------------

Jane.DefaultTransform = Object.create(null);
Jane.DefaultTransform.name = "default";
Jane.DefaultTransform.params = {};
Jane.DefaultTransform.HandleRecord = function (record, format, readOnly, params) {
    // XXX could check the incoming format to see if it's known - do we care?
    return (readOnly) ? record : Object.create(record);
};
Jane.DefaultTransform.GetTransformedFormat = function (format, readOnly, params) {
    // XXX could check the incoming format to see if it's known - do we care?
    return (readOnly) ? format : Jane.formats.OBJECT_AS_PROTOTYPE;
};
Jane.DataReferenceCopy.InstallTransformPlugin(Jane.DefaultTransform);
