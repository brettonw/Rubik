"use strict";
var Vector2d = function () {
    var v2d = Object.create(null);
    var makeVector = function (x, y) {
        var vector = Object.create(v2d);
        vector.x = x;
        vector.y = y;
        return vector;
    };
    v2d.make = function (a) { return makeVector(a[0], a[1]); };
    v2d.xy = function (x, y) { return makeVector(x, y); };
    v2d.v = function (v) { return makeVector(v.x, v.y); };
    v2d.zero = function () { return makeVector(0, 0); };
    v2d.one = function () { return makeVector(1, 1); };
    v2d.add = function (b) { return makeVector(this.x + b.x, this.y + b.y); };
    v2d.subtract = function (b) { return makeVector(this.x - b.x, this.y - b.y); };
    v2d.scale = function (b) { return makeVector(this.x * b, this.y * b); };
    v2d.dot = function (b) { return (this.x * b.x) + (this.y * b.y); };
    v2d.cross = function (b) { return (this.x * b.y) - (this.y * b.x); };
    v2d.normSq = function () { return this.dot(this); };
    v2d.norm = function () { return Math.sqrt(this.normSq()); };
    v2d.normalized = function () { return this.Scale(1.0 / this.norm(a)); }
    return v2d;
}();
var Thing = function () {
    var T = Object.create(null);
    T.init = function (name) {
        this.name = name;
        this.moment = 0.0;
        this.oneOverMoment = 0.0;
        this.spinPosition = 0.0;
        this.spinVelocity = 0.0;
        this.torque = 0.0;
        this.mass = 0.0;
        this.oneOverMass = 0.0;
        this.position = Vector2d.zero();
        this.velocity = Vector2d.zero();
        this.force = Vector2d.zero();
        this.mediumDensity = -0.05;
        return this;
    }
    T.integrate = function (deltaTime) {
        this.force = this.force.add(this.velocity.scale(this.mediumDensity));
        var deltaVelocity = this.force.scale(this.oneOverMass * deltaTime);
        this.force = Vector2d.zero();
        var deltaSpinVelocity = this.torque * (this.oneOverMoment * deltaTime);
        this.torque = 0.0;
        this.position = this.position.add((deltaVelocity.scale(0.5).add(this.velocity)).scale(deltaTime));
        this.spinPosition = this.spinPosition + (((deltaSpinVelocity * 0.5) + this.spinVelocity) * deltaTime);
        this.velocity = this.velocity.add(deltaVelocity);
        this.spinVelocity = this.spinVelocity + deltaSpinVelocity;
        if (Math.abs(this.spinVelocity) > 0) {
            var sgn = (this.spinVelocity / Math.abs(this.spinVelocity));
            this.spinVelocity = sgn * Math.min(5, Math.abs(this.spinVelocity));
        }
        var TWO_PI = Math.PI * 2;
        while (this.spinPosition >= TWO_PI)
            this.spinPosition -= TWO_PI;
        while (this.spinPosition < 0)
            this.spinPosition += TWO_PI;
    }
    T.applyForce = function (force) {
        this.force = this.force.add(force);
    }
    T.applyTorque = function (torque) {
        this.torque += torque;
    }
    T.makeBallGeometry = function (container, radius) {
        this.mass = Math.PI * radius * radius;
        this.moment = (this.mass * radius * radius) / 2.0;
        this.oneOverMass = 1.0 / this.mass;
        this.oneOverMoment = 1.0 / this.moment;
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
        var radius = 0.10;
        this.mass = Math.PI * radius * radius;
        this.moment = (this.mass * radius * radius) / 2.0;
        this.oneOverMass = 1.0 / this.mass;
        this.oneOverMoment = 1.0 / this.moment;
    };
    T.update = function (deltaTime) {
        this.integrate(deltaTime);
        this.svg.attr("transform", "translate(" + this.position.x + "," + this.position.y + ") rotate(" + (this.spinPosition * (180.0 / Math.PI)) + ", 0, 0)");
    };
    return T;
}();
var Ship = function () {
    var S = Object.create(Thing);
    S.init = function (name) {
        this.engines = Vector2d.zero();
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
var scale = 1.0;
var upkeydown = false;
var leftkeydown = false;
var rightkeydown = false;
function initPage() {
    var body = document.body;
    body.onkeydown = function () {
        if (event.keyCode == 38) upkeydown = true;
        if (event.keyCode == 37) leftkeydown = true;
        if (event.keyCode == 39) rightkeydown = true;
    }
    body.onkeyup = function () {
        if (event.keyCode == 38) upkeydown = false;
        if (event.keyCode == 37) leftkeydown = false;
        if (event.keyCode == 39) rightkeydown = false;
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
    var ship = Object.create (Ship).init ("Ship 1");
    ship.makePolygonGeometry(svg, {});
    var deltaTime = 1.0 / 20.0;
    debugger;
    var gametimer = setInterval(function () {
        if (upkeydown) { ship.thrust(50); }
        if (leftkeydown) { ship.rotate(80); }
        if (rightkeydown) { ship.rotate(-80); }
        ship.update(deltaTime);
    }, 1000 * deltaTime);
}
