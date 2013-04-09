// JavaScript source code
function StartApp() {
    // the event logger object
    var testLogger = Object.create(null);
    testLogger.name = "TestLogger";
    testLogger.ReceiveEvent = function (sender, event) {
        console.log(this.name + " receives " + event + " from " + sender.name);
    };
    Jane.Subscribe(testLogger, testLogger.ReceiveEvent);

    // create the source data reference
    var jsonJdr = Object.create(Jane.DataReferenceEspace).Init({
        "resultSetName" : "Espace Time Data",
        "numRows" :82,
        "resultSetUrl" : "https://localhost:8443/bwade/JavaScript/Jane/site/data/timeData.json",
        "cdmMapUrl" : "/bwade/JavaScript/Jane/site/data/timeData-metaData.json"
    });
    jsonJdr.SubscribeReadOnly(testLogger, testLogger.ReceiveEvent);

    // create and configure a copy of the source
    var copyJdr1 = Object.create(Jane.DataReferenceCopy).Init({
        name: "Copy 1",
        source: jsonJdr
    });
    copyJdr1.select = ["CardHolder", "Date"];
    copyJdr1.filter = {
        HandleRecord: function (record) {
            return (record["Devices"] == "South Rear Door") && (record["CardHolder"] != "(System)");
        }
    };
    copyJdr1.transform = Jane.TransformFlatten;
    copyJdr1.SubscribeReadOnly(testLogger, testLogger.ReceiveEvent);


    // create and configure a copy of the copy
    var copyJdr2 = Object.create(Jane.DataReferenceCopy).Init({
        name: "Copy 2",
        source: copyJdr1
    });
    copyJdr2.sort = [
        { name: "CardHolder", asc: false },
        { name: "Date", asc: true }
    ];
    copyJdr2.Subscribe(testLogger, testLogger.ReceiveEvent, { "X" : "X" });
}
