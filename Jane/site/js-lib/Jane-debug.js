"use strict";
var EventSource = Object.create(null);
EventSource.Init = function (params) {
    this.subscriptions = [];
    if ("name" in params) this["name"] = params["name"];
    return this;
};
EventSource.Subscribe = function (target, receiver) {
    if (this.subscriptions.indexOf(target) >= 0) {
        return null;
    }
    console.log (target.name + " subscribes to " + this.name);
    var subscription = { "target": target, "receiver": receiver };
    this.subscriptions.push(subscription);
    return subscription;
};
EventSource.Unsubscribe = function (target) {
    var i = this.subscriptions.indexOf(target);
    if (i >= 0) {
        this.subscriptions.splice(i, 1);
    }
};
EventSource.PostEvent = function (event) {
    for (var i = 0, count = this.subscriptions.length; i < count; ++i) {
        var subscription = this.subscriptions[i];
        console.log (this.name + " posting " + event + " to " + subscription.target.name);
        subscription.receiver.apply(subscription.target, [this, event]);
    }
};
var Jane = Object.create (EventSource);
Jane.constants = {
    __IDENTIFIER__ : "__IDENTIFIER__",
    __HIGHTLIGHTED__ : "__HIGHTLIGHTED__"
};
Jane.events = {
    DATA_POPULATED : "DATA_POPULATED",
    DATA_FLUSHED : "DATA_FLUSHED",
    DATA_CHANGED : "DATA_CHANGED",
    HIGHLIGHT_CHANGED : "HIGHLIGHT_CHANGED"
};
Jane.formats = {
    EMPTY : "EMPTY",
    OBJECT : "OBJECT",
    OBJECT_AS_PROTOTYPE : "OBJECT_AS_PROTOTYPE"
};
Jane.dataRefs = {};
Jane.DataReference = Object.create(EventSource);
Jane.DataReference.allowFlushForSubscription = false;
Jane.DataReference.Init = function(params) {
    EventSource.Init.call(this, params);
    this.metaData = {
        "fields" : {},
        "tags" : {}
    };
    if ("allowFlushForSubscription" in params) this["allowFlushForSubscription"] = params["allowFlushForSubscription"];
    Jane.dataRefs[this.name] = this;
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
        var subscription = EventSource.Subscribe.call (this, target, receiver);
        if (subscription != null) {
            subscription["contract"] = contract;
            if (this.HasData ()) {
                receiver.apply (target, [Jane.events.DATA_POPULATED, this.cachedData]);
            }
        }
        return subscription;
    }
    return null;
};
Jane.DataReference.SubscribeReadOnly = function(target, receiver) {
    return this.Subscribe (target, receiver, {});
};
Jane.DataReference.HasData = function () {
    return ("cachedData" in this);
};
Jane.DataReference.GetData = function () {
    return (this.HasData ()) ? this.cachedData.rows : null;
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
Jane.DataReference.PopulateDataResponse = function (rows, readOnly, format, event) {
    this.cachedData = { "rows" : rows, "readOnly" : readOnly, "format" : format };
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
    return (Object.getOwnPropertyNames(this.metaData.fields).length > 0);
};
Jane.DataReference.GetMetaData = function () {
    return this.metaData;
};
Jane.DataReference.AddFieldMetaData = function (fieldName, displayName, type, tags) {
    console.log (this.name + " adding metaData for " + fieldName + " as " + type);
    var field = {
        "fieldName" : fieldName,
        "displayName" : displayName,
        "type" : type
    };
    this.metaData.fields[fieldName] = field;
    for (var i = 0, count = tags.length; i < count; ++i) {
        var tag = tags[i];
        console.log (this.name + " tagging " + fieldName + " (" + tag + ")");
        if (! (tag in this.metaData.tags)) {
            this.metaData.tags[tag] = [];
        }
        this.metaData.tags[tag].push (field);
    }
};
Jane.DataReference.ValidateMetaData = function () {
};
Jane.DataReferenceEspace = Object.create(Jane.DataReference);
Jane.DataReferenceEspace.Init = function (params) {
    Jane.DataReference.Init.call(this, params);
    if ("url" in params) this["url"] = params["url"];
    if ("metaDataUrl" in params) this["metaDataUrl"] = params["metaDataUrl"];
    this.PopulateMetaData();
    return this;
};
Jane.DataReferenceEspace.PopulateMetaData = function () {
    if (! this.HasMetaData()) {
        var scope = this;
        $.getJSON(this.metaDataUrl, function (metaData) {
            scope.PopulateMetaDataResponse(metaData);
        });
    }
};
Jane.DataReferenceEspace.PopulateMetaDataResponse = function (metaData) {
    console.log (this.name + " populate metaData");
    var fields = metaData.columns;
    for (var i = 0, count = fields.length; i < count; ++i) {
        var field = fields[i];
        this.AddFieldMetaData(field.accessName, field.displayName, field.types[0], field.tags);
    }
    if ("populateRequested" in this) {
        delete this.populateRequested;
        this.PopulateData();
    }
};
Jane.DataReferenceEspace.PopulateData = function () {
    if (this.HasMetaData ()) {
        var scope = this;
        $.getJSON(this.url, function (data) {
            scope.PopulateDataResponse(data.rows, true, Jane.formats.OBJECT, Jane.events.DATA_POPULATED);
        });
    } else {
        this.populateRequested = true;
    }
};
Jane.DataReferenceCopy = Object.create(Jane.DataReference);
Jane.DataReferenceCopy.Init = function (params) {
    Jane.DataReference.Init.call(this, params);
    this.filter = null;
    this.select = null;
    this.transform = null;
    this.sort = null;
    if ("source" in params) this["source"] = params["source"];
    this.source.SubscribeReadOnly (this, this.ReceiveEvent);
    return this;
};
Jane.DataReferenceCopy.Validate = function () {
    if (! this.HasMetaData ()) {
        return false;
    }
    var metaData = this.GetMetaData ();
    if (this.select != null) {
        for (var i = 0, count = this.select.length; i < count; ++i) {
            var fieldName = this.select[i];
            if (! (fieldName in metaData.fields)) {
                console.log (this.name + " - invalid selection field (" + fieldName + ")");
                return false;
            }
        }
    }
    if (this.sort != null) {
        for (var i = 0, count = this.sort.length; i < count; ++i) {
            var fieldName = this.sort[i].name;
            if (! (fieldName in metaData.fields)) {
                console.log (this.name + " - invalid sort field (" + fieldName + ")");
                return false;
            }
        }
    }
    return true;
};
Jane.DataReferenceCopy.CopyData = function (rows, event) {
    if (this.Validate ()) {
        var format = this.GetDataFormat();
        var readOnly = this.GetDataIsReadOnly();
        var newRows = [];
        for (var i = 0, count = rows.length; i < count; ++i) {
            var record = rows[i];
            if ((this.filter == null) || (this.filter.HandleRecord (record))) {
                if (this.select == null) {
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
                if (this.transform != null) {
                    record = this.transform.HandleRecord (record);
                    format = this.transform.GetFormat ();
                }
                newRows.push(record);
            }
        }
        if (this.sort != null) {
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
                    var sortResult = sortOrderLexical(a[sortField.name], b[sortField.name], metaData.fields[sortField.name].type, sortField.asc);
                    if (sortResult != 0) {
                        return sortResult;
                    }
                }
                return 0;
            };
            newRows.sort (sortFunc);
        }
        this.PopulateDataResponse(newRows, readOnly, format, event);
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
    return (Jane.DataReference.HasMetaData.call (this)) ? true : this.source.HasMetaData ();
};
Jane.DataReferenceCopy.GetMetaData = function () {
    return (Jane.DataReference.HasMetaData.call (this)) ? Jane.DataReference.GetMetaData.call (this) : this.source.GetMetaData ();
};
Jane.TransformFlatten = Object.create(null);
Jane.TransformFlatten.name = "Flatten";
Jane.TransformFlatten.EnumerateRecord = function (record, into) {
    for (var key in record) {
        if (record.hasOwnProperty(key)) {
            var value = record[key];
            var valueType = typeof (value);
            if (valueType == "object") {
                EnumerateRecord(value, into);
            } else {
                into[key] = value;
            }
        }
    }
};
Jane.TransformFlatten.HandleRecord = function (record) {
    var into = Object.create(null);
    this.EnumerateRecord(record, into);
    return into;
};
Jane.TransformFlatten.GetFormat = function () {
    return Jane.formats.OBJECT;
};
