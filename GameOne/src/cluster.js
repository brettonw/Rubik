var Cluster = function () {
    var _ = Object.create(null);

    // the points to define the geometry
    var points = [
        Vector2d.xy(-0.05, 0.05),
        Vector2d.xy(-0.05, -0.05),
        Vector2d.xy(0.10, 0.00)
    ];

    // how many polishing steps we want to take to allow the frame of reference
    // to be stabilized after applying forces
    var subStepCount = 3;

    var updateFrameOfReference = function (cluster) {
        // the frame of reference sets the position at the centroid of the
        // particle cluster
        var particles = cluster.particles;
        cluster.position = particles[0].position
            .add(particles[1].position)
            .add(particles[2].position)
            .scale(1.0 / 3.0);

        // the frame of reference is computed as the line between [0,1] and
        // [2,midpoint(0,1)], we might need to do a "polish" step on this in
        // case those lines aren't actually perpendicular in the simulation
        var midpoint = cluster.particles[0].position
            .add(cluster.particles[1].position)
            .scale(0.5);
        var xAxis = cluster.particles[2].position
            .subtract(midpoint).normalized();
        cluster.spinPosition = Math.atan2(xAxis.y, xAxis.x);

        // reset the particles to be where they are supposed to be, this is 
        // kind of cheating, but it resolves the drift problem that can only
        // be fixed by going to much higher simulation rates
        var yAxis = xAxis.perpendicular();
        var resetParticle = function (i) {
            particles[i].position = cluster.position
                .add(xAxis.scale(points[i].x))
                .add(yAxis.scale(points[i].y));
        }
        resetParticle(0);
        resetParticle(1);
        resetParticle(2);
    }

    _.init = function (name) {
        this.name = name;

        // create the particle list
        var particle = function (i) {
            var r = 0.01, d = 300;
            var p = Object.create(Particle).init(name + "-" + i, points[i], r, d);
            p.damping = 0;//-0.001;
            return p;
        }
        this.particles = [particle(0), particle(1), particle(2)];

        // create the constraint list
        var constrain = function (a, b) {
            return { "a": a, "b": b, "d": points[a].subtract(points[b]).norm() };
        }
        this.constraints = [constrain(0, 1), constrain(1, 2), constrain(2, 0)];

        // set the initial frame of reference
        updateFrameOfReference(this);

        return this;
    }

    _.makeGeometry = function (container) {
        // add the particles so I can see them
        this.particles[0].makeGeometry(container);
        this.particles[1].makeGeometry(container);
        this.particles[2].makeGeometry(container);

        // add a ghost of my intended shape
        var points = this.particles[0].position.toString() + " " +
                     this.particles[1].position.toString() + " " +
                     this.particles[2].position.toString();
        this.svg = container.append("polygon")
            .attr("fill", "red")
            .attr("fill-opacity", "0.33")
            .attr("points", points);

        return this;
    };

    _.update = function (deltaTime) {
        var scope = this;
        var subStep = function (dT) {
            // resolve the constraints
            var resolveConstraint = function (c) {
                var constraint = scope.constraints[c];
                var a = scope.particles[constraint.a];
                var b = scope.particles[constraint.b];
                var delta = a.position.subtract(b.position);
                var d = delta.normalize();

                // compute the relative velocity damping to apply
                // XXX this should be relative to dt and mass, I think
                var relativeVelocity = a.velocity.subtract(b.velocity);
                var springVelocity = relativeVelocity.dot(delta);
                var velocityDampingForce = 0.5 * 0.5 * springVelocity * (a.mass + b.mass) / dT;

                // compute a spring force to make d be equal to constraint.d,
                // using Hooke's law
                var x = d - constraint.d;
                var k = 1;
                var springForce = k * x;

                // apply the forces
                var F = springForce + velocityDampingForce;
                a.applyForce(delta.scale(-F));
                b.applyForce(delta.scale(F))
            }
            resolveConstraint(0);
            resolveConstraint(1);
            resolveConstraint(2);

            // integrate the particles
            scope.particles[0].update(dT);
            scope.particles[1].update(dT);
            scope.particles[2].update(dT);
        }

        var dT = deltaTime / subStepCount;
        for (var i = 0; i < subStepCount; ++i) {
            subStep(dT);
        }
    };

    _.paint = function () {
        // update the frame of reference
        updateFrameOfReference(this);

        // update the ghost
        this.svg.attr("transform", "translate(" + this.position.x + "," + this.position.y + ") rotate(" + (this.spinPosition * (180.0 / Math.PI)) + ", 0, 0)");

        // update the particles
        this.particles[0].paint();
        this.particles[1].paint();
        this.particles[2].paint();
    }

    return _;
}();
