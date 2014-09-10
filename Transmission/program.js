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
// provide a source for the disease transfer to occur.

// events model transmission rates, affected by the various strategies

var pink = "#FF8080";
var red = "#800000";
var gray = "#808080";

// the population
var populationWidth = 10;
var populationHeight = 10;
var populationSize = populationWidth * populationHeight;
var population;
var infectedCount = 0;

// the disease parameters
var transmissibility = 0.5;
var daysToIncubateMin = 7.0;
var daysToIncubateMax = 14.0;
var daysToHealMin = 10.0;
var daysToHealMax = 21.0;

// the clock
var clock = 0;
var day = 0;
var clockDisplay;
var paused = true;
var individualEventsPerWeek = 2.0;
var eventsPerDiem = Math.floor (populationSize * (individualEventsPerWeek / 7.0));

var lastPersonA, lastPersonB;

function createPopulation () {
    population = [];

    // create all the individuals
    for (var id = 0; id < populationSize; ++id) {
        population[id] = {
            id: id,
            infectedDay: -1,
            contagious: false
        };
    }

    // now seed one of the individuals with disease
    var a = Math.floor (Math.random () * populationSize);
    var personA = population[a];
    personA.infectedDay = day;
    //personA.contagious = true;
    infectedCount = 1;
};

function conductEvent () {
    // clear the last event info by denoting those persons as "active"
    lastPersonA.link.style.stroke = gray;
    lastPersonB.link.style.stroke = gray;

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
    // occurs, only if just one of them is infected and the other is not
    if ((personA.contagious && (personB.infectedDay < 0)) ||
        (personB.contagious && (personA.infectedDay < 0))) {
        // if the infections would transfer...
        if (Math.random () < transmissibility) {
            ++infectedCount;

            // transfer the infection
            if (personA.infectedDay < 0) {
                personA.infectedDay = day;
                personA.link.style.fill = pink;
            } else {
                personB.infectedDay = day;
                personB.link.style.fill = pink;
            }
        }
    }

    // save this information for the next event
    lastPersonA = personA;
    lastPersonB = personB;
}

function allInfected () {
    lastPersonA.link.style.stroke = gray;
    lastPersonB.link.style.stroke = gray;
}

function startNewDay () {
    day = Math.floor (clock / eventsPerDiem);

    // sweep over the population to see if somebody becomes contagious or heals
    // or dies, or...
    for (var id = 0; id < populationSize; ++id) {
        var person = population[id];

        // if the person is infected but not contagious, generate a probability
        // they will become contagious
        if ((person.infectedDay >= 0) && (!person.contagious)) {
            var probability = ((day - person.infectedDay) - daysToIncubateMin) / (daysToIncubateMax - daysToIncubateMin);
            if (Math.random () < probability) {
                person.contagious = true;
                person.link.style.fill = red;
            }
        }
    }
}

function tick () {
    if (! paused) {
        if (infectedCount < populationSize) {
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
    clockDisplay.textContent = "Day " + day + " (" + clock + ", " + infectedCount + "/" + populationSize + ")";
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
        rect += " fill=\"" + ((person.infectedDay >= 0) ? pink : "white") + "\"";
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
        var person = population[id];
        var link = document.getElementById ("rect" + id);
        person.link = link;
    }
    clockDisplay = document.getElementById ("clockDisplay");

    // create a dummy person to clear status on so that we can remove an "if"
    // statement from the inner loop (this object will be abandoned very early)
    lastPersonA = {};
    lastPersonA.link = {};
    lastPersonA.link.style = {};
    lastPersonB = lastPersonA;
}

function main () {
    createPopulation ();
    var display = makeSvg ();
    document.getElementById ("display").innerHTML = display;
    linkSvg ();
}

