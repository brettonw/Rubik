var Ship = function () {
    var S = Object.create(Thing);

    S.init = function (name) {
        this.engines = Vector2d.zero();

        // do the parental thing
        return Object.getPrototypeOf(Ship).init.call(this, name);
    }

    S.thrust = function (percent) {
        var orientationVector = Vector2d.xy(Math.cos(this.spinPosition), Math.sin(this.spinPosition));
        var forceScale = this.mass * (percent / 100.0);
        var force = orientationVector.scale(forceScale);
        this.applyForce(force);
    }

    S.rotate = function (percent) {
        var torqueScale = this.moment * (percent / 100.0);
        this.applyTorque(torqueScale);
    }

    return S;
}();
