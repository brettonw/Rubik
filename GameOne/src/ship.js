var Ship = function () {
    var _ = Object.create(Thing);

    _.init = function (name, position, spinPosition) {
        // do the parental thing
        Object.getPrototypeOf(Ship).init.call(this, name, position, spinPosition);

        this.thrustRatio = 10.0;
        this.rotateRatio = 10.0;

        return this;
    }

    _.thrust = function (percent) {
        var orientationVector = Vector2d.angle(this.spinPosition);
        var thrustVector = orientationVector.scale(this.thrustRatio * (percent / 100.0));
        this.applyAcceleration(thrustVector);
    }

    _.rotate = function (percent) {
        this.applySpinAcceleration(this.rotateRatio * (percent / 100.0));
    }

    return _;
}();
