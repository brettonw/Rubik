// This program is intended to provide a foundation for simulating disease
// transfer in a population with a variety of different factors, and across a
// range of strategies for preventing disease transfer

// factors:
// - Geographic Locality
// - Activity level (some people will have more interactions than others, maybe
//   males in general?
// - perfect random mixing vs. sweep over every user...
// - gestation/incubation period
// - non-contagious phases (healed after initial infection, or like HSV in sporadic bursts)
// - immunity (after catching it once)
// - dying? withdrawing from population...

// strategies
// - Ignorance
// - Abstinence/Avoidance
// - Monogamy/Partner Preference/"Limited" Poly
// - Prophylactic use
// - Selective Partnering based on Testing/Knowledge
// - self limiting behavior based on self knowledge

// We start with a population of unspecified individuals, and run a clock. At
// each tick of the clock we conduct an event. An event represents an
// opportunity for a disease transfer to occur. The population is seeded to
// provide a source for the disease.

// events model transmission rates, affected by the various strategies

var borderColorHighlight = "#FFFF00";
var borderColorActive = "#808080";

// the population
var populationWidth = 10;
var populationHeight = 10;
var populationSize = populationWidth * populationHeight;
var population;
var infectedCount = 0;
var totalInfectedCount = 0;

// the clock
var clock;
var day;
var clockDisplay;
var paused;
var individualEventsPerWeek = 2.0;
var eventsPerDiem = Math.floor (populationSize * (individualEventsPerWeek / 7.0));

var lastAtomA, lastAtomB;

// disease states with parameters
var states = {
    HEALTHY         : { name: "HEALTHY",      color: "#FFFFFF", susceptible: true,  contagious: 0.00 },
    INFECTED        : { name: "INFECTED",     color: "#FFC0C0", susceptible: false, contagious: 0.05, daysMin: 7, daysMax: 14 },
    CLINICAL        : { name: "CLINICAL",     color: "#FF0000", susceptible: false, contagious: 0.50, daysMin: 7, daysMax: 14 },
    CONVALESCENT    : { name: "CONVALESCENT", color: "#800000", susceptible: false, contagious: 0.05 }
};

function createPopulation () {
    population = [];

    // create all the individuals
    for (var id = 0; id < populationSize; ++id) {
        population[id] = {
            id: id,
            state: states.HEALTHY,
            day: -1
        };
    }

    // now seed one of the individuals with disease
    var randomIndex = Math.floor (Math.random () * populationSize);
    var atom = population[randomIndex];
    atom.state = states.INFECTED;
    atom.day = day;
    infectedCount = totalInfectedCount = 1;
};

function conductEvent () {
    // clear the last event info by denoting those atoms as "active"
    lastAtomA.link.style.stroke = borderColorActive;
    lastAtomB.link.style.stroke = borderColorActive;

    // randomly pair two individuals from the population
    var indexA = Math.floor (Math.random () * populationSize);
    var indexB = indexA;
    while (indexB == indexA) {
        indexB = Math.floor (Math.random () * populationSize);
    }
    var atomA = population[indexA];
    var atomB = population[indexB];

    // highlight this pairing
    atomA.link.style.stroke = borderColorHighlight;
    atomB.link.style.stroke = borderColorHighlight;

    // check if a transmission an occur
    var canTransmit = function (a, b) {
        return (a.state.susceptible && (b.state.contagious > 0.0));
    }

    // now handle the event between these two individuals to see if transmission
    // occurs, only if just one of them is infected and the other is not
    if (canTransmit (atomA, atomB) || canTransmit (atomB, atomA)) {
        // if the infections would transfer...
        if (Math.random () < (atomA.state.contagious + atomB.state.contagious)) {
            var infectAtom = function (atom) {
                if (atom.state == states.HEALTHY) {
                    ++infectedCount;
                    ++totalInfectedCount;
                    atom.state = states.INFECTED;
                    atom.day = day;
                    atom.link.style.fill = atom.state.color;
                }
            };

            infectAtom (atomA);
            infectAtom (atomB);
        }
    }

    // save this information for the next event
    lastAtomA = atomA;
    lastAtomB = atomB;
}

function allInfected () {
    lastAtomA.link.style.stroke = borderColorActive;
    lastAtomB.link.style.stroke = borderColorActive;
}

function startNewDay () {
    day = Math.floor (clock / eventsPerDiem);

    // sweep over the population to see if somebody becomes contagious or heals
    // or dies, or...
    for (var id = 0; id < populationSize; ++id) {
        var atom = population[id];

        switch (atom.state.name) {
            case states.INFECTED.name: {
                var probability = ((day - atom.day) - atom.state.daysMin) / (atom.state.daysMax - atom.state.daysMin);
                if (Math.random () < probability) {
                    atom.state = states.CLINICAL;
                    atom.day = day;
                    atom.link.style.fill = atom.state.color;
                }
            }
            break;
            case states.CLINICAL.name: {
                var probability = ((day - atom.day) - atom.state.daysMin) / (atom.state.daysMax - atom.state.daysMin);
                if (Math.random () < probability) {
                    --infectedCount;
                    atom.state = states.CONVALESCENT;
                    atom.day = day;
                    atom.link.style.fill = atom.state.color;
                }
            }
            break;
        }
    }
}

function tick () {
    if (! paused) {
        if ((infectedCount > 0) && (infectedCount < populationSize)) {
            // see if it's a new day
            if ((clock % eventsPerDiem) == 0) {
                // do things that happen on a per day basis - we do this here to
                // avoid bias in events
                startNewDay ();
            }

            // advance the clock
            ++clock;

            // conduct an event and loop back @ 30Hz
            conductEvent ();
            setTimeout(tick, 33);
        } else {
            allInfected ();
            paused = true;
        }
    }
    clockDisplay.textContent = "Day " + day + " (" + clock + ", " + totalInfectedCount + "/" + populationSize + ")";
}

function click () {
    // pause and resume animation
    if (paused) {
        paused = false;
        tick ();
    } else {
        paused = true;
    }
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
        var atom = population[id];

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
        rect += " fill=\"" + atom.state.color + "\"";
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

function linkSvg () {
    for (var id = 0; id < populationSize; ++id) {
        var atom = population[id];
        var link = document.getElementById ("rect" + id);
        atom.link = link;
    }
    clockDisplay = document.getElementById ("clockDisplay");

    // create a dummy atom to clear status on so that we can remove an "if"
    // statement from the inner loop (this object will be abandoned very early)
    lastAtomA = {};
    lastAtomA.link = {};
    lastAtomA.link.style = {};
    lastAtomB = lastAtomA;
}

function main () {
    // reset the clock
    clock = day = 0;
    paused = true;

    createPopulation ();
    var display = makeSvg ();
    document.getElementById ("display").innerHTML = display;
    linkSvg ();
}

