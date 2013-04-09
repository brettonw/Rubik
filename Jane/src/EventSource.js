//------------------------------------------------------------------------------
// An EventSource is a base class implementing a subscription event model.
//
// object interface
//  - function: Subscribe (target, receiver, contract)
//  - function: Unsubscribe (target)
//  - function: PostEvent (event)
//
//  - property: name
//
// Event subscribers are expected to have a "name" property.  
//  - property: name
//
// Events are posted to the subscriber's receiver, which is expected to have a 
// signature resembling this:
//  - function: ReceiveEvent (sender, event)
//
//------------------------------------------------------------------------------

var EventSource = Object.create(null);

EventSource.Init = function (params) {
    // start by creating an empty subscription list
    this.subscriptions = [];

    // copy some parameters
    COPY_PARAM(name, params);

    return this;
};

EventSource.Subscribe = function (target, receiver) {
    // check if the target is already subscribed, if so - bail out
    if (this.subscriptions.indexOf(target) >= 0) {
        return null;
    }

    // add the subscription and return it
    DEBUGLOG(target.name + " subscribes to " + this.name);
    var subscription = { "target": target, "receiver": receiver };
    this.subscriptions.push(subscription);
    return subscription;
};

EventSource.Unsubscribe = function (target) {
    // the subscriptions array is examined to remove the target
    var i = this.subscriptions.indexOf(target);
    if (i >= 0) {
        this.subscriptions.splice(i, 1);
    }
};

EventSource.PostEvent = function (event) {
    for (var i = 0, count = this.subscriptions.length; i < count; ++i) {
        var subscription = this.subscriptions[i];
        DEBUGLOG(this.name + " posting " + event + " to " + subscription.target.name);
        subscription.receiver.apply(subscription.target, [this, event]);
    }
};

