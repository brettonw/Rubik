var Cluster = function () {
    var C = Object.create(null);

    var centroid = function (particles) {
        var c = particles[0].position.add (particles[1].position).add (particles[2].position);
        return c.scale (1.0 / 3.0);
    }

    var updateFrame = function (cluster) {
        // the frame of reference sets the position at the centroid of the
        // particle cluster
        cluster.position = centroid (cluster.particles);

        // the frame of reference is computed as the line between [0,1] and
        // [2,midpoint(0,1)], we might need to do a "polish" step on this in
        // case those lines aren't actually perpendicular in the simulation
        var midpoint = cluster.particles[0].position.add(cluster.particles[1].position).scale(0.5);
        var delta = cluster.particles[2].position.subtract(midpoint).normalized ();
        cluster.spinPosition = Math.atan2 (delta.y, delta.x);
    }

    C.init = function (name) {
        this.name = name;

        // the points to define the geometry
        var points = [ Vector2d.xy(-0.05, 0.05), Vector2d.xy(-0.05, -0.05), Vector2d.xy(0.10, 0.00) ];

        // create the particle list
        var particle = function (i) {
            var r = 0.01, d = 300;
            var p = Object.create(Particle).init (name + "-" + i, points[i], r, d);
            p.damping = 0;//-0.001;
            return p;
        }
        this.particles = [ particle (0), particle (1), particle (2) ];

        // create the constraint list
        var constrain = function (a, b) {
            return { "a":a, "b":b, "d":points[a].subtract (points[b]).norm () };
        }
        this.constraints = [ constrain(0, 1), constrain(1, 2), constrain(2, 0) ];

        // set the initial frame of reference
        updateFrame (this);

        return this;
    }

    C.makeGeometry = function (container) {
        // add the particles so I can see them
        this.particles[0].makeGeometry (container);
        this.particles[1].makeGeometry (container);
        this.particles[2].makeGeometry (container);

        // add a ghost of my intended shape
        var points = this.particles[0].position.toString () + " " +
                     this.particles[1].position.toString () + " " +
                     this.particles[2].position.toString ();
        this.svg = container.append("polygon")
        .attr("fill", "red")
        .attr("fill-opacity", "0.33")
        .attr("points", points);

        return this;
    };

    C.update = function (deltaTime) {
        var scope = this;
        var subStep = function (dT) {
            // integrate the particles
            scope.particles[0].update (dT);
            scope.particles[1].update (dT);
            scope.particles[2].update (dT);

            // resolve the constraints
            var resolve = function (c) {
                //debugger;
                var constraint = scope.constraints[c];
                var a = scope.particles[constraint.a];
                var b = scope.particles[constraint.b];
                var delta = a.position.subtract (b.position);
                var d = delta.normalize ();

                // compute the relative velocity damping to apply
                // XXX this should be relative to dt and mass, I think
                var relativeVelocity = a.velocity.subtract(b.velocity);
                var springVelocity = relativeVelocity.dot (delta);
                var velocityDampingForce = 0.5 * 0.5 * springVelocity * (a.mass + b.mass) / dT;

                // apply a spring force to make d be equal to constraint.d,
                // using Hooke's law
                var x = d - constraint.d;
                var k = 0.5;

                /*
                var xd = x * 0.5;
                var xv = xd * 0.5;
                var fx = ((a.mass * xv) / dT) * ((Math.abs(x / constraint.d) > 0.25) ? 2.0 : 1.0);
                */

                var Fspring = 0.5 * 0.5 * 0.5 * 2.0 * x * (a.mass + b.mass) / (deltaTime * deltaTime);
                var F = (k * x) + velocityDampingForce;
                a.applyForce (delta.scale (-F));
                b.applyForce (delta.scale (F))
            }
            resolve (0); resolve (1); resolve (2);
        }

        var subSteps = 3;
        var dT = deltaTime / subSteps;
        for (var i = 0; i < subSteps; ++i) {
            subStep (dT);
        }

        // update the frame of reference
        updateFrame (this);

        // update the ghost
        this.svg.attr("transform", "translate(" + this.position.x + "," + this.position.y + ") rotate(" + (this.spinPosition * (180.0 / Math.PI)) + ", 0, 0)");
    };

    return C;
}();
