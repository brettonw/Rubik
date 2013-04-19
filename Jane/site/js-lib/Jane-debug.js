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
var Jane = Object.create (EventSource).Init ({ "name" : "Jane" });
Jane.constants = {
    __IDENTIFIER__ : "__IDENTIFIER__",
    __HIGHTLIGHTED__ : "__HIGHTLIGHTED__"
};
Jane.events = {
    DATA_REFERENCE_ADDED : "DATA_REFERENCE_ADDED",
    DATA_POPULATED : "DATA_POPULATED",
    DATA_FLUSHED : "DATA_FLUSHED",
    DATA_CHANGED : "DATA_CHANGED",
    HIGHLIGHT_CHANGED : "HIGHLIGHT_CHANGED"
};
Jane.dataRefs = {};
Jane.AddDataReference = function (dataRef) {
    this.dataRefs [dataRef.name] = dataRef;
    this.PostEvent (Jane.events.DATA_REFERENCE_ADDED);
};
Jane.Utility = function (base) {
    var Utility = Object.create(base);
    Utility.SortLexical = function (a, b, type, asc) {
        switch (type) {
            case "integer":
            case "double":
            case "string": {
                var na = Number(a);
                var nb = Number(b);
                if ((na == a.toString()) && (nb == b.toString())) {
                    return asc ? (na - nb) : (nb - na);
                }
                return asc ? a.localeCompare(b) : b.localeCompare(a);
            } break;
            case "temporal": {
                var da = new Date(a).valueOf();
                var db = new Date(b).valueOf();
                return asc ? (da - db) : (db - da);
            } break;
        };
        return 0;
    };
    Utility.ObjectIsEmpty = function (object) {
        return (Object.getOwnPropertyNames(object).length == 0);
    };
    return Utility;
}(null);
Jane.DataObject = function (base) {
    var DataObject = Object.create(base, {
        "allowFlushForSubscription": {
            "value": false,
            "enumerable": true,
            "writable": true
        }
    });
    DataObject.Init = function (params) {
        base.Init.call(this, params);
        this.metaData = {
            "fields": {},
            "tags": {}
        };
        this.keyName = Jane.constants.__IDENTIFIER__;
        this["filter"] = ("filter" in params) ? params["filter"] : null;
        this["select"] = ("select" in params) ? params["select"] : null;
        this["transform"] = ("transform" in params) ? params["transform"] : null;
        this["sort"] = ("sort" in params) ? params["sort"] : null;
        if ("allowFlushForSubscription" in params) this["allowFlushForSubscription"] = params["allowFlushForSubscription"];
        Jane.AddDataReference(this);
        return this;
    };
    DataObject.CanSubscribe = function (contract) {
        if (! Jane.Utility.ObjectIsEmpty (contract)) {
            for (var contractElement in contract) {
                if (contract.hasOwnProperty(contractElement)) {
                    for (var i = 0, count = this.subscriptions.length; i < count; ++i) {
                        var subscription = this.subscriptions[i];
                        if (contractElement in subscription.contract) {
                            return false;
                        }
                    }
                }
            }
            if (this.HasData() && this.cachedData.readOnly) {
                if (this.allowFlushForSubscription) {
                    this.Flush();
                } else {
                    return false;
                }
            }
        }
        return true;
    };
    DataObject.Subscribe = function (target, receiver, contract) {
        if (this.CanSubscribe(contract)) {
            var subscription = EventSource.Subscribe.call(this, target, receiver);
            if (subscription != null) {
                subscription["contract"] = contract;
                if (this.HasData()) {
                    receiver.apply(target, [Jane.events.DATA_POPULATED, this.cachedData]);
                }
            }
            return subscription;
        }
        return null;
    };
    DataObject.SubscribeReadOnly = function (target, receiver) {
        return this.Subscribe(target, receiver, {});
    };
    DataObject.HasData = function () {
        return ("cachedData" in this);
    };
    DataObject.GetData = function () {
        return (this.HasData()) ? this.cachedData.records : null;
    };
    DataObject.GetDataIsReadOnly = function () {
        if (this.HasData()) { return this.cachedData.readOnly; }
        for (var i = 0, count = this.subscriptions.length; i < count; ++i) {
            var subscription = this.subscriptions[i];
            if (! Jane.Utility.ObjectIsEmpty (subscription.contract)) {
                return false;
            }
        }
        return true;
    };
    DataObject.Validate = function () {
        if (!this.HasMetaData()) {
            return false;
        }
        var metaData = this.GetMetaData();
        if (this.select != null) {
            for (var i = 0, count = this.select.length; i < count; ++i) {
                var fieldName = this.select[i];
                if (!(fieldName in metaData.fields)) {
                    console.log (this.name + " - invalid selection field (" + fieldName + ")");
                    return false;
                }
            }
        }
        if (this.sort != null) {
            for (var i = 0, count = this.sort.length; i < count; ++i) {
                var fieldName = this.sort[i].name;
                if (!(fieldName in metaData.fields)) {
                    console.log (this.name + " - invalid sort field (" + fieldName + ")");
                    return false;
                }
            }
        }
        return true;
    };
    DataObject.CopyData = function (records, readOnly) {
        if (this.Validate()) {
            var newRecords = [];
            for (var i = 0, count = records.length; i < count; ++i) {
                var record = records[i];
                if ((this.filter == null) || (this.filter.HandleRecord(record))) {
                    if (this.select == null) {
                        if (!readOnly) {
                            record = Object.create(record);
                        }
                    } else {
                        var newRecord = {};
                        for (var j = 0, selectCount = this.select.length; j < selectCount; ++j) {
                            var fieldName = this.select[j];
                            newRecord[fieldName] = record[fieldName];
                        }
                        record = newRecord;
                    }
                    if (this.transform != null) {
                        record = this.transform.HandleRecord(record, readOnly);
                    }
                    newRecords.push(record);
                }
            }
            if (this.sort != null) {
                var scope = this;
                var metaData = this.GetMetaData();
                var sortCount = this.sort.length;
                var sortFunc = function (a, b) {
                    for (var i = 0, count = scope.sort.length; i < count; ++i) {
                        var sortField = scope.sort[i];
                        var sortResult = Jane.Utility.SortLexical(a[sortField.name], b[sortField.name], metaData.fields[sortField.name].type, sortField.asc);
                        if (sortResult != 0) {
                            return sortResult;
                        }
                    }
                    return 0;
                };
                newRecords.sort(sortFunc);
            }
            return newRecords;
        }
        return null;
    };
    DataObject.PopulateDataResponse = function (records, event) {
        var readOnly = this.GetDataIsReadOnly();
        if ((this.sort != null) || (this.select != null) || (this.filter != null) || (this.transform != null) || (!readOnly)) {
            records = this.CopyData(records, readOnly);
        }
        if (records != null) {
            this.cachedData = { "records": records, "readOnly": readOnly, "key": null, "indexes": {} };
            this.PostEvent(event);
        }
    };
    DataObject.PopulateData = function () {
    };
    DataObject.Populate = function () {
        if (!this.HasData()) { this.PopulateData(); }
    };
    DataObject.Flush = function () {
        if (this.HasData()) {
            delete this.cachedData;
            this.PostEvent(Jane.events.DATA_FLUSHED);
        }
    };
    DataObject.Refresh = function () {
        this.Flush();
        this.Populate();
    };
    DataObject.HasMetaData = function () {
        return ! (Jane.Utility.ObjectIsEmpty(this.metaData.fields));
    };
    DataObject.GetMetaData = function () {
        return this.metaData;
    };
    DataObject.AddTagMetaData = function (tag, fieldName) {
        console.log (this.name + " tagging " + fieldName + " (" + tag + ")");
        if (!(tag in this.metaData.tags)) {
            this.metaData.tags[tag] = [];
        }
        this.metaData.tags[tag].push(fieldName);
    };
    DataObject.AddFieldMetaData = function (fieldName, displayName, type, tags) {
        console.log (this.name + " adding metaData for " + fieldName + " as " + type);
        var field = {
            "fieldName": fieldName,
            "displayName": displayName,
            "type": type
        };
        this.metaData.fields[displayName] = field;
        for (var i = 0, count = tags.length; i < count; ++i) {
            this.AddTagMetaData(tags[i], displayName);
        }
    };
    DataObject.ValidateMetaData = function () {
        var metaData = this.GetMetaData();
        if ("primary key" in metaData.tags) {
            this.keyName = metaData.tags["primary key"][0];
        } else {
            this.AddFieldMetaData(this.keyName, "Key", "integer", ["primary key"]);
        }
    };
    DataObject.GetKey = function () {
        var key = null;
        if (this.HasData()) {
            key = this.cachedData.key;
            if (key == null) {
                var fieldName = this.keyName;
                if (fieldName in this.metaData.fields) {
                    var records = this.cachedData.records;
                    if (!(fieldName in records[0])) {
                        for (var i = 0, count = records.length; i < count; ++i) {
                            records[i][fieldName] = i;
                        }
                    }
                    key = {};
                    for (var i = 0, count = records.length; i < count; ++i) {
                        var record = records[i];
                        key[record[fieldName]] = i;
                    }
                }
                this.cachedData.key = key;
            }
        }
        return key;
    };
    DataObject.GetIndex = function (fieldName) {
        var index = null;
        if (this.HasData()) {
            if (fieldName in this.cachedData.indexes) {
                index = this.cachedData.indexes[fieldName];
            } else {
                if (fieldName in this.metaData.fields) {
                    var records = this.cachedData.records;
                    if (!(fieldName in records[0])) {
                        if (fieldName == this.keyName) {
                            for (var i = 0, count = records.length; i < count; ++i) {
                                records[i][fieldName] = i;
                            }
                        }
                    }
                    index = [];
                    for (var i = 0, count = records.length; i < count; ++i) {
                        var record = records[i];
                        index.push({ "value": record[fieldName], "index": i });
                    }
                    var metaDataFields = this.metaData.fields;
                    var sortFunc = function (a, b) {
                        return Jane.Utility.SortLexical(a.value, b.value, metaDataFields[fieldName].type, true);
                    };
                    index.sort(sortFunc);
                    this.cachedData.indexes[fieldName] = index;
                }
            }
        }
        return index;
    };
    return DataObject;
}(EventSource);
Jane.DataObjectReference = function (base) {
    var DataObjectReference = Object.create(base);
    DataObjectReference.Init = function (params) {
        base.Init.call(this, params);
        if ("source" in params) this["source"] = params["source"];
        this.source.SubscribeReadOnly (this, this.ReceiveEvent);
        return this;
    };
    DataObjectReference.ReceiveEvent = function (sender, event) {
        console.log (this.name + " receives " + event + " from " + sender.name);
        switch (event) {
            case Jane.events.DATA_POPULATED:
                if ("populateRequested" in this) {
                    delete this.populateRequested;
                    this.PopulateDataResponse(sender.GetData(), event);
                }
                break;
            case Jane.events.DATA_CHANGED:
                if (this.HasData()) {
                    this.PopulateDataResponse(sender.GetData(), event);
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
    DataObjectReference.PopulateData = function () {
        var sourceData = this.source.GetData();
        if (sourceData != null) {
            this.PopulateDataResponse(sourceData, Jane.events.DATA_POPULATED);
        } else {
            this.populateRequested = true;
            this.source.Populate();
        }
    };
    DataObjectReference.HasMetaData = function () {
        return (base.HasMetaData.call (this)) ? true : this.source.HasMetaData ();
    };
    DataObjectReference.GetMetaData = function () {
        return (base.HasMetaData.call (this)) ? base.GetMetaData.call (this) : this.source.GetMetaData ();
    };
    return DataObjectReference;
}(Jane.DataObject);
Jane.DataObjectEspace = function (base) {
    var DataObjectEspace = Object.create(base);
    DataObjectEspace.Init = function (params) {
        params["name"] = params.resultSetName;
        base.Init.call(this, params);
        if ("resultSetUrl" in params) this["dataUrl"] = params["resultSetUrl"];
        if ("cdmMapUrl" in params) this["metaDataUrl"] = params["cdmMapUrl"];
        if ("dataSourceName" in params) this["metaDataName"] = params["dataSourceName"];
        if ("numRows" in params) this["recordCount"] = params["numRows"];
        this.PopulateMetaData();
        return this;
    };
    DataObjectEspace.PopulateMetaData = function () {
        if (!this.HasMetaData()) {
            var parser = document.createElement('a');
            parser.href = this.dataUrl;
            var metaDataUrl = parser.protocol + "//" + parser.host + "/espace/rest/data/sources/" + this.metaDataName;
            var scope = this;
            $.getJSON(metaDataUrl, function (metaData) {
                scope.PopulateMetaDataResponse(metaData);
            });
        }
    };
    DataObjectEspace.PopulateMetaDataResponse = function (espaceMetaData) {
        console.log (this.name + " populate metaData");
        this.espaceMetaData = espaceMetaData;
        var fields = espaceMetaData.columns;
        for (var i = 0, count = fields.length; i < count; ++i) {
            var field = fields[i];
            this.AddFieldMetaData(field.accessName, field.displayName, field.types[0], field.tags);
        }
        this.ValidateMetaData();
        if ("populateRequested" in this) {
            delete this.populateRequested;
            this.PopulateData();
        }
    };
    DataObjectEspace.PopulateEspaceFields = function (rows) {
        var espaceMetaData = this.espaceMetaData;
        var geoColumns = espaceMetaData.geoColumns;
        for (var i = 0, iCount = geoColumns.length; i < iCount; ++i) {
            var geoColumn = geoColumns[i];
            var findDisplaybyAccessName = function (accessName) {
                for (var k = 0, kCount = espaceMetaData.columns.length; k < kCount; ++k) {
                    var column = espaceMetaData.columns[k];
                    if (column.accessName == accessName) {
                        return column.displayName;
                    }
                }
            };
            var fieldName = findDisplaybyAccessName(geoColumn.geoColumn);
            var lonFieldName = findDisplaybyAccessName(geoColumn.lonColumn);
            var latFieldName = findDisplaybyAccessName(geoColumn.latColumn);
            for (var j = 0, jCount = rows.length; j < jCount; ++j) {
                var rowData = rows[j].data;
                rowData[fieldName] = { "longitude": rowData[lonFieldName], "latitude": rowData[latFieldName] };
            }
        }
    };
    DataObjectEspace.PopulateData = function () {
        if (this.HasMetaData()) {
            var scope = this;
            $.getJSON(this.dataUrl, function (data) {
                scope.PopulateEspaceFields(data.rows);
                scope.PopulateDataResponse(data.rows, Jane.events.DATA_POPULATED);
            });
        } else {
            this.populateRequested = true;
        }
    };
    return DataObjectEspace;
}(Jane.DataObject);
Jane.TransformFlatten = function (base) {
    var TransformFlatten = Object.create(base, {
        "name": {
            "value": "Flatten",
            "enumerable": true,
            "writable": false
        }
    });
    TransformFlatten.EnumerateRecord = function (record, into) {
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
        var into = Object.create(null);
        this.EnumerateRecord(record, into);
        return into;
    };
    return TransformFlatten;
}(null);
Jane.TransformExtract = function (base) {
    var TransformExtract = Object.create(base, {
        "name": {
            "value": "Extract",
            "enumerable": true,
            "writable": false
        },
        "extract": {
            "value": "xxx",
            "enumerable": true,
            "writable": true
        }
    });
    TransformExtract.Init = function (params) {
        if ("extract" in params) this["extract"] = params["extract"];
        return this;
    };
    TransformExtract.HandleRecord = function (record, readOnly) {
        if (this.extract in record) {
            return readOnly ? record[this.extract] : Object.create(record[this.extract]);
        } else {
            console.log ("Can't extract '" + this.extract + "' from record");
            debugger;
        }
        return record;
    };
    return TransformExtract;
}(null);
Jane.TransformCompound = function (base) {
    var TransformCompound = Object.create(base, {
        "name": {
            "value": "Compound",
            "enumerable": true,
            "writable": false
        }
    });
    TransformCompound.Init = function (params) {
        if ("transforms" in params) this["transforms"] = params["transforms"];
        return this;
    };
    TransformCompound.HandleRecord = function (record, readOnly) {
        for (var i = 0, count = this.transforms.length; i < count; ++i) {
            record = this.transforms[i].HandleRecord(record, readOnly);
        }
        return record;
    };
    return TransformCompound;
}(null);
