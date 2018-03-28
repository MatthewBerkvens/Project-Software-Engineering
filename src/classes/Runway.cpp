#include "Runway.h"

bool Runway::properlyInitialized() const {
    return this == init;
}

Runway::Runway(const std::string& _runwayName) : runwayName(_runwayName), airport(NULL), init(this) {
    ENSURE(properlyInitialized(), "Runway was not properly initialized.");
}

Runway::Runway(const Runway* _runway) : runwayName(_runway->getRunwayName()), airport(_runway->getAirport()), init(this) {
    REQUIRE(_runway->properlyInitialized(), "Reference Runway was not properly initialized.");
    ENSURE(properlyInitialized(), "Runway was not properly initialized.");
}

const std::string& Runway::getRunwayName() const {
    REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
    return runwayName;
}

const Airport* Runway::getAirport() const {
    REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
    return airport;
}

void Runway::setAirport(const Airport* _airport) {
    REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
    airport = _airport;
    ENSURE(airport == _airport, "Referenced Airport was not properly set.");
}

void Runway::printInfo(std::ostream& stream) const
{
    REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
    stream << "Runway: " << getRunwayName() << std::endl;
    stream << std::endl;
}