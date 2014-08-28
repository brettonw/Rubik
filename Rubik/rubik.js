// functions to create and manipulate the cube representation
function newCube () {
    var WHT = "#FFFFFF";
    var GRN = "00C400";
    var RED = "#FF0000";
    var BLU = "#0000FF";
    var ONG = "#FF8000";
    var YEL = "#FFFF00";
    
    return [
        WHT, WHT, WHT, WHT,     // 0 - top face
        GRN, GRN, GRN, GRN,     // 4 - left face
        RED, RED, RED, RED,     // 8 - front face
        BLU, BLU, BLU, BLU,     // 12 - right face
        ONG, ONG, ONG, ONG,     // 16 - back face
        YEL, YEL, YEL, YEL      // 20 - bottom face
    ];
}

function rotate (cube, sides, face) {
    var t1, t2;
    
    // rotate the side faces in the order given
    t1 = cube[sides[0]]; t2 = cube[sides[1]];
    cube[sides[0]] = cube[sides[2]]; cube[sides[1]] = cube[sides[3]];
    cube[sides[2]] = cube[sides[4]]; cube[sides[3]] = cube[sides[5]];
    cube[sides[4]] = cube[sides[6]]; cube[sides[5]] = cube[sides[7]];
    cube[sides[6]] = t1; cube[sides[7]] = t2;
    
    // rotate the face in the order given
    t1 = cube[face[0]];
    cube[face[0]] = cube[face[1]];
    cube[face[1]] = cube[face[2]];
    cube[face[2]] = cube[face[3]];
    cube[face[3]] = t1;
}

function moveCube (cube, axis) {
    switch (axis) {
        // right face
    case "x+":
        rotate (cube, [1, 3, 9, 11, 21, 23, 18, 16], [12, 14, 15, 13]);
        break;
    case "x-":
        rotate (cube, [1, 3, 18, 16, 21, 23, 9, 11], [12, 13, 15, 14]);
        break;
        // top face
    case "y+":
        rotate (cube, [4, 5, 8, 9, 12, 13, 16, 17], [0, 2, 3, 1]);
        break;
    case "y-":
        rotate (cube, [4, 5, 16, 17, 12, 13, 8, 9], [0, 1, 3, 2]);
        break;
        // front face
    case "z+":
        rotate (cube, [3, 2, 5, 7, 20, 21, 14, 12], [8, 10, 11, 9]);
        break;
    case "z-":
        rotate (cube, [3, 2, 14, 12, 20, 21, 5, 7], [8, 9, 11, 10]);
        break;
    }
}

function scoreCube (srcCube, dstCube) {
    // ideally, "God's algorithm" would compute the distance from the source to 
    // the destination as the number of moves. We can't do that because of the 
    // size of the configuration space, so we characterize the relative distance
    // as the number of faces that are not the correct color. A score of zero
    // indicates a 0 distance, a perfect match.
    var score = 0;
    for (var i = 0, length = srcCube.length; i < length; ++i) {
        score += (srcCube[i] != dstCube[i]) ? 1 : 0;
    }
    return score;
}

function makeSvgPolygon (color, points) {
    var polygon = "<polygon points=\"";
    polygon += points[0][0] + "," + points[0][1] + " ";
    polygon += points[1][0] + "," + points[1][1] + " ";
    polygon += points[2][0] + "," + points[2][1] + " ";
    polygon += points[3][0] + "," + points[3][1] + "\" ";
    polygon += "fill=\"" + color + "\" stroke=\"#202020\" stroke-width=\"0.025\" stroke-linecap=\"round\"/>";
    return polygon;
}

function makeSvgFace (cube, face, corners) {
    // make four sub faces
    var center = [
        (corners[0][0] + corners[1][0] + corners[2][0] + corners[3][0]) / 4,
        (corners[0][1] + corners[1][1] + corners[2][1] + corners[3][1]) / 4
    ];
    
    var edgeCenters = [
        [(corners[0][0] + corners[1][0]) / 2, (corners[0][1] + corners[1][1]) / 2],
        [(corners[1][0] + corners[2][0]) / 2, (corners[1][1] + corners[2][1]) / 2],
        [(corners[2][0] + corners[3][0]) / 2, (corners[2][1] + corners[3][1]) / 2],
        [(corners[3][0] + corners[0][0]) / 2, (corners[3][1] + corners[0][1]) / 2]
    ];
    
    var svg = makeSvgPolygon (cube[face[0]], [corners[0], edgeCenters[0], center, edgeCenters[3]]);
    svg += makeSvgPolygon (cube[face[1]], [corners[1], edgeCenters[1], center, edgeCenters[0]]);
    svg += makeSvgPolygon (cube[face[2]], [corners[2], edgeCenters[2], center, edgeCenters[1]]);
    svg += makeSvgPolygon (cube[face[3]], [corners[3], edgeCenters[3], center, edgeCenters[2]]);
    return svg;
}

function makeSvg (cube) {
    // open the SVG and make the render port work like a mathematical system
    var svg="<svg viewBox=\"-1.25 -1.25 2.5 2.5\">";
    svg += "<g transform=\"scale(1, -1)\">";
    
    // 6 points defining 3 faces of a necker cube
    var p = 0.925;
    var w = 0.875;
    var points = [[0.0, 0.0], [0.0, -1.0], [p * -w, p * -0.5], [-w, 0.5], [0.0, p * 1.0], [w, 0.5], [p * w, p * -0.5]];
    
    // render the faces with the subcubes linked in order of the vertices
    svg += makeSvgFace (cube, [9, 11, 10, 8], [points[0], points[1], points[2], points[3]]);
    svg += makeSvgFace (cube, [12, 14, 15, 13], [points[0], points[1], points[6], points[5]]);
    svg += makeSvgFace (cube, [3, 2, 0, 1], [points[0], points[3], points[4], points[5]]);
    
    // close the SVG
    svg += "</g></svg>";
    return svg;
}

function main () {
    var cube = newCube ();
    
    // a quick test
    moveCube (cube, "x+");
    moveCube (cube, "x-");
    moveCube (cube, "y+");
    moveCube (cube, "y-");
    moveCube (cube, "z+");
    moveCube (cube, "z-");
    
    // another quick test
    moveCube (cube, "x+");
    moveCube (cube, "y+");
    
    var display = makeSvg (cube);
    document.getElementById ("display").innerHTML = display;
}

