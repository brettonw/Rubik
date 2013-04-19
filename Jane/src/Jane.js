//------------------------------------------------------------------------------
// Jane is a data source manager for client-only operations on result sets. It
// is designed to be a lazy instantiator of queries, filters, and transform-
// ations using shallow copies to manage memory utilization. 
//
// Messaging is accomplished using a simple direct subscriber mechanism, rather
// than use channels or any universal publish and subscribe methodology.
//
// We include a "contract" for subscribers so that lazy instantiations can be 
// shared across widgets when we can guarantee they won't conflict with each 
// other in use.
//
//------------------------------------------------------------------------------
var Jane = Object.create (EventSource).Init ({ "name" : "Jane" });

Jane.constants = {
    DEFINE_AS(__IDENTIFIER__),
    DEFINE_AS(__HIGHTLIGHTED__)
};

Jane.events = {
    DEFINE_AS(DATA_REFERENCE_ADDED),

    DEFINE_AS(DATA_POPULATED),
    DEFINE_AS(DATA_FLUSHED),
    DEFINE_AS(DATA_CHANGED),
    DEFINE_AS(HIGHLIGHT_CHANGED)
};

Jane.dataRefs = {};

Jane.AddDataReference = function (dataRef) {
    this.dataRefs [dataRef.name] = dataRef;
    this.PostEvent (Jane.events.DATA_REFERENCE_ADDED);
};
