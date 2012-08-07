var Thing = Object.create (null);

// rotational parameters of a physical body in 2 dimensions, e.g. it can only 
// rotate around an axis that is perpendicular to the 2D plane
Thing.oneOverMoment = 0.0;
Thing.spinPosition = 0.0;
Thing.spinVelocity = 0.0;
Thing.torque = 0.0;

// translational parameters of a physical body in 2 dimensions
Thing.oneOverMass = 0.0;
Thing.position = Vector2d.Zero ();
Thing.velocity = Vector2d.Zero ();
Thing.force = Vector2d.Zero ();

// density of the traveling medium that will be use to impede the movement of
// the object (think air drag)
Thing.mediumDensity = -0.05;

Thing.integrate = function(deltaTime) {
    // compute forces due to viscous drag
    this.force = Vector2d.Add(this.force, Vector2d.Scale(this.velocity, this.mediumDensity));
    this.torque = this.torque + (this.spinVelocity * this.viscosity);
    
    // compute accelerations from the forces, then clear out the forces
    var deltaVelocity = Vector2d.Scale(this.force, (this.oneOverMass * deltaTime));
    this.force = Vector2d.Zero();
    var deltaSpinVelocity = this.torque * this.oneOverMoment * deltaTime;
    this.torque = 0.0;
    
    // using the midpoint method, compute the position changes
    this.position = Vector2d.Add(this.position, Vector2d.Scale(Vector2d.Add(Vector2d.Scale(deltaVelocity, 0.5), this.velocity), deltaTime));
    this.spinPosition = this.spinPosition + (((deltaSpinVelocity * 0.5) + this.spinVelocity) * deltaTime);
    
    // update the velocities from the deltas
    this.velocity = Vector2d.Add(this.velocity, deltaVelocity);
    this.spinVelocity = this.spinVelocity + deltaSpinVelocity;

    // keep the spin position in a math friendly range
    var TWO_PI = MATH.PI * 2;
	while (this.spinPosition >= TWO_PI)
		this.spinPosition -= TWO_PI;
	while (this.spinPosition < 0)
		this.spinPosition += TWO_PI;
}

Thing.applyForce = function(force) {
    this.force = force;
}

thing.applyTorque = function(torque) {
    this.torque = torque;
}

// geometry is used for bounding the object, for collision detection, for drawing,
// and for creating the visual representation, computed values assume a homo-
// genous object with geometry centered (the CG is located at the origin)
Thing.makeBallGeometry = function(radius) {
    // compute the mass and the moment
    var mass = Math.PI * radius * radius;
    var moment = (mass * radius * radius) / 2.0;
    this.oneOverMass = 1.0 / mass;
    this.oneOverMoment = 1.0 / moment;
    
    this.svg = container.append("circle")
        .attr("stroke-width", 2.0 / scale)
        .attr("fill", "red")
        .attr("fill-opacity", "1.0")
        .attr("stroke", "black")
        .attr("stroke-opacity", "1.0")
        .attr("r", radius);
};

Thing.makePolygonGeometry = function(geometry) {
    var geometry = [
        Vector2d.XY ( 0.00, 0.00), 
        Vector2d.XY (-0.05, 0.05), 
        Vector2d.XY ( 0.10, 0.00), 
        Vector2d.XY (-0.05,-0.05)
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
};

Thing.update = function(deltaTime) {
    this.position = Vector2d.XY (this.position.x + (this.velocity.x * deltaTime), this.position.y + (this.velocity.y * deltaTime));
    this.svg.attr("transform", "translate(" + this.position.x + "," + this.position.y + ") rotate(" + this.orientation + ", 0.0, 0.0)");
};