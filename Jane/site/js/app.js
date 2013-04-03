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
    /*
CardHolder: "Allen, Mark (Extra)"
CardNo: "31085"
Computer, Port, Panel: "PC-65, Com: 10, Panel #1"
Date: "11-23-2012 07:38:11 am"
Devices: "South Rear Door"
Transactions: "Access Granted"
    */
    copyJdr.select = ["CardHolder", "Date"];
    copyJdr.filter = {
        HandleRecord: function (record) {
            return (record["Devices"] == "South Rear Door") && (record["CardHolder"] != "(System)");
        }
    };
    copyJdr.sort = [{ name: "CardHolder", asc: false }];

    var testLogger = Object.create(null);
    testLogger.name = "TestLogger";
    testLogger.ReceiveEvent = function (sender, event) {
        console.log(this.name + " receives " + event + " from " + sender.name);
    };

    jsonJdr.SubscribeReadOnly (testLogger, testLogger.ReceiveEvent);
    copyJdr.Subscribe(testLogger, testLogger.ReceiveEvent, { "X" : "X" });

    copyJdr.Populate();
}
