"use strict";
var Jane = Object.create (null);
Jane.events = {
    DATA_POPULATED : "DATA_POPULATED",
    DATA_FLUSHED : "DATA_FLUSHED",
    DATA_CHANGED : "DATA_CHANGED"
};
Jane.formats = {
    EMPTY : "EMPTY",
    OBJECT : "OBJECT",
    OBJECT_AS_PROTOTYPE : "OBJECT_AS_PROTOTYPE"
};
Jane.DataReference = Object.create(null);
Jane.DataReference.allowFlushForSubscription = false;
Jane.DataReference.Init = function(params) {
    this.subscriptions = [];
    if ("name" in params) this["name"] = params["name"];
    if ("allowFlushForSubscription" in params) this["allowFlushForSubscription"] = params["allowFlushForSubscription"];
    return this;
};
Jane.DataReference.CanSubscribe = function (contract) {
    if (Object.getOwnPropertyNames (contract).length > 0) {
        for (var contractElement in contract) {
            if (contract.hasOwnProperty (contractElement)) {
                for (var i = 0, count = this.subscriptions.length; i < count; ++i) {
                    var subscription = this.subscriptions[i];
                    if (contractElement in subscription.contract) {
                        return false;
                    }
                }
            }
        }
        if (this.HasData () && this.chachedData.readOnly) {
            if (this.allowFlushForSubscription) {
                this.Flush ();
            } else {
                return false;
            }
        }
    }
    return true;
};
Jane.DataReference.Subscribe = function(target, receiver, contract) {
    if (this.CanSubscribe (contract)) {
        for (var i = 0, count = this.subscriptions.length; i < count; ++i) {
            var subscription = this.subscriptions[i];
            if (target == subscription.target) {
                return false;
            }
        }
        console.log (target.name + " subscribes to " + this.name);
        this.subscriptions.push ({ "target" : target, "receiver" : receiver, "contract" : contract});
        if (this.HasData ()) {
            receiver.apply (target, [Jane.events.DATA_POPULATED, this.cachedData]);
        }
        return true;
    }
    return false;
};
Jane.DataReference.SubscribeReadOnly = function(target, receiver) {
    return this.Subscribe (target, receiver, {});
};
Jane.DataReference.Unsubscribe = function(target) {
    for (var i = 0, count = this.subscriptions.length; i < count; ++i) {
        var subscription = this.subscriptions[i];
        if (target == subscription.target) {
            this.subscriptions.splice (i, 1);
            return;
        }
    }
};
Jane.DataReference.PostEvent = function (event) {
    for (var i = 0, count = this.subscriptions.length; i < count; ++i) {
        var subscription = this.subscriptions[i];
        console.log (this.name + " posting " + event + " to " + subscription.target.name);
        subscription.receiver.apply (subscription.target, [this, event]);
    }
};
Jane.DataReference.HasData = function () {
    return ("cachedData" in this);
};
Jane.DataReference.GetData = function () {
    return (this.HasData ()) ? this.cachedData.data : null;
};
Jane.DataReference.GetDataIsReadOnly = function() {
    if (this.HasData ()) { return this.cachedData.readOnly; }
    for (var i = 0, count = this.subscriptions.length; i < count; ++i) {
        var subscription = this.subscriptions[i];
        if (Object.getOwnPropertyNames (subscription.contract).length > 0) {
            return false;
        }
    }
    return true;
};
Jane.DataReference.GetDataFormat = function() {
    if (this.HasData ()) { return this.cachedData.format; }
    return Jane.formats.EMPTY;
};
Jane.DataReference.PopulateDataResponse = function (data, readOnly, format, event) {
    this.cachedData = { "data" : data, "readOnly" : readOnly, "format" : format };
    this.PostEvent (event);
};
Jane.DataReference.PopulateData = function () {
};
Jane.DataReference.Populate = function () {
    if (! this.HasData ()) { this.PopulateData (); }
};
Jane.DataReference.Flush = function () {
    if (this.HasData ()) {
        delete this.cachedData;
        this.PostEvent (Jane.events.DATA_FLUSHED);
    }
};
Jane.DataReference.Refresh = function () {
    this.Flush ();
    this.Populate ();
};
Jane.DataReferenceJson = Object.create(Jane.DataReference);
Jane.DataReferenceJson.Init = function (params) {
    (Object.getPrototypeOf((Object.getPrototypeOf(this)))).Init.call(this, params);
    if ("url" in params) this["url"] = params["url"];
    return this;
};
Jane.DataReferenceJson.PopulateData = function () {
    var scope = this;
    $.getJSON(this.url, function (data) {
        scope.PopulateDataResponse(data, true, Jane.formats.OBJECT, Jane.events.DATA_POPULATED);
    });
};
Jane.DataReferenceCopy = Object.create(Jane.DataReference);
Jane.DataReferenceCopy.filterPlugins = {};
Jane.DataReferenceCopy.transformPlugins = {};
Jane.DataReferenceCopy.Init = function (params) {
    (Object.getPrototypeOf((Object.getPrototypeOf(this)))).Init.call(this, params);
    this.filters = [];
    this.sort = [];
    this.transform = "none";
    if ("source" in params) this["source"] = params["source"];
    this.source.SubscribeReadOnly (this, this.ReceiveEvent);
    return this;
};
Jane.DataReferenceCopy.FilterRecord = function (record) {
    return true;
};
Jane.DataReferenceCopy.TransformRecord = function (record) {
    return record;
};
Jane.DataReferenceCopy.CopyData = function (data, event) {
    var copiedData = [];
    var readOnly = this.GetDataIsReadOnly();
    var format;
    if (readOnly) {
        format = data.format;
        for (var i = 0, count = data.data.length; i < count; ++i) {
            var record = data.data[i];
            if (this.FilterRecord(record)) {
                var transformedRecord = this.TransformRecord(record);
                copiedData.push(transformedRecord);
            }
        }
    } else {
        format = this.formats.OBJECT_AS_PROTOTYPE;
        for (var i = 0, count = data.data.length; i < count; ++i) {
            copiedData.push(Object.create (data.data[i]));
        }
    }
    this.PopulateDataResponse({ data : copiedData }, readOnly, format, event);
};
Jane.DataReferenceCopy.ReceiveEvent = function (sender, event) {
    console.log (this.name + " receives " + event + " from " + sender.name);
    switch (event) {
        case Jane.events.DATA_POPULATED:
            if ("populateRequested" in this) {
                delete this.populateRequested;
                this.CopyData(sender.GetData(), event);
            }
            break;
        case Jane.events.DATA_CHANGED:
            if (this.HasData()) {
                this.CopyData(sender.GetData(), event);
            }
            break;
        case Jane.events.DATA_FLUSHED:
            if (this.HasData()) {
                this.Flush ();
            }
            break;
        default:
            break;
    }
};
Jane.DataReferenceCopy.PopulateData = function () {
    var sourceData = this.source.GetData();
    if (sourceData != null) {
        this.CopyData(sourceData, Jane.events.DATA_POPULATED);
    } else {
        this.populateRequested = true;
        this.source.Populate();
    }
};
Jane.DataReferenceCopy.InstallFilterPlugin = function (plugin) {
    this.filterPlugins[plugin.name] = plugin;
};
Jane.DataReferenceCopy.InstallTransformPlugin = function (plugin) {
    this.transformPlugins[plugin.name] = plugin;
};
Jane.DataReferenceCopy.Transform = function (name, params) {
};
Jane.DefaultFilter = Object.create(null);
Jane.DefaultFilter.name = "default";
Jane.DefaultFilter.params = {};
Jane.DefaultFilter.HandleRecord = function (record, params) {
    return true;
};
Jane.DataReferenceCopy.InstallFilterPlugin(Jane.DefaultFilter, {});
Jane.DefaultTransform = Object.create(null);
Jane.DefaultTransform.name = "default";
Jane.DefaultTransform.params = {};
Jane.DefaultTransform.HandleRecord = function (record, format, readOnly, params) {
    return (readOnly) ? record : Object.create(record);
};
Jane.DefaultTransform.GetTransformedFormat = function (format, readOnly, params) {
    return (readOnly) ? format : Jane.formats.OBJECT_AS_PROTOTYPE;
};
Jane.DataReferenceCopy.InstallTransformPlugin(Jane.DefaultTransform);
