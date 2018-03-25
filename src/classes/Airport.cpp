#include "Airport.h"

bool Airport::properlyInitialized() const {
    return this == init;
}

Airport::Airport(const std::string& _airportName,
                 const std::string& _iata,
                 const std::string& _callsign,
                 const unsigned int _gates)
        : airportName(_airportName),
          iata(_iata),
          callsign(_callsign),
          gates(_gates),
          init(this) {
    ENSURE(properlyInitialized(), "Airport was not properly initialized.");
}

Airport::Airport(Airport* _airport)
        : airportName(_airport->getAirportName()),
          iata(_airport->getIata()),
          callsign(_airport->getCallsign()),
          gates(_airport->getGates()),
          init(this) {
    REQUIRE(_airport->properlyInitialized(), "Reference Airport was not properly initialized.");
    ENSURE(properlyInitialized(), "Airport was not properly initialized.");
}

const std::string& Airport::getAirportName() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return airportName;
}

const std::string& Airport::getIata() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return iata;
}

const std::string& Airport::getCallsign() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return callsign;
}

const unsigned int Airport::getGates() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return gates;
}

void Airport::addRunway(Runway* _runway)
{
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    REQUIRE(_runway != NULL, "Referenced Runway was a nullpointer.");
    REQUIRE(_runway->getAirport() == this, "Referenced Runway does not have a link to the current airport.");
    REQUIRE(!vectorContains<Runway*>(runways, _runway), "Referenced Runway already added to the airport.");
    runways.push_back(_runway);
    ENSURE(vectorContains<Runway*>(runways, _runway), "Referenced Runway was not properly added.");
}

/*void Airport::deleteRunway(Runway* _runway)
{
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    REQUIRE(vectorContains<Runway*>(runways, _runway), "Referenced Runway was not found, can't be deleted.");
    removeFromVector<Runway*>(runways, _runway);
    _runway->setAirport(NULL);
    ENSURE(!vectorContains<Runway*>(runways, _runway), "Referenced Runway was not properly deleted.");
    ENSURE(_runway->getAirport() == NULL, "References Airport was not properly unlinked from this airport");
}*/


void Airport::printInfo(std::ostream& stream) const
{
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    stream << "Airport: " << getAirportName() << " (" << getIata() << ")" << std::endl;
    stream << " -> gates: " << gates << std::endl;
    stream << " -> runways: " << runways.size() << std::endl;

    for (RunwayVectorConstIterator it_runway = runways.begin(); it_runway != runways.end(); it_runway++) {
        stream << "    ~ " << (*it_runway)->getRunwayName() << std::endl;
    }

    stream << std::endl;
}