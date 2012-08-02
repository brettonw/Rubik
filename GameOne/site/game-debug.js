"use strict";
var Vector2d = Object.create (null);
Vector2d.Create = function(a) { return {x:a[0], y:a[1]}; };
Vector2d.XY = function(x, y) { return {x:x, y:y}; };
Vector2d.Zero = function() { return {x:0.0, y:0.0}; };
Vector2d.One = function() { return {x:1.0, y:1.0}; };
Vector2d.Add = function(a, b) { return this.Create (a.x + b.x, a.y + b.y); };
Vector2d.Subtract = function(a, b) { return this.Create (a.x - b.x, a.y - b.y); };
Vector2d.Scale = function(a, b) { return this.Create (a.x * b, a.y * b); };
Vector2d.Dot = function(a, b) { return (a.x * b.x) + (a.y * b.y); };
Vector2d.Cross = function(a, b) { return (a.x * b.y) - (a.y * b.x); };
Vector2d.NormSq = function(a) { return this.Dot (a, a); };
Vector2d.Norm = function(a) { return Math.sqrt (this.NormSq (a)); };
var Thing = Object.create (null);
Thing.orientation = 0.0;
Thing.angularVelocity = 0.0;
Thing.position = Vector2d.Zero ();
Thing.velocity = Vector2d.Zero ();
Thing.geometry = [
    Vector2d.XY ( 0.00, 0.00),
    Vector2d.XY (-0.05, 0.05),
    Vector2d.XY ( 0.10, 0.00),
    Vector2d.XY (-0.05,-0.05)
];
Thing.mass = 1.0;
Thing.momentOfInertia = 0.0;
Thing.makeSvg = function(container) {
    var points = this.geometry[0].x + "," + this.geometry[0].y;
    for (var i = 1; i < this.geometry.length; ++i) {
        points += " " + this.geometry[i].x + "," + this.geometry[i].y;
    }
    this.svg = container.append("polygon")
    .attr("stroke-width", 2.0 / scale)
    .attr("fill", "red")
    .attr("fill-opacity", "1.0")
    .attr("stroke", "black")
    .attr("stroke-opacity", "1.0")
    .attr("stroke-linejoin", "round")
    .attr("points", points);
};
Thing.update = function(deltaTime) {
    this.position = Vector2d.XY (this.position.x + (this.velocity.x * deltaTime), this.position.y + (this.velocity.y * deltaTime));
    this.svg.attr("transform", "translate(" + this.position.x + "," + this.position.y + ") rotate(" + this.orientation + ", 0.0, 0.0)");
};var Ship = Object.create (Thing);
Ship.engines = Vector2d.Zero ();
Ship.update = function(deltaTime) {
    var orientationRadians = (this.orientation * Math.PI) / 180.0;
    var accel = 0.01;
    this.velocity = Vector2d.XY (this.velocity.x + (Math.cos (orientationRadians) * accel * deltaTime), this.velocity.y + (Math.sin (orientationRadians) * accel * deltaTime));
    this.orientation += 1.0;
    Object.getPrototypeOf (Ship).update.call (this, deltaTime);
};
var scale = 1.0;
function initPage() {
    var target = d3.select("#display");
    var svg = target.append("svg").attr("class", "gameDisplay");
    svg.append("rect")
        .attr("class", "gameBackground")
        .attr("width", "100%")
        .attr("height", "100%");
    svg.style("opacity", 1.0e-6)
        .transition().duration(1000)
        .style("opacity", 1.0);
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
    var ship = Object.create (Ship);
    ship.makeSvg (svg);
    var ship2 = Object.create (Ship);
    ship2.makeSvg (svg);
    ship2.position = Vector2d.XY (-0.5, -0.5);
    var deltaTime = 1.0 / 20.0;
    var gametimer = setInterval (function () {
        ship.update (deltaTime);
        ship2.update (deltaTime);
    }, 1000 * deltaTime);
}
