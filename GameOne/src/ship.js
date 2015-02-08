var Ship = function () {
    var _ = Object.create(Cluster);

    _.learn = function () {
        var substepCount = Cluster.subStepCount;
        Cluster.subStepCount = 20;

        this.reset (Vector2d.zero (), 0);
        var spinVelocity = this.spinVelocity;
        var accumulator = 0;
        var accumulatorCount = 0;
        var scope = this;
        var report = function () {
            scope.update (deltaTime);
            scope.updateFrameOfReference();
            var spinAcceleration = Math.abs (scope.spinVelocity - spinVelocity) / deltaTime;
            spinVelocity = scope.spinVelocity;
            console.log ("Velocity: " + spinVelocity.toPrecision (5) + ", Spin Acceleration: " + spinAcceleration.toPrecision (5) + "/frame");
            accumulator += spinAcceleration;
            ++accumulatorCount;
            return spinAcceleration;
        }

        report ();

        this.thrust (-1, 1);
        report ();
        report ();
        report ();
        report ();

        this.thrust (-1, 1);
        report ();
        report ();
        report ();
        report ();

        this.thrust (1, -1);
        report ();
        report ();
        report ();
        report ();

        this.spinAcceleration = accumulator / accumulatorCount;
        console.log ("Spin Acceleration: " + this.spinAcceleration);

        Cluster.subStepCount = substepCount;
    }

    _.init = function (name, position, spinPosition) {
        // do the parental thing
        Object.getPrototypeOf(Ship).init.call(this, name, Vector2d.zero (), 0);

        this.thrustRatio = 16.0;
        this.rotateRatio = 5.0;

        this.learn ();
        this.reset (position, spinPosition);

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
        var targetSpinPosition = Math.atan2 (direction.y, direction.x);
        var deltaSpinPosition = targetSpinPosition - this.spinPosition
    	while (deltaSpinPosition > Math.PI) {
            deltaSpinPosition -= (Math.PI * 2.0);
        }
        while (deltaSpinPosition < -Math.PI) {
            deltaSpinPosition += (Math.PI * 2.0);
        }
        var deltaSpinPositionMagnitude = Math.abs (deltaSpinPosition);

        // compute the desired change in rotational velocity, and the thrust
        // associated with that, then apply it
        var timeToTargetSpinPosition = 0.15 * (1 + deltaSpinPositionMagnitude);
        var velocityToTargetSpinPosition = (deltaSpinPosition / timeToTargetSpinPosition);
        var deltaVelocityNeeded = velocityToTargetSpinPosition - this.spinVelocity;
        var thrustNeeded = deltaVelocityNeeded / this.spinAcceleration;
        var clampedThrust = Math.min(Math.max(thrustNeeded, -1.0), 1.0);
        this.thrust (-clampedThrust, clampedThrust);
        //console.log ("thrust: " + clampedThrust);

        // return how close the ship is to pointing in the right direction
        return deltaSpinPositionMagnitude;
    }

    _.pointAt = function (point) {
        var direction = point.subtract (this.position).normalized ();
        this.point (direction);
    }

    _.go = function (targetVelocity) {
        // compute the frame for calculations
        var speed = targetVelocity.norm ();
        //console.log ("Go speed = " + speed.toPrecision (5));
        var axis = targetVelocity.scale (1.0 / speed);
        var perp = axis.perpendicular ();

        // compute the velocity corrections needed, including a clamped axis
        // component so the ship never slows down, and a doubled perp component
        // to help catch the target vector quicker
        var axisComponent = Math.max (0.0, speed - (axis.dot (this.velocity)));
        var perpComponent = 2.0 * perp.dot (this.velocity);

        var pointDirection = axis.scale (axisComponent).add (perp.scale (-perpComponent));
        var deltaSpinPosition = this.point (pointDirection);

        // if we are pointing the right way (within a few degrees), let's go...
        var thrustLevel = 1.0 - (deltaSpinPosition / (Math.PI * 0.5));
        if (thrustLevel > 0) {
            thrustLevel *= thrustLevel;
            this.thrust (thrustLevel, thrustLevel);
        }
    }

    return _;
}();
