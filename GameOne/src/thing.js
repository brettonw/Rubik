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
    
    // computed values based on the geometry assuming a homogenous
Thing.mass = 1.0;
    
    // recenter the geometry so that it's CoG is at (0, 0)
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
};