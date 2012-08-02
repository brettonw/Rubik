var Ship = Object.create (Thing);
Ship.engines = Vector2d.Zero ();

Ship.update = function(deltaTime) {
    // apply acceleration
    var orientationRadians = (this.orientation * Math.PI) / 180.0;
    var accel = 0.01;
    this.velocity = Vector2d.XY (this.velocity.x + (Math.cos (orientationRadians) * accel * deltaTime), this.velocity.y + (Math.sin (orientationRadians) * accel * deltaTime));
    this.orientation += 1.0;
    
    // do the parental thing
    Object.getPrototypeOf (Ship).update.call (this, deltaTime);
};
