var scale = 1.0;

function initPage() {
    var target = d3.select("#display");
    var svg = target.append("svg").attr("class", "gameDisplay");
        
    // fade the display in
    svg.style("opacity", 1.0e-6)
        .transition().duration(1000)
        .style("opacity", 1.0);
        
    // add a background object so the base transform handler will always receive 
    // the pan and zoom interactions
    svg.append("rect")
        .attr("class", "gameBackground")
        .attr("width", "100%")
        .attr("height", "100%");

    // create a child g element to receive the zoom/pan transformation
    var child = svg.append("g").attr("class", "gameDisplay");
    svg.call (d3.behavior.zoom()
        .translate ([0, 0])
        .scale (1.0)
        .scaleExtent([0.125, 8.0])
        .on("zoom", function() {
            child
                //.transition().duration(100)
                .attr("transform", 
                    "translate(" + d3.event.translate[0] + "," +  d3.event.translate[1] + ") " + 
                    "scale(" +  d3.event.scale + ")"
                );
        })
    );
    
    // create a child g element to receive the universe transform (invert y and scale the view to [0..1, 0..1])
    svg = child.append("g").attr("class", "gameDisplay");
    var xScale = target[0][0].clientWidth;
    var yScale = target[0][0].clientHeight;
    scale = Math.min (xScale, yScale);
    svg.attr ("transform", "translate(" + (xScale / 2.0) + "," + (yScale / 2.0) + ") scale(" + scale + "," + -scale + ")");
    
    // create a child g element to contain the world
    svg = svg.append("g").attr("class", "gameDisplay");
    
    // add a grid
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
    ship.makePolygonGeometry (svg);
    var ship2 = Object.create (Ship);
    ship2.makeBallGeometry (svg);
    ship2.position = Vector2d.XY (-0.5, -0.5);
        
    var deltaTime = 1.0 / 20.0;
    var gametimer = setInterval (function () {
        ship.update (deltaTime);
        ship2.update (deltaTime);
    }, 1000 * deltaTime);
    
    
    // need a pause time button
}

