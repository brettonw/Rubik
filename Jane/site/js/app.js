// JavaScript source code
function StartApp() {
    // the event logger object
    var testLogger = Object.create(null);
    testLogger.name = "TestLogger";
    testLogger.ReceiveEvent = function (sender, event) {
        console.log(this.name + " receives " + event + " from " + sender.name);
        debugger;
    };
    Jane.Subscribe(testLogger, testLogger.ReceiveEvent);

    // create the source data reference
    var espaceJdr = Object.create(Jane.DataReferenceEspace).Init(
        {
            "resultSetName": "Restaurants",
            "resultSetId": "job@dee750d6-cf31-4d5f-a305-5972432ed18d",
            "state": "FINISHED",
            "numRows": 5525,
            "dataSourceName": "restaurants.json",
            "resultSetUrl": "https://localhost:8443/espace/rest/store/resultset/job@dee750d6-cf31-4d5f-a305-5972432ed18d.json",
            "cdmMapUrl": "/espace/rest/map/job@dee750d6-cf31-4d5f-a305-5972432ed18d",
            "lastAccessTime": 1365599182779,
            "submitTime": 1365599181025
        }
        /*
        {
        "resultSetName" : "Espace Time Data",
        "numRows" :82,
        "resultSetUrl" : "https://localhost:8443/bwade/JavaScript/Jane/site/data/timeData.json",
        "cdmMapUrl" : "/bwade/JavaScript/Jane/site/data/timeData-metaData.json"
        }
        */
    );
    espaceJdr.SubscribeReadOnly(testLogger, testLogger.ReceiveEvent);

    // create and configure a copy of the source
    var copyJdr1 = Object.create(Jane.DataReferenceCopy).Init({
        name: "Copy 1",
        source: espaceJdr
    });
    /*
    copyJdr1.select = ["CardHolder", "Date"];
    copyJdr1.filter = {
        HandleRecord: function (record) {
            return (record["Devices"] == "South Rear Door") && (record["CardHolder"] != "(System)");
        }
    };
    */
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
