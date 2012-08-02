DERIVE_EXPR(Constant);
Constant.N = function(values) {
    return this.constant;
};

Constant.D = function(values) {
    return EXPR(Constant)(0);
};

Constant.Render = function(enclose) {
    return this.constant.toString();
};

Constant._init = function() {
    this.constant = arguments[0];
    return this;
};
