"use strict";
var Vector2d = function () {
    var _ = Object.create(null);
    var makeVector = function (x, y) {
        var vector = Object.create(_);
        vector.x = x;
        vector.y = y;
        return vector;
    };
    _.make = function (a) { return makeVector(a[0], a[1]); };
    _.xy = function (x, y) { return makeVector(x, y); };
    _.v = function (v) { return makeVector(v.x, v.y); };
    _.angle = function (a) { return makeVector(Math.cos(a), Math.sin(a)); };
    _.zero = function () { return makeVector(0, 0); };
    _.one = function () { return makeVector(1, 1); };
    _.add = function (b) { return makeVector(this.x + b.x, this.y + b.y); };
    _.subtract = function (b) { return makeVector(this.x - b.x, this.y - b.y); };
    _.scale = function (b) { return makeVector(this.x * b, this.y * b); };
    _.dot = function (b) { return (this.x * b.x) + (this.y * b.y); };
    _.cross = function (b) { return (this.x * b.y) - (this.y * b.x); };
    _.normSq = function () { return this.dot(this); };
    _.norm = function () { return Math.sqrt(this.normSq()); };
    _.normalize = function () { var norm = this.norm (); this.copy (this.scale(1.0 / norm)); return norm; }
    _.normalized = function () { return this.scale(1.0 / this.norm()); }
    _.perpendicular = function () { return makeVector(-this.y, this.x); }
    _.toString = function () { return this.x + "," + this.y; }
    _.copy = function (v) { this.x = v.x; this.y = v.y; }
    return _;
}();
var Particle = function () {
    var _ = Object.create(null);
    _.init = function (name, position, radius, density) {
        this.name = name;
        this.radius = radius;
        this.mass = Math.PI * radius * radius * density;
        this.position = position;
        this.velocity = Vector2d.zero();
        this.force = Vector2d.zero();
        this.damping = -0.1;
        return this;
    }
    _.applyForce = function (force) {
        this.force = this.force.add(force);
    }
    _.applyAcceleration = function (acceleration) {
        var force = acceleration.scale(this.mass);
        this.applyForce(force);
    }
    _.makeGeometry = function (container) {
        this.svg = container.append("circle")
            .attr("stroke-width", 2.0 / scale)
            .attr("fill", "red")
            .attr("fill-opacity", "1.0")
            .attr("stroke", "black")
            .attr("stroke-opacity", "1.0")
            .attr("r", this.radius);
        return this;
    };
    _.update = function (deltaTime) {
        this.applyAcceleration(this.velocity.scale(this.damping / deltaTime));
        var deltaVelocity = this.force.scale(deltaTime / this.mass);
        this.force = Vector2d.zero();
        this.position = this.position.add((deltaVelocity.scale(0.5).add(this.velocity)).scale(deltaTime));
        this.velocity = this.velocity.add(deltaVelocity);
    };
    _.paint = function () {
        this.svg.attr("transform", "translate(" + this.position.x + "," + this.position.y + ")");
    }
    return _;
}();
var Cluster = function () {
    var _ = Object.create(null);
    var points = [
        Vector2d.xy(-0.05, 0.05),
        Vector2d.xy(-0.05, -0.05),
        Vector2d.xy(0.10, 0.00)
    ];
    var updateFrame = function (cluster) {
        var particles = cluster.particles;
        cluster.position = particles[0].position
            .add(particles[1].position)
            .add(particles[2].position)
            .scale(1.0 / 3.0);
        var midpoint = cluster.particles[0].position
            .add(cluster.particles[1].position)
            .scale(0.5);
        var xAxis = cluster.particles[2].position
            .subtract(midpoint).normalized();
        cluster.spinPosition = Math.atan2(xAxis.y, xAxis.x);
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
        var particle = function (i) {
            var r = 0.01, d = 300;
            var p = Object.create(Particle).init(name + "-" + i, points[i], r, d);
            p.damping = 0;
            return p;
        }
        this.particles = [particle(0), particle(1), particle(2)];
        var constrain = function (a, b) {
            return { "a": a, "b": b, "d": points[a].subtract(points[b]).norm() };
        }
        this.constraints = [constrain(0, 1), constrain(1, 2), constrain(2, 0)];
        this.subStepCount = 3;
        updateFrame(this);
        return this;
    }
    _.makeGeometry = function (container) {
        this.particles[0].makeGeometry(container);
        this.particles[1].makeGeometry(container);
        this.particles[2].makeGeometry(container);
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
            var resolveConstraint = function (c) {
                var constraint = scope.constraints[c];
                var a = scope.particles[constraint.a];
                var b = scope.particles[constraint.b];
                var delta = a.position.subtract(b.position);
                var d = delta.normalize();
                var relativeVelocity = a.velocity.subtract(b.velocity);
                var springVelocity = relativeVelocity.dot(delta);
                var velocityDampingForce = 0.5 * 0.5 * springVelocity * (a.mass + b.mass) / dT;
                var x = d - constraint.d;
                var k = 1;
                var springForce = k * x;
                var F = springForce + velocityDampingForce;
                a.applyForce(delta.scale(-F));
                b.applyForce(delta.scale(F))
            }
            resolveConstraint(0);
            resolveConstraint(1);
            resolveConstraint(2);
            scope.particles[0].update(dT);
            scope.particles[1].update(dT);
            scope.particles[2].update(dT);
        }
        var dT = deltaTime / this.subStepCount;
        for (var i = 0; i < this.subStepCount; ++i) {
            subStep(dT);
        }
    };
    _.paint = function () {
        updateFrame(this);
        this.svg.attr("transform", "translate(" + this.position.x + "," + this.position.y + ") rotate(" + (this.spinPosition * (180.0 / Math.PI)) + ", 0, 0)");
        this.particles[0].paint();
        this.particles[1].paint();
        this.particles[2].paint();
    }
    return _;
}();
var Thing = function () {
    var _ = Object.create(Particle);
    _.init = function (name, position, spinPosition) {
        Object.getPrototypeOf(Thing).init.call(this, name, position, 1.0, 1.0);
        this.spinMass = this.mass;
        this.spinPosition = spinPosition;
        this.spinVelocity = 0.0;
        this.spinForce = 0.0;
        this.spinDamping = -0.05;
        return this;
    }
    _.integrate = function (deltaTime) {
    }
    _.applySpinForce = function (spinForce) {
        this.spinForce += spinForce;
    }
    _.applySpinAcceleration = function (spinAcceleration) {
        var spinForce = spinAcceleration * this.spinMass;
        this.applySpinForce(spinForce);
    }
    _.makeGeometry = function (container) {
        var geometry = [
            Vector2d.xy(0.00, 0.00),
            Vector2d.xy(-0.05, 0.05),
            Vector2d.xy(0.10, 0.00),
            Vector2d.xy(-0.05, -0.05)
        ];
        var points = geometry[0].toString ();
        for (var i = 1; i < geometry.length; ++i) {
            points += " " + geometry[i].toString ();
        }
        this.svg = container.append("polygon")
        .attr("stroke-width", 2.0 / scale)
        .attr("fill", "red")
        .attr("fill-opacity", "1.0")
        .attr("stroke", "black")
        .attr("stroke-opacity", "1.0")
        .attr("stroke-linejoin", "round")
        .attr("points", points);
        return this;
    };
    _.update = function (deltaTime) {
        Object.getPrototypeOf(Thing).update.call(this, deltaTime);
        this.applySpinAcceleration(this.spinVelocity * this.spinDamping / deltaTime);
        var deltaSpinVelocity = this.spinForce * (deltaTime / this.spinMass);
        this.spinForce = 0.0;
        this.spinPosition = this.spinPosition + (((deltaSpinVelocity * 0.5) + this.spinVelocity) * deltaTime);
        this.spinVelocity = this.spinVelocity + deltaSpinVelocity;
        var TWO_PI = Math.PI * 2;
        while (this.spinPosition >= TWO_PI)
            this.spinPosition -= TWO_PI;
        while (this.spinPosition < 0)
            this.spinPosition += TWO_PI;
    };
    _.paint = function () {
        this.svg.attr("transform", "translate(" + this.position.x + "," + this.position.y + ") rotate(" + (this.spinPosition * (180.0 / Math.PI)) + ", 0, 0)");
    }
    return _;
}();
var Ship = function () {
    var _ = Object.create(Thing);
    _.init = function (name, position, spinPosition) {
        Object.getPrototypeOf(Ship).init.call(this, name, position, spinPosition);
        this.thrustRatio = 10.0;
        this.rotateRatio = 10.0;
        return this;
    }
    _.thrust = function (percent) {
        var orientationVector = Vector2d.angle(this.spinPosition);
        var thrustVector = orientationVector.scale(this.thrustRatio * (percent / 100.0));
        this.applyAcceleration(thrustVector);
    }
    _.rotate = function (percent) {
        this.applySpinAcceleration(this.rotateRatio * (percent / 100.0));
    }
    return _;
}();
var scale = 1.0;
var leftkeydown = false;
var upkeydown = false;
var rightkeydown = false;
var downkeydown = false;
function initPage() {
    var body = document.body;
    body.onkeydown = function () {
        if (event.keyCode == 37) leftkeydown = true;
        if (event.keyCode == 38) upkeydown = true;
        if (event.keyCode == 39) rightkeydown = true;
        if (event.keyCode == 40) downkeydown = true;
    }
    body.onkeyup = function () {
        if (event.keyCode == 37) leftkeydown = false;
        if (event.keyCode == 38) upkeydown = false;
        if (event.keyCode == 39) rightkeydown = false;
        if (event.keyCode == 40) downkeydown = false;
    }
    var target = d3.select("#display");
    var svg = target.append("svg").attr("class", "gameDisplay");
    svg.style("opacity", 1.0e-6)
        .transition().duration(1000)
        .style("opacity", 1.0);
    svg.append("rect")
        .attr("class", "gameBackground")
        .attr("width", "100%")
        .attr("height", "100%");
    var child = svg.append("g").attr("class", "gameDisplay");
    svg.call (d3.behavior.zoom()
        .translate ([0, 0])
        .scale (1.0)
        .scaleExtent([0.125, 8.0])
        .on("zoom", function() {
            child
                .attr("transform",
                    "translate(" + d3.event.translate[0] + "," + d3.event.translate[1] + ") " +
                    "scale(" + d3.event.scale + ")"
                );
        })
    );
    svg = child.append("g").attr("class", "gameDisplay");
    var xScale = target[0][0].clientWidth;
    var yScale = target[0][0].clientHeight;
    scale = Math.min (xScale, yScale);
    svg.attr ("transform", "translate(" + (xScale / 2.0) + "," + (yScale / 2.0) + ") scale(" + scale + "," + -scale + ")");
    svg = svg.append("g").attr("class", "gameDisplay");
    var gridLines = [0.0];
    var gridMin;
    var gridMax;
    for (var i = 1; i < 25; ++i) {
        var value = 0.05 * i;
        gridMin = -value;
        gridMax = value
        gridLines = [gridMin].concat (gridLines).concat ([gridMax]);
    }
    svg.selectAll(".xTicks")
        .data(gridLines)
        .enter().append("line").attr("class", "xTicks")
        .attr("x1", function(d) { return d; })
        .attr("y1", gridMin)
        .attr("x2", function(d) { return d; })
        .attr("y2", gridMax)
        .attr("stroke", "rgba(0, 0, 0, 0.20)")
        .attr("stroke-width", 1 / scale);
    svg.selectAll(".yTicks")
        .data(gridLines)
        .enter().append("line").attr("class", "yTicks")
        .attr("x1", gridMin)
        .attr("y1", function(d) { return d; })
        .attr("x2", gridMax)
        .attr("y2", function(d) { return d; })
        .attr("stroke", "rgba(0, 0, 0, 0.20)")
        .attr("stroke-width", 1 / scale);
    var ship = Object.create (Cluster).init ("Ship 1").makeGeometry(svg);
    ship
    var deltaTime = 1.0 / 20.0;
    var gametimer = setInterval(function () {
        var o = Vector2d.angle(ship.spinPosition);
        if (upkeydown) {
            ship.particles[0].applyAcceleration(o);
            ship.particles[1].applyAcceleration(o);
        }
        if (downkeydown) {
            o = o.scale(-0.5);
            ship.particles[0].applyAcceleration(o);
            ship.particles[1].applyAcceleration(o);
        }
        if (rightkeydown) {
            ship.particles[0].applyAcceleration(o);
            ship.particles[1].applyAcceleration(o.scale(-1));
        }
        if (leftkeydown) {
            ship.particles[0].applyAcceleration(o.scale(-1));
            ship.particles[1].applyAcceleration(o);
        }
        ship.update(deltaTime);
        ship.paint();
    }, 1000 * deltaTime);
}
