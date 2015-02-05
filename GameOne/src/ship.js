var Ship = function () {
    var _ = Object.create(Cluster);

    _.init = function (name, position, spinPosition) {
        // do the parental thing
        Object.getPrototypeOf(Ship).init.call(this, name, position, spinPosition);

        this.thrustRatio = 5.0;
        this.rotateRatio = 5.0;

        return this;
    }

    _.thrust = function (percent) {
        // compute the thrust vector
        var orientationVector = Vector2d.angle(this.spinPosition);
        var thrustVector = orientationVector.scale(this.thrustRatio * (percent / 100.0));

        // engines are assumed to be (left) particle 0, and (right) particle 1
        this.particles[0].applyAcceleration(thrustVector);
        this.particles[1].applyAcceleration(thrustVector);
    }

    _.rotate = function (percent) {
        // compute the thrust vector
        var orientationVector = Vector2d.angle(this.spinPosition);
        var thrustVector = orientationVector.scale(this.rotateRatio * (percent / 100.0));

        // engines are assumed to be (left) particle 0, and (right) particle 1
        this.particles[0].applyAcceleration(thrustVector);
        this.particles[1].applyAcceleration(thrustVector.scale (-1));
    }

    return _;
}();
