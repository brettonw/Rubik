var Particle = function () {
    var P = Object.create(null);

    P.init = function (name, radius, density) {
        this.name = name;

        // parameters of a physical body in 2 dimensions
        this.radius = radius;
        this.mass = Math.PI * radius * radius * density;
        this.position = Vector2d.zero();
        this.velocity = Vector2d.zero();
        this.force = Vector2d.zero();
        this.damping = -0.5;

        return this;
    }

    P.integrate = function (deltaTime) {
        // compute force due to damping, this is computed on a frame by frame 
        // basis, as opposed to over some time period (like 1 second)
        this.applyAcceleration(this.velocity.scale(this.damping / deltaTime));

        // compute acceleration from the forces, then clear out the forces
        var deltaVelocity = this.force.scale(deltaTime / this.mass);
        this.force = Vector2d.zero();

        // using the midpoint method, compute the position change
        this.position = this.position.add((deltaVelocity.scale(0.5).add(this.velocity)).scale(deltaTime));

        // update the velocity from the delta
        this.velocity = this.velocity.add(deltaVelocity);
    }

    P.applyForce = function (force) {
        this.force = this.force.add(force);
    }

    P.applyAcceleration = function (acceleration) {
        var force = acceleration.scale(this.mass);
        this.applyForce(force);
    }

    // geometry is used for bounding the object, for collision detection, for drawing,
    // and for creating the visual representation, computed values assume a homo-
    // genous object with geometry centered (the CG is located at the origin)
    P.makeGeometry = function (container) {
        this.svg = container.append("circle")
            .attr("stroke-width", 2.0 / scale)
            .attr("fill", "red")
            .attr("fill-opacity", "1.0")
            .attr("stroke", "black")
            .attr("stroke-opacity", "1.0")
            .attr("r", this.radius);
    };

    P.update = function (deltaTime) {
        this.integrate(deltaTime);
        this.svg.attr("transform", "translate(" + this.position.x + "," + this.position.y + ")");
    };

    return P;
}();
