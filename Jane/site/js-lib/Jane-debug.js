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
    this.metaData = {};
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
Jane.DataReference.HasMetaData = function () {
    return (Object.getOwnPropertyNames(this.metaData).length > 0);
};
Jane.DataReference.GetMetaData = function () {
    return this.metaData;
};
Jane.DataReference.AddFieldMetaData = function (fieldName, displayName, type) {
    this.metaData[fieldName] = {
        "fieldName" : fieldName,
        "displayName" : displayName,
        "type" : type
    };
};
Jane.DataReferenceJson = Object.create(Jane.DataReference);
Jane.DataReferenceJson.Init = function (params) {
    (Object.getPrototypeOf((Object.getPrototypeOf(this)))).Init.call(this, params);
    if ("url" in params) this["url"] = params["url"];
    if ("metaDataUrl" in params) this["metaDataUrl"] = params["metaDataUrl"];
    this.PopulateMetaData();
    return this;
};
Jane.DataReferenceJson.PopulateMetaData = function () {
    if (! this.HasMetaData()) {
        var scope = this;
        $.getJSON(this.metaDataUrl, function (metaData) {
            scope.PopulateMetaDataResponse(metaData);
        });
    }
};
Jane.DataReferenceJson.PopulateMetaDataResponse = function (metaData) {
    console.log (this.name + " populating metaData");
    var fields = metaData.columns;
    for (var i = 0, count = fields.length; i < count; ++i) {
        var field = fields[i];
        this.AddFieldMetaData(field.accessName, field.displayName, field.types[0]);
    }
    if ("populateRequested" in this) {
        delete this.populateRequested;
        this.PopulateData();
    }
};
Jane.DataReferenceJson.PopulateData = function () {
    if (Object.getOwnPropertyNames(this.metaData).length > 0) {
        var scope = this;
        $.getJSON(this.url, function (data) {
            scope.PopulateDataResponse(data, true, Jane.formats.OBJECT, Jane.events.DATA_POPULATED);
        });
    } else {
        this.populateRequested = true;
    }
};
Jane.DataReferenceCopy = Object.create(Jane.DataReference);
Jane.DataReferenceCopy.Init = function (params) {
    (Object.getPrototypeOf((Object.getPrototypeOf(this)))).Init.call(this, params);
    this.filter = null;
    this.select = null;
    this.transform = null;
    this.sort = null;
    if ("source" in params) this["source"] = params["source"];
    this.source.SubscribeReadOnly (this, this.ReceiveEvent);
    return this;
};
Jane.DataReferenceCopy.Validate = function () {
    debugger;
    if (! this.HasMetaData ()) {
        return false;
    }
    var metaData = this.GetMetaData ();
    for (var i = 0, count = this.select.length; i < count; ++i) {
        var fieldName = this.select[i];
        if (! (fieldName in metaData)) {
            console.log (this.name + " invalid selection field (" + fieldName + ")");
            return false;
        }
    }
    for (var i = 0, count = this.sort.length; i < count; ++i) {
        var fieldName = this.sort[i].name;
        if (! (fieldName in metaData)) {
            console.log (this.name + " invalid sort field (" + fieldName + ")");
            return false;
        }
    }
    return true;
};
Jane.DataReferenceCopy.CopyData = function (srcData, event) {
    if (this.Validate ()) {
        var data = srcData.data;
        var format = this.GetDataFormat();
        var readOnly = this.GetDataIsReadOnly();
        var newData = [];
        for (var i = 0, count = data.length; i < count; ++i) {
            var record = data[i];
            if ((this.filter === null) || (this.filter.HandleRecord (record))) {
                if (this.select === null) {
                    if (! readOnly) {
                        record = Object.create (record);
                        format = Jane.formats.OBJECT_AS_PROTOTYPE;
                    }
                } else {
                    var newRecord = {};
                    for (var j = 0, selectCount = this.select.length; j < selectCount; ++j) {
                        var fieldName = this.select[j];
                        newRecord[fieldName] = record[fieldName];
                    }
                    record = newRecord;
                    format = Jane.formats.OBJECT;
                }
                if (this.transform !== null) {
                    record = this.transform.HandleRecord (record);
                }
                newData.push(record);
            }
        }
        if (this.sort !== null) {
            var scope = this;
            var metaData = this.GetMetaData ();
            var sortCount = this.sort.length;
            var sortFunc = function (a, b) {
                var sortOrderLexical = function(a, b, type, asc) {
                    switch (type) {
                        case "integer" :
                        case "double" :
                        case "string" : {
                            var na = Number(a);
                            var nb = Number(b);
                            if ((na == a.toString ()) && (nb == b.toString ())) {
                                return asc ? (na - nb) : (nb - na);
                            }
                            return asc ? a.localeCompare (b) : b.localeCompare (a);
                        } break;
                        case "temporal" : {
                            var da = new Date(a).valueOf ();
                            var db = new Date(b).valueOf ();
                            return asc ? (da - db) : (db - da);
                        } break;
                    };
                    return 0;
                };
                for (var i = 0, count = scope.sort.length; i < count; ++i) {
                    var sortField = scope.sort[i];
                    if (sortField.name in metaData) {
                        var sortResult = sortOrderLexical(a[sortField.name], b[sortField.name], metaData[sortField.name].type, sortField.asc);
                        if (sortResult != 0) {
                            return sortResult;
                        }
                    }
                }
                return 0;
            };
            newData.sort (sortFunc);
        }
        this.PopulateDataResponse({ data : newData }, readOnly, format, event);
    }
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
Jane.DataReferenceCopy.HasMetaData = function () {
    return ((Object.getPrototypeOf((Object.getPrototypeOf(this)))).HasMetaData.call (this)) ? true : this.source.HasMetaData ();
};
Jane.DataReferenceCopy.GetMetaData = function () {
    return ((Object.getPrototypeOf((Object.getPrototypeOf(this)))).HasMetaData.call (this)) ? (Object.getPrototypeOf((Object.getPrototypeOf(this)))).GetMetaData.call (this) : this.source.GetMetaData ();
};
