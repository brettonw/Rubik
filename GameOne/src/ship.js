var Ship = function () {
    var S = Object.create(Thing);

    S.init = function (name) {
        // do the parental thing
        Object.getPrototypeOf(Ship).init.call(this, name);

        this.engines = Vector2d.zero();
        this.thrustRatio = 1.0;
        this.rotateRatio = 5.0;

        return this;
    }

    S.thrust = function (percent) {
        var orientationVector = Vector2d.xy(Math.cos(this.spinPosition), Math.sin(this.spinPosition));
        var forceScale = this.mass * this.thrustRatio * (percent / 100.0);
        var force = orientationVector.scale(forceScale);
        this.applyForce(force);
    }

    S.rotate = function (percent) {
        var torqueScale = this.moment * this.rotateRatio * (percent / 100.0);
        this.applyTorque(torqueScale);
    }

    return S;
}();
