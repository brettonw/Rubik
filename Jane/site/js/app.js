// JavaScript source code
function StartApp() {
    var testLogger = Object.create(null);
    testLogger.name = "TestLogger";
    testLogger.ReceiveEvent = function (sender, event) {
        console.log(this.name + " receives " + event + " from " + sender.name);
    };

    var jsonJdr = Object.create(Jane.DataReferenceJson).Init({
        name: "Json JDR",
        url: "data/timeData.json",
        metaDataUrl: "data/timeData-metaData.json"
    });

    var copyJdr = Object.create(Jane.DataReferenceCopy).Init({
        name: "Copy 1",
        source: jsonJdr
    });
    copyJdr.select = ["CardHolder", "Date"];
    copyJdr.filter = {
        HandleRecord: function (record) {
            return (record["Devices"] == "South Rear Door") && (record["CardHolder"] != "(System)");
        }
    };
    copyJdr.sort = [
        { name: "CardHolder", type: "String", asc: false },
        { name: "Date", type: "Date", asc: true }
    ];

    jsonJdr.SubscribeReadOnly (testLogger, testLogger.ReceiveEvent);
    copyJdr.Subscribe(testLogger, testLogger.ReceiveEvent, { "X" : "X" });

    copyJdr.Populate();
}
