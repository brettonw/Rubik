// This program is intended to provide a foundation for simulating disease
// transfer in a population with a variety of different factors, and across a
// range of strategies for preventing disease transfer

// factors:
// - Geographic Locality
// - Activity level (some people will have more interactions than others, maybe
//   males in general?
// - perfect random mixing vs. sweep over every user...
// - gestation period
// - dying? withdrawing from population...

// strategies
// - Ignorance
// - Abstinence
// - Monogamy
// - "Limited" Poly
// - Prophylactic use
// - Selective Partnering based on Testing/Knowledge

// We start with a population of unspecified individuals, and run a clock. At
// each tick of the clock we conduct an event. An event represents an
// opportunity for a disease transfer to occur. The population is seeded to
// provide a source for the disease transfer to occur.

// events model transmission rates, affected by the various strategies

var red = "#800000";
var gray = "#808080";

// the population
var populationWidth = 16;
var populationHeight = 16;
var populationSize = populationWidth * populationHeight;
var population;
var infectedCount = 0;

// the disease parameters
var transmissibility = 1.0;

// the clock
var clock = 0;

function createPopulation () {
    population = [];

    // create all the individuals
    for (var id = 0; id < populationSize; ++id) {
        var person = {};
        person.id = id;
        person.infected = -1;
        population[id] = person;
    }

    // now seed one of the individuals with disease
    var a = Math.floor (Math.random () * populationSize);
    var personA = population[a];
    personA.infected = clock;
    infectedCount = 1;
};

// parameters from the last event
var lastPersonA, lastPersonB;

function conductEvent () {
    if (infectedCount < populationSize) {
        // advance the clock and clear the last event info
        if (++clock > 1) {
            // denote these people as "active"
            lastPersonA.link.style.stroke = gray;
            lastPersonB.link.style.stroke = gray;
        }

        // randomly pair two individuals from the population
        var a = Math.floor (Math.random () * populationSize);
        var b = a;
        while (b == a) {
            b = Math.floor (Math.random () * populationSize);
        }
        var personA = population[a];
        var personB = population[b];

        // highlight this pairing
        personA.link.style.stroke = "yellow";
        personB.link.style.stroke = "yellow";

        // now handle the event between these two individuals to see if transmission
        // occurs, only if just one of them is infected (i.e. we don't consider that
        // an individual might "get it more" or "get it again")
        // XXX might want to consider ability to get it again, if it can clear...
        if ((personA.infected >= 0) != (personB.infected >= 0)) {
            // if the infections would transfer...
            if (Math.random () < transmissibility) {
                ++infectedCount;

                // transfer the infection
                if (personA.infected < 0) {
                    personA.infected = clock; personA.link.style.fill = red;
                } else {
                    personB.infected = clock; personB.link.style.fill = red;
                }

            }
        }

        // save this information for the next event
        lastPersonA = personA;
        lastPersonB = personB;
    } else {
        lastPersonA.link.style.stroke = gray;
        lastPersonB.link.style.stroke = gray;
        paused = true;
    }
    clockDisplay.textContent = clock + " (" + infectedCount + "/" + populationSize + ")";
}

function makeSvg () {
    // open the SVG and make the render port work like a mathematical system
    var svg="<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"-1.25 -1.25 2.5 2.5\" preserveAspectRatio=\"xMidYMid meet\" onclick=\"click()\">";
    svg += "<g id=\"root\" transform=\"scale(1, -1)\">";

    // compute the placement parameters
    var blockSize = 0.75;
    var horizontalSpacing = 2.0 / (populationWidth + 1);
    var horizontalSize = horizontalSpacing * blockSize;
    var horizontalOffset = -horizontalSize / 2.0;
    var verticalSpacing = 2.0 / (populationHeight + 1);
    var verticalSize = verticalSpacing * blockSize;
    var verticalOffset = -verticalSize / 2.0;

    // loop over the whole population to place each individual in the world
    for (var id = 0; id < populationSize; ++id) {
        // get the individual from the population
        var person = population[id];

        // compute the position of the block
        var x = (id % populationWidth) + 1;
        x = -1 + (x * horizontalSpacing) + horizontalOffset;
        var y = Math.floor (id / populationWidth) + 1;
        y = -1 + (y * verticalSpacing) + verticalOffset;

        // enumerate the block
        var rect = "";
        rect += "<rect id=\"rect" + id + "\"";
        rect += " x=\"" + x + "\" y=\"" + y + "\"";
        rect += " width=\"" + horizontalSize + "\" height=\"" + verticalSize + "\"";
        rect += " fill=\"" + ((person.infected >= 0) ? red : "white") + "\"";
        rect += " stroke=\"black\" stroke-width=\"0.005\" />"
        svg += rect;
    }
    svg += "</g>";

    // add the clock
    svg += "<text id=\"clockDisplay\" x=\"-1\" y=\"-1\" font-family=\"Verdana\" font-size=\"0.075\" fill=\"#404040\">Click to start</text>";

    // close the SVG
    svg += "</svg>";
    return svg;
}

var clockDisplay;
function linkSvg () {
    for (var id = 0; id < populationSize; ++id) {
        var person = population[id];
        var link = document.getElementById ("rect" + id);
        person.link = link;
    }
    clockDisplay = document.getElementById ("clockDisplay");
}

// clock functions
var paused = true;
function tick () {
    if (! paused) {
        conductEvent ();
        setTimeout(tick, 20);
    }
}

function click () {
    // pause and resume animation
    if (paused && (infectedCount < populationSize)) {
        paused = false;
        tick ();
    } else {
        paused = true;
    }
}

function main () {
    createPopulation ();
    var display = makeSvg ();
    document.getElementById ("display").innerHTML = display;
    linkSvg ();
}

