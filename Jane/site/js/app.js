var testLogger;

function RunApp() {
    // create the tree for display
    BuildTree();

    // the event logger object
    testLogger = Object.create(null);
    testLogger.name = "TestLogger";
    testLogger.ReceiveEvent = function (sender, event) {
        console.log(this.name + " receives " + event + " from " + sender.name);
        DrawTree();
        //debugger;
    };
    Jane.Subscribe(testLogger, testLogger.ReceiveEvent);

    // hardcode the result set
    var resultSet = {
        "resultSetName": "Restaurants",
        "resultSetId": "job@dee750d6-cf31-4d5f-a305-5972432ed18d",
        "state": "FINISHED",
        "numRows": 5525,
        "dataSourceName": "restaurants.json",
        "resultSetUrl": "https://localhost:8443/espace/rest/store/resultset/job@dee750d6-cf31-4d5f-a305-5972432ed18d.json",
        "cdmMapUrl": "/espace/rest/map/job@dee750d6-cf31-4d5f-a305-5972432ed18d",
        "lastAccessTime": 1365599182779,
        "submitTime": 1365599181025
    };

    // create the source data reference
    var espaceParam = Object.create(resultSet);
    espaceParam.transform = Object.create(Jane.TransformExtract).Init({ "extract": "data" });
    var espaceJdr = Object.create(Jane.DataObjectEspace).Init(espaceParam);
    espaceJdr.SubscribeReadOnly(testLogger, testLogger.ReceiveEvent);

    // create and configure a copy of the source
    var copyJdr1 = Object.create(Jane.DataObjectReference).Init({
        name: "Copy 1",
        source: espaceJdr,
        select: ["ID", "Name", "Address", "Geo Loc"],
        filter: {
            HandleRecord: function (record) {
                return (record["Current Grade"] == "A");
            }
        }
    });
    copyJdr1.SubscribeReadOnly(testLogger, testLogger.ReceiveEvent);
}

function AddCopy2() {
    var copyJdr1 = Jane.GetDataReference("Copy 1");

    // create and configure a sorted copy of the copy - the logger has a 
    // contract that will cause it to be an array of protos
    var copyJdr2 = Object.create(Jane.DataObjectReference).Init({
        name: "Copy 2",
        source: copyJdr1,
        sort: [{ name: "Name", asc: true }]
    });
    copyJdr2.Subscribe(testLogger, testLogger.ReceiveEvent, { "X": "X" });
}
