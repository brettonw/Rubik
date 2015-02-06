var Ship = function () {
    var _ = Object.create(Cluster);

    _.init = function (name, position, spinPosition) {
        // do the parental thing
        Object.getPrototypeOf(Ship).init.call(this, name, position, spinPosition);

        this.thrustRatio = 16.0;
        this.rotateRatio = 5.0;

        return this;
    }

    _.thrust = function (left, right) {
        var orientationVector = Vector2d.angle(this.spinPosition);
        var leftThrustVector = orientationVector.scale(this.thrustRatio * left);
        var rightThrustVector = orientationVector.scale(this.thrustRatio * right);

        // engines are assumed to be (left) particle 0, and (right) particle 1
        this.particles[0].applyAcceleration(leftThrustVector);
        this.particles[1].applyAcceleration(rightThrustVector);
    }

    _.point = function (direction) {
        // compute the angle delta
        var pointPosition = Math.atan2 (direction.y, direction.x);
        var delta = pointPosition - this.spinPosition
    	while (delta > Math.PI)
            delta -= (Math.PI * 2.0);
        while (delta < -Math.PI)
            delta += (Math.PI * 2.0);

        // compute the desired change in rotational velocity, and the thrust
        // associated with that, then apply it
        var speed = 1.0;
        var deltaVelocity = speed * ((delta / deltaTime) - this.spinVelocity);
        var sign = (deltaVelocity < 0) ? -1 : ((deltaVelocity > 0) ? 1 : 0);
        var stiffness = 0.5;
        var thrust = stiffness * sign * Math.min (1.0, sign * deltaVelocity / Math.PI);
        this.thrust (-thrust, thrust);
        //console.log ("thrust: " + thrust);
    }

    _.pointAt = function (point) {
        var direction = point.subtract (this.position).normalized ();
        this.point (direction);
    }

    return _;
}();
