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
var Jane = function () {
    if (("Jane" in window.top) && (window.top.Jane !== undefined)) {
        return window.top.Jane;
    }

    var Jane = Object.create (EventSource).Init ({ "name" : "Jane" });

    Jane.constants = {
        DEFINE_AS(__IDENTIFIER__),
        DEFINE_AS(__HIGHTLIGHT__),
        DEFINE_AS(__COLOR__)
    };

    Jane.events = {
        DEFINE_AS(DATA_REFERENCE_ADDED),

        DEFINE_AS(DATA_POPULATED),
        DEFINE_AS(DATA_FLUSHED),
        DEFINE_AS(DATA_CHANGED),
        DEFINE_AS(HIGHLIGHT_CHANGED)
    };

    Jane.dataRefs = { 
        index : {}, 
        nodes : [], 
        links : [], 
        depth : 0,

        AddNode : function (name, dataRef) {
            console.log ("node - " + name);
            var arrayIndex = this.nodes.length;
            var node = {
                "name"          : name,
                "arrayIndex"    : arrayIndex,
                "depth"         : 0,
                "children"      : [],
                "dataRef"       : dataRef
            };
            this.nodes.push (node);        
            this.index[name] = node;
            return node;
        }
    };
    Jane.dataRefs.AddNode ("Jane", null);

    Jane.AddDataReference = function (dataRef) {
        var dataRefs = this.dataRefs;
        var node = dataRefs.AddNode (dataRef.name, dataRef);

        var parentName = ("source" in dataRef) ? dataRef.source.name : "Jane";
        if (parentName in dataRefs.index) {
            var parent =  dataRefs.index[parentName];
            node.parent = parent;
            parent.children.push (node);
            node.depth = parent.depth + 1;
            dataRefs.depth = Math.max (node.depth, dataRefs.depth);
            dataRefs.links.push ({ 
                "source"    : parent.index, 
                "target"    : node.index
            });
        }

        dataRef.SubscribeReadOnly(this, this.ReceiveEvent);
        this.PostEvent (Jane.events.DATA_REFERENCE_ADDED);
    };

    Jane.GetDataReference = function (name) {
        if (name in this.dataRefs.index) {
            return this.dataRefs.index[name].dataRef;
        }
        return null;
    };

    Jane.ReceiveEvent = function (sender, event) {
        console.log(this.name + " receives " + event + " from " + sender.name);
        this.PostEvent (event);
    };

    window.top.Jane = Jane;
    return Jane;
} ();
