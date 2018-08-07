#include "Location.h"

bool Location::properlyInitialized() const {
    return this == init;
}

Location::Location(const std::string& _name): name(_name), previousLocation(NULL), nextLocation(NULL), airport(NULL), init(this) {
    ENSURE(properlyInitialized(), "Location was not properly initialized.");
}

Location::Location(const Location* _location) : name(_location->getName()), previousLocation(_location->getPreviousLocation()), nextLocation(_location->getNextLocation()), airport(_location->getAirport()), init(this) {
    REQUIRE(_location->properlyInitialized(), "Referenced Location was not properly initialized.");
    ENSURE(properlyInitialized(), "Location was not properly initialized.");
}

const std::string& Location::getName() const {
    REQUIRE(properlyInitialized(), "Location was not properly initialized.");
    return name;
}

Location* Location::getPreviousLocation() const {
    REQUIRE(properlyInitialized(), "Location was not properly initialized.");
    return previousLocation;
}

void Location::setPreviousLocation(Location* _previousLocation) {
    REQUIRE(properlyInitialized(), "Location was not properly initialized.");
    previousLocation = _previousLocation;
    ENSURE(previousLocation == _previousLocation, "Referenced Next Location was not properly set.");
}

Location* Location::getNextLocation() const {
    REQUIRE(properlyInitialized(), "Location was not properly initialized.");
    return nextLocation;
}

void Location::setNextLocation(Location* _nextLocation) {
    REQUIRE(properlyInitialized(), "Location was not properly initialized.");
    nextLocation = _nextLocation;
    ENSURE(nextLocation == _nextLocation, "Referenced Next Location was not properly set.");
}

const Airport* Location::getAirport() const {
    REQUIRE(properlyInitialized(), "Location was not properly initialized.");
    return airport;
}

void Location::setAirport(const Airport* _airport) {
    REQUIRE(properlyInitialized(), "Location was not properly initialized.");
    airport = _airport;
    ENSURE(airport == _airport, "Referenced Airport was not properly set.");
}