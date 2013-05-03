function RunApp() {
    BuildTree();

    // the event logger object
    var testLogger = Object.create(null);
    testLogger.name = "TestLogger2";
    testLogger.ReceiveEvent = function (sender, event) {
        DrawTree();
    };
    Jane.Subscribe(testLogger, testLogger.ReceiveEvent);
}
