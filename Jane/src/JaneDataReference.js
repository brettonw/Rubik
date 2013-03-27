//------------------------------------------------------------------------------
// A Jane Data Reference (JDR) is a base class for data sources. It supports
// lazy population via RESTful interfaces, and sharing of the data. This base 
// class implements a contract-based subscription event model.
//
// public interfaces
//  - function: Subscribe (target, callback, contract)
//  - function: SubscribeReadOnly (target, callback)
//  - function: Unsubscribe (target)
//
//  - function: HasData ()
//  - function: Populate ()
//  - function: GetData ()
//  - function: Flush ()
//  - function: Refresh ()
//
//  - property: name
//  - property: readOnly - set when the data is populated
//  - property: metadata - implementation dependent
//  - property: allowFlushForSubscription
//
// Events are posted to the subscriber's callback, which is expected to have a
// signature resembling this:
//  - function: ReceiveEvent (event, data)
//
// A contract is a javascript object with names of values the subcriber can 
// modify. we assume that any modification is expected to be private data that
// would cause a collision.
// 
// NOTE - we assume that all sources will use the "_HIGHLIGHTED_" key to 
// indicate the highlighted status of a record.
//  
//------------------------------------------------------------------------------

var JaneDataReference = Object.create(null);

JaneDataReference.allowFlushForSubscription = false;

JaneDataReference.Init = function(params) {
    // start by creating an empty subscription list, and an empty data cache
    this.subscriptions = [];
    
    // copy some parameters
    COPY_PARAM(name, params);
    COPY_PARAM(allowFlushForSubscription, params);

    return this;
};

JaneDataReference.CanSubscribe = function (contract) {
    // the proposed contract is checked against all previously subscribed 
    // contracts to see if there is any conflict, return false if any is found
    for (var contractElement in contract) {
        if (contract.hasOwnProperty (contractElement)) {
            for (var i = 0, count = this.subscriptions.count; i < count; ++i) {
                var subscription = this.subscriptions[i];
                if (contractElement in subscription.contract) {
                    return false;
                }
            }
        }
    }

    // if the data has been populated, the readOnly property of the object 
    // should be set appropriately. we check to see if there is any kind of 
    // conflict there
    if ("readOnly" in this) {
        if (this.readOnly AND (Object.getOwnPropertyNames (contract).length > 0)) {
            // there is no contract conflict, but the data was already populated
            // with a read only flag. we *could* subscribe, but only if we flush 
            // first and repopulate with modifiable data - NOTE this is NOT the 
            // default behavior
            if (this.allowFlushForSubscription) {
                this.Flush ();
            } else {
                return false;
            }
        }
    }

    // no conflict found
    return true;
};

JaneDataReference.Subscribe = function(target, callback, contract) {
    if (this.CanSubscribe (contract)) {
        subscriptions.push ({ "target" : target, "callback" : callback, "contract" : contract});
        return true;
    }
    return false;
};

JaneDataReference.SubscribeReadOnly = function(target, callback) {
    return this.Subscribe (target, callback, {});
};

JaneDataReference.Unsubscribe = function(target) {
    // the subscriptions array is examined to remove the target
    for (var i = 0, count = this.subscriptions.count; i < count; ++i) {
        var subscription = this.subscriptions[i];
        if (target == subscription.target) {
            subscriptions.splice (i, 1);
            return;
        }
    }
};

JaneDataReference.IsReadOnly = function() {
    // first check to see if the property has been set (which should happen when
    // the data is populated)
    if ("readOnly" in this) { return this.readOnly; }

    // otherwise look at all the contracts to see if any of them will modify
    // the data sets
    for (var i = 0, count = this.subscriptions.count; i < count; ++i) {
        var subscription = this.subscriptions[i];
        if (Object.getOwnPropertyNames (subscription.contract).length > 0) {
            return false;
        }
    }

    // nobody says this will be modified, 
    return true;
};

JaneDataReference.PostEvent = function (event) {
    for (var i = 0, count = this.subscriptions.count; i < count; ++i) {
        var subscription = this.subscriptions[i];
        subscription.callback.apply (subscription.target, [event, this.cachedData]);
    }
};

JaneDataReference.PopulateDataResponse = function (data, readOnly) {
    // whatever mechanism populates the data is descendant classes, it should
    // call this method
    this.cachedData = data;
    this.readOnly = readOnly;
    this.PostEvent (this.events.DATA_POPULATED);
};

JaneDataReference.HasData = function () {
    return ("cachedData" in this);
};

JaneDataReference.PopulateData = function () {
    // internal method to be overridden by descendant classes, this method will
    // only ever be called if the data source is not populated already
};

JaneDataReference.Populate = function () {
    if (NOT ("cachedData") in this) { this.PopulateData (); }
};

JaneDataReference.GetData = function () {
    return ("cachedData" in this) ? this.cachedData : null;
};

JaneDataReference.Flush = function () {
    if ("cachedData" in this) { delete this.cachedData; }
    if ("readOnly" in this) { delete this.readOnly; }
    this.PostEvent (this.events.DATA_FLUSHED);
};

JaneDataReference.Refresh = function () {
    this.Flush ();
    this.Populate ();
};

JaneDataReference.events = {
    DEFINE_AS(DATA_POPULATED),
    DEFINE_AS(DATA_FLUSHED),
    //DEFINE_AS(ROW_HIGHLIGHT_CHANGED),
    DEFINE_AS(HIGHLIGHT_CHANGED),
    DEFINE_AS(SELECTION_CHANGED)
};
