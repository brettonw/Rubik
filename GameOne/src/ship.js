var Ship = function () {
    var S = Object.create(Thing);

    S.init = function (name, position, spinPosition) {
        // do the parental thing
        Object.getPrototypeOf(Ship).init.call(this, name, position, spinPosition);

        this.thrustRatio = 10.0;
        this.rotateRatio = 10.0;

        return this;
    }

    S.thrust = function (percent) {
        var orientationVector = Vector2d.angle(this.spinPosition);
        var thrustVector = orientationVector.scale(this.thrustRatio * (percent / 100.0));
        this.applyAcceleration(thrustVector);
    }

    S.rotate = function (percent) {
        this.applySpinAcceleration(this.rotateRatio * (percent / 100.0));
    }

    return S;
}();
