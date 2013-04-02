// JavaScript source code
function StartApp() {
    var jsonJdr = Object.create(Jane.DataReferenceJson).Init({
        name: "Json JDR",
        url: "data/timeData.json"
    });

    var copyJdr = Object.create(Jane.DataReferenceCopy).Init({
        name: "Copy 1",
        source: jsonJdr
    });

    var testLogger = Object.create(null);
    testLogger.name = "TestLogger";
    testLogger.ReceiveEvent = function (sender, event) {
        console.log(this.name + " receives " + event + " from " + sender.name);
    };

    jsonJdr.SubscribeReadOnly (testLogger, testLogger.ReceiveEvent);
    copyJdr.SubscribeReadOnly(testLogger, testLogger.ReceiveEvent);

    jsonJdr.Populate();
}
