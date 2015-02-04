var Thing = function () {
    var T = Object.create(null);

    T.init = function (name) {
        this.name = name;

        // rotational parameters of a physical body in 2 dimensions, e.g. it can only 
        // rotate around an axis that is perpendicular to the 2D plane
        this.spinMass = 0.0;
        this.spinPosition = 0.0;
        this.spinVelocity = 0.0;
        this.spinForce = 0.0;
        this.spinDamping = -0.05;

        // translational parameters of a physical body in 2 dimensions
        this.mass = 0.0;
        this.position = Vector2d.zero();
        this.velocity = Vector2d.zero();
        this.force = Vector2d.zero();
        this.damping = -0.5;

        return this;
    }

    T.integrate = function (deltaTime) {
        // compute forces due to viscous damping
        this.applyAcceleration(this.velocity.scale(this.damping / deltaTime));
        this.applySpinAcceleration(this.spinVelocity * this.spinDamping / deltaTime);

        // compute accelerations from the forces, then clear out the forces
        var deltaVelocity = this.force.scale(deltaTime / this.mass);
        this.force = Vector2d.zero();
        var deltaSpinVelocity = this.spinForce * (deltaTime / this.spinMass);
        this.spinForce = 0.0;

        // using the midpoint method, compute the position changes
        this.position = this.position.add((deltaVelocity.scale(0.5).add(this.velocity)).scale(deltaTime));
        this.spinPosition = this.spinPosition + (((deltaSpinVelocity * 0.5) + this.spinVelocity) * deltaTime);

        // update the velocities from the deltas
        this.velocity = this.velocity.add(deltaVelocity);
        this.spinVelocity = this.spinVelocity + deltaSpinVelocity;
        if (Math.abs(this.spinVelocity) > 0) {
            var sgn = (this.spinVelocity / Math.abs(this.spinVelocity));
            this.spinVelocity = sgn * Math.min(5, Math.abs(this.spinVelocity));
        }

        // keep the spin position in a math friendly range
        var TWO_PI = Math.PI * 2;
        while (this.spinPosition >= TWO_PI)
            this.spinPosition -= TWO_PI;
        while (this.spinPosition < 0)
            this.spinPosition += TWO_PI;
    }

    T.applyForce = function (force) {
        this.force = this.force.add(force);
    }

    T.applyAcceleration = function (acceleration) {
        var force = acceleration.scale(this.mass);
        this.applyForce(force);
    }

    T.applySpinForce = function (spinForce) {
        this.spinForce += spinForce;
    }

    T.applySpinAcceleration = function (spinAcceleration) {
        var spinForce = spinAcceleration * this.spinMass;
        this.applySpinForce(spinForce);
    }

    // geometry is used for bounding the object, for collision detection, for drawing,
    // and for creating the visual representation, computed values assume a homo-
    // genous object with geometry centered (the CG is located at the origin)
    T.makeBallGeometry = function (container, radius) {
        // compute the mass and the spinMass
        this.mass = Math.PI * radius * radius;
        this.spinMass = (this.mass * radius * radius) / 2.0;

        this.svg = container.append("circle")
            .attr("stroke-width", 2.0 / scale)
            .attr("fill", "red")
            .attr("fill-opacity", "1.0")
            .attr("stroke", "black")
            .attr("stroke-opacity", "1.0")
            .attr("r", radius);
    };

    T.makePolygonGeometry = function (container, geometry) {
        var geometry = [
            Vector2d.xy(0.00, 0.00),
            Vector2d.xy(-0.05, 0.05),
            Vector2d.xy(0.10, 0.00),
            Vector2d.xy(-0.05, -0.05)
        ];
        var points = geometry[0].x + "," + geometry[0].y;
        for (var i = 1; i < geometry.length; ++i) {
            points += " " + geometry[i].x + "," + geometry[i].y;
        }
        this.svg = container.append("polygon")
        .attr("stroke-width", 2.0 / scale)
        .attr("fill", "red")
        .attr("fill-opacity", "1.0")
        .attr("stroke", "black")
        .attr("stroke-opacity", "1.0")
        .attr("stroke-linejoin", "round")
        .attr("points", points);

        // temporary values
        this.mass = 1.0; //Math.PI * radius * radius;
        this.spinMass = this.mass;//(this.mass * radius * radius) / 2.0;
    };

    T.update = function (deltaTime) {
        this.integrate(deltaTime);
        this.svg.attr("transform", "translate(" + this.position.x + "," + this.position.y + ") rotate(" + (this.spinPosition * (180.0 / Math.PI)) + ", 0, 0)");
    };

    return T;
}();
