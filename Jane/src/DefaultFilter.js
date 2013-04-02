//------------------------------------------------------------------------------
// filter plugins are javascript objects with a defined format used to 
// process records during the filtering phase
// object interface
//  - property: name
//  - property: params
//  - function: HandleRecord (record, params)
//
//------------------------------------------------------------------------------

Jane.DefaultFilter = Object.create(null);
Jane.DefaultFilter.name = "default";
Jane.DefaultFilter.params = {};
Jane.DefaultFilter.HandleRecord = function (record, params) {
    return true;
};
Jane.DataReferenceCopy.InstallFilterPlugin(Jane.DefaultFilter, {});
