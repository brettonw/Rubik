//------------------------------------------------------------------------------
// A Jane Data Reference (JDR) is a base class for data sources. It supports
// lazy population via RESTful interfaces, and sharing of the data. This base 
// class implements a contract-based subscription event model.
//
// object interface
//  - function: Subscribe (target, receiver, contract)
//  - function: SubscribeReadOnly (target, receiver)
//  - function: Unsubscribe (target)
//
//  - function: HasData ()
//  - function: GetData ()
//  - function: GetDataIsReadOnly ()
//  - function: GetDataFormat ();
//  - function: Populate ()
//  - function: Flush ()
//  - function: Refresh ()
//
//  - property: name
//  - property: metadata - implementation dependent
//  - property: allowFlushForSubscription
//
// global interface
//  + property: events
//      - DATA_POPULATED
//      - DATA_FLUSHED
//      - DATA_CHANGED
//
// Event subscribers are expected to have a "name" property.  
//  - property: name
//
// Events are posted to the subscriber's receiver, which is expected to have a 
// signature resembling this:
//  - function: ReceiveEvent (sender, event)
//
// A contract is a javascript object with names of values the subcriber can 
// modify. we assume that any modification is expected to be private data that
// would cause a collision.
// 
// NOTE - we assume that all sources will use the "_HIGHLIGHTED_" key to 
// indicate the highlighted status of a record.
//
// cached data is an object with three properties:
//  - property: data
//  - property: readOnly
//  - property: format
//  
//------------------------------------------------------------------------------

Jane.DataReference = Object.create(null);

Jane.DataReference.allowFlushForSubscription = false;

Jane.DataReference.Init = function(params) {
    // start by creating an empty subscription list, and an empty data cache
    this.subscriptions = [];
    
    // copy some parameters
    COPY_PARAM(name, params);
    COPY_PARAM(allowFlushForSubscription, params);

    return this;
};

Jane.DataReference.CanSubscribe = function (contract) {
    // check if the contract indicates the subscriber will modify the data
    if (Object.getOwnPropertyNames (contract).length > 0) {
        // the proposed contract is checked against all previously subscribed 
        // contracts to see if there is any conflict, return false if any is found
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

        // if the data has been populated, we can check to see if there is a 
        // conflict between the readOnly property and the contract
        if (this.HasData () AND this.chachedData.readOnly) {
            // we know there is no conflict with other contracts because we 
            // checked that above, but the data was already populated as a read-
            // only value. we *could* subscribe, but only if we flush first and 
            // repopulate with modifiable data - NOTE this is NOT the default 
            // behavior
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

Jane.DataReference.Subscribe = function(target, receiver, contract) {
    if (this.CanSubscribe (contract)) {
        // check if the target is already subscribed, if so - bail out
        for (var i = 0, count = this.subscriptions.length; i < count; ++i) {
            var subscription = this.subscriptions[i];
            if (target == subscription.target) {
                return false;
            }
        }

        // add the subscription
        DEBUGLOG (target.name + " subscribes to " + this.name);
        this.subscriptions.push ({ "target" : target, "receiver" : receiver, "contract" : contract});

        // if the data is already populated, send a populate event to this
        // receiver so it can join in the fun
        if (this.HasData ()) {
            receiver.apply (target, [Jane.events.DATA_POPULATED, this.cachedData]);
        }

        // report the success
        return true;
    }
    return false;
};

Jane.DataReference.SubscribeReadOnly = function(target, receiver) {
    return this.Subscribe (target, receiver, {});
};

Jane.DataReference.Unsubscribe = function(target) {
    // the subscriptions array is examined to remove the target
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
        DEBUGLOG (this.name + " posting " + event + " to " + subscription.target.name);
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
    // if the data is populated, return its real property
    if (this.HasData ()) { return this.cachedData.readOnly; }

    // otherwise look at all the contracts to see if any of them will modify
    // the data
    for (var i = 0, count = this.subscriptions.length; i < count; ++i) {
        var subscription = this.subscriptions[i];
        if (Object.getOwnPropertyNames (subscription.contract).length > 0) {
            return false;
        }
    }

    // nobody says this will be modified, it's read only
    return true;
};

Jane.DataReference.GetDataFormat = function() {
    // if the data is populated, return its real property
    if (this.HasData ()) { return this.cachedData.format; }

    // otherwise it's empty
    return Jane.formats.EMPTY;
};

Jane.DataReference.PopulateDataResponse = function (data, readOnly, format, event) {
    // whatever mechanism populates the data is descendant classes, it should
    // call this method. 
    this.cachedData = { "data" : data, "readOnly" : readOnly, "format" : format };
    this.PostEvent (event);
};

Jane.DataReference.PopulateData = function () {
    // internal method to be overridden by descendant classes, this method will
    // only ever be called if the data source is not populated already
};

Jane.DataReference.Populate = function () {
    if (NOT this.HasData ()) { this.PopulateData (); }
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

