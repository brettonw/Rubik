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
    v2d.angle = function (a) { return makeVector(Math.cos(a), Math.sin(a)); };

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
