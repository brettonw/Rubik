var visDisplay = null;
var vis;

var margins = [20, 20, 20, 20],
    displayWidth = 640 - margins[1] - margins[3],
    displayHeight = 480 - margins[0] - margins[2],
    idCounter = 0;

var tree = null;
var diagonal = d3.svg.diagonal().projection(function (d) { return [d.y, d.x]; });
var root;
var maxDepth = 0;

function BuildTree() {
    tree = d3.layout.tree().size([displayHeight, displayWidth]);

    if (visDisplay != null) {
        visDisplay.remove();
    }

    visDisplay = d3.select("#divDisplayTree")
        .append("svg:svg")
        .attr("width", displayWidth + margins[1] + margins[3])
        .attr("height", displayHeight + margins[0] + margins[2]);

    vis = visDisplay
        .append("svg:g")
        .attr("transform", "translate(" + margins[3] + "," + margins[0] + ")");

    // get the Jane display tree
    root = Jane.dataRefs.nodes[0];

    root.x0 = displayHeight / 2;
    root.y0 = 0;
}

function ToggleNode(d) {
    if (d.children) {
        d._children = d.children;
        d.children = null;
    } else {
        d.children = d._children;
        d._children = null;
    }
}

function UpdateTreeDisplay(source) {
    var duration = d3.event && d3.event.altKey ? 5000 : 500;

    var colorNode = function (d) {
        if (d.dataRef != null) {
            return d.dataRef.HasData() ? "#4f4" : "#f44";
        }
        return "#44f";
    };

    // Compute the new tree layout.
    var nodes = tree.nodes(root).reverse();

    // space the nodes according to the tree depth
    nodes.forEach(function (d) { d.y = root.y0 + (d.depth * ((displayWidth - (root.y0 + (margins[1] * 2.0))) / maxDepth)); });

    // Update the nodes…
    var node = vis.selectAll("g.node")
        .data(nodes, function (d) { return d.id || (d.id = ++idCounter); });

    // Enter any new nodes at the parent's previous position.
    var nodeEnter = node.enter().append("svg:g")
        .attr("class", "node")
        .attr("transform", function (d) { return "translate(" + source.y0 + "," + source.x0 + ")"; })
        .on("click", function (d) { ToggleNode(d); update(d); });

    nodeEnter.append("svg:circle")
        .attr("r", 1e-6)
        .style("fill", colorNode);

    nodeEnter.append("svg:text")
        .attr("x", function (d) { return d.children || d._children ? -10 : 10; })
        .attr("dy", ".35em")
        .attr("text-anchor", function (d) { return d.children || d._children ? "end" : "start"; })
        .text(function (d) { return d.name; })
        .style("fill-opacity", 1e-6);

    // Transition nodes to their new position.
    var nodeUpdate = node.transition()
        .duration(duration)
        .attr("transform", function (d) { return "translate(" + d.y + "," + d.x + ")"; });

    nodeUpdate.select("circle")
        .attr("r", 4.5)
        .style("fill", colorNode);

    nodeUpdate.select("text")
        .style("fill-opacity", 1);

    // Transition exiting nodes to the parent's new position.
    var nodeExit = node.exit().transition()
        .duration(duration)
        .attr("transform", function (d) { return "translate(" + source.y + "," + source.x + ")"; })
        .remove();

    nodeExit.select("circle")
        .attr("r", 1e-6);

    nodeExit.select("text")
        .style("fill-opacity", 1e-6);

    // Update the links…
    var link = vis.selectAll("path.link")
        .data(tree.links(nodes), function (d) { return d.target.id; });

    // Enter any new links at the parent's previous position.
    link.enter().insert("svg:path", "g")
        .attr("class", "link")
        .attr("d", function (d) {
            var o = { x: source.x0, y: source.y0 };
            return diagonal({ source: o, target: o });
        })
      .transition()
        .duration(duration)
        .attr("d", diagonal);

    // Transition links to their new position.
    link.transition()
        .duration(duration)
        .attr("d", diagonal);

    // Transition exiting nodes to the parent's new position.
    link.exit().transition()
        .duration(duration)
        .attr("d", function (d) {
            var o = { x: source.x, y: source.y };
            return diagonal({ source: o, target: o });
        })
        .remove();

    // Stash the old positions for transition.
    nodes.forEach(function (d) {
        d.x0 = d.x;
        d.y0 = d.y;
    });
}

function DrawTree() {
    maxDepth = Jane.dataRefs.depth;
    UpdateTreeDisplay(root);
}

