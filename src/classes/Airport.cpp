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
          airplanes(AirplaneMap()),
          runways(RunwayMap()),
          locations(LocationMap()),
          gates(AirplaneVector(_gates)),
          ft5000(NULL),
          ft3000(NULL),
          init(this) {
    ENSURE(properlyInitialized(), "Airport was not properly initialized.");
}

Airport::Airport(const Airport* _airport)
        : airportName(_airport->getAirportName()),
          iata(_airport->getIata()),
          callsign(_airport->getCallsign()),
          airplanes(AirplaneMap()),
          runways(RunwayMap()),
          locations(LocationMap()),
          gates(_airport->getGateCount()),
          ft5000(NULL),
          ft3000(NULL),
          init(this) {
    REQUIRE(_airport->properlyInitialized(), "Reference Airport was not properly initialized.");

    const RunwayMap& _runways = _airport->getRunways();
    for (RunwayMap::const_iterator it_runway = _runways.begin(); it_runway != _runways.end(); it_runway++) {
        const Runway* originalRunway = (*it_runway).second;
        Runway* newRunway = new Runway(originalRunway);
        newRunway->setAirport(this);
        runways[newRunway->getName()] = newRunway;
        locations[newRunway->getName()] = newRunway;
    }

    const LocationMap& _locations = _airport->getLocations();
    for (LocationMap::const_iterator it_location = _locations.begin(); it_location != _locations.end(); it_location++) {
        const Location* originalLocation = (*it_location).second;

        if (this->getLocationByName(originalLocation->getName()) == NULL) {
            Location* newLocation = new Location(originalLocation);

            newLocation->setAirport(this);

            locations[newLocation->getName()] = newLocation;
        }
    }

    const AirplaneMap& _airplanes = _airport->getAirplanes();
    for (AirplaneMap::const_iterator it_airplane = _airplanes.begin(); it_airplane != _airplanes.end(); it_airplane++) {
        const Airplane* originalAirplane = (*it_airplane).second;
        Airplane* newAirplane = new Airplane(originalAirplane);
        newAirplane->setAirport(this);

        if (originalAirplane->getRunway() != NULL) {
            newAirplane->setRunway(this->getRunwayByName(originalAirplane->getRunway()->getName()));
        }

        if (originalAirplane->getCurrentLocation() != NULL) {
            newAirplane->setCurrentLocation(this->getLocationByName(originalAirplane->getCurrentLocation()->getName()));
        }

        if (originalAirplane->getTakeoffRunway() != NULL) {
            newAirplane->setTakeoffRunway(this->getRunwayByName(originalAirplane->getTakeoffRunway()->getName()));
        }

        airplanes[newAirplane->getNumber()] = newAirplane;

        const AirplaneVector& _gates = _airport->getGates();
        for (unsigned int i = 0; i < gates.size() && i < _gates.size(); i++) {
            if (_gates[i] != NULL && _gates[i]->getCallsign() == newAirplane->getCallsign()) {
                gates[i] = newAirplane;
            }
        }
    }

    for (LocationMap::const_iterator it_location = this->getLocations().begin(); it_location != this->getLocations().end(); it_location++) {
        Location* newLocation = (*it_location).second;

        const Location* originalLocation = _airport->getLocationByName(newLocation->getName());
        const Location* originalPreviousLocation = originalLocation->getPreviousLocation();
        const Location* originalNextLocation = originalLocation->getNextLocation();

        if (originalPreviousLocation != NULL) {
            newLocation->setPreviousLocation(this->getLocationByName(originalPreviousLocation->getName()));
        }

        if (originalNextLocation != NULL) {
            newLocation->setNextLocation(this->getLocationByName(originalNextLocation->getName()));
        }
    }

    for (RunwayMap::const_iterator it_runway = this->getRunways().begin(); it_runway != this->getRunways().end(); it_runway++) {
        const Runway* originalRunway = (*it_runway).second;

        if (originalRunway->getAirplane() != NULL) {
            this->getRunwayByName(originalRunway->getName())->setAirplane(this->getAirplanesByRegistrationNumber(originalRunway->getAirplane()->getNumber()));
        }

        if ( originalRunway->getCrossingAirplane() != NULL) {
            this->getRunwayByName(originalRunway->getName())->setCrossingAirplane(this->getAirplanesByRegistrationNumber(originalRunway->getCrossingAirplane()->getNumber()));
        }
    }

    ENSURE(properlyInitialized(), "Airport was not properly initialized.");
}

Airport::~Airport() {
    for (AirplaneMap::iterator it = airplanes.begin(); it != airplanes.end(); it++) {
        delete (*it).second;
    }
    airplanes.clear();

    for (LocationMap::iterator it = locations.begin(); it != locations.end(); it++) {
        delete (*it).second;
    }
    locations.clear();
    runways.clear();

    gates.clear();
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

const AirplaneMap& Airport::getAirplanes() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return airplanes;
}

AirplaneMap& Airport::getAirplanes() {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return airplanes;
}

const Airplane* Airport::getAirplanesByRegistrationNumber(const std::string& number) const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    if (airplanes.find(number) == airplanes.end()) {
        return NULL;
    } else {
        return airplanes.at(number);
    }
}

Airplane* Airport::getAirplanesByRegistrationNumber(const std::string& number) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    if (airplanes.find(number) == airplanes.end()) {
        return NULL;
    } else {
        return airplanes.at(number);
    }
}

void Airport::addAirplane(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    REQUIRE(airplane != NULL, "Referenced Airplane was a nullpointer.");
    REQUIRE(airplane->getAirport() == this, "Referenced Airplane does not have a link to the current airport.");
    REQUIRE(airplanes[airplane->getNumber()] == NULL, "Referenced Airplane already added to the airport.");
    airplanes[airplane->getNumber()] = airplane;
    ENSURE(airplanes[airplane->getNumber()] == airplane, "Referenced Airplane was not properly added.");
}

const RunwayMap& Airport::getRunways() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return runways;
}

RunwayMap& Airport::getRunways() {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return runways;
}

const Runway* Airport::getRunwayByName(const std::string& name) const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    if (runways.find(name) == runways.end()) {
        return NULL;
    } else {
        return runways.at(name);
    }
}

Runway* Airport::getRunwayByName(const std::string& name) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    if (runways.find(name) == runways.end()) {
        return NULL;
    } else {
        return runways.at(name);
    }
}

Runway* Airport::getFreeCompatibleRunway(const Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");

    for (RunwayMap::iterator it_runway = runways.begin(); it_runway != runways.end(); it_runway++) {
        Runway* runway = (*it_runway).second;
        if (!runway->isVacant()) continue;
        if (airplane->getSize() == AirplaneEnums::kSmall) {
            if (airplane->getEngine() == AirplaneEnums::kPropeller) {
                if (runway->getLength() >= 500) {
                    return runway;
                }
            } else if (airplane->getEngine() == AirplaneEnums::kJet) {
                if (runway->getLength() >= 1000 && runway->getType() == RunwayEnums::kAsphalt) {
                    return runway;
                }
            }
        } else if (airplane->getSize() == AirplaneEnums::kMedium) {
            if (airplane->getEngine() == AirplaneEnums::kPropeller) {
                if (runway->getLength() >= 1000 && runway->getType() == RunwayEnums::kAsphalt) {
                    return runway;
                }
            } else if (airplane->getEngine() == AirplaneEnums::kJet) {
                if (runway->getLength() >= 2000 && runway->getType() == RunwayEnums::kAsphalt) {
                    return runway;
                }
            }
        } else if (airplane->getSize() == AirplaneEnums::kLarge) {
            if (airplane->getEngine() == AirplaneEnums::kPropeller) {
                if (runway->getLength() >= 1500 && runway->getType() == RunwayEnums::kAsphalt) {
                    return runway;
                }
            } else if (airplane->getEngine() == AirplaneEnums::kJet) {
                if (runway->getLength() >= 3000 && runway->getType() == RunwayEnums::kAsphalt) {
                    return runway;
                }
            }
        }
    }

    return NULL;
}

void Airport::addRunway(Runway* _runway) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    REQUIRE(_runway != NULL, "Referenced Runway was a nullpointer.");
    REQUIRE(_runway->getAirport() == this, "Referenced Runway does not have a link to the current airport.");
    REQUIRE(runways[_runway->getName()] == NULL, "Referenced Runway already added to the airport.");
    runways[_runway->getName()] = _runway;
    ENSURE(runways[_runway->getName()] = _runway, "Referenced Runway was not properly added.");
}

const LocationMap& Airport::getLocations() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return locations;
}

LocationMap& Airport::getLocations() {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return locations;
}

const Location* Airport::getLocationByName(const std::string& name) const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    if (locations.find(name) == locations.end()) {
        return NULL;
    } else {
        return locations.at(name);
    }
}

Location* Airport::getLocationByName(const std::string& name) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    if (locations.find(name) == locations.end()) {
        return NULL;
    } else {
        return locations.at(name);
    }
}

void Airport::addLocation(Location* location) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    REQUIRE(location != NULL, "Referenced Airplane was a nullpointer.");
    REQUIRE(location->getAirport() == this, "Referenced Airplane does not have a link to the current airport.");
    locations[location->getName()] = location;
    ENSURE(locations[location->getName()] == location, "Referenced Airplane was not properly added.");
}

const AirplaneVector& Airport::getGates() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return gates;
}

AirplaneVector& Airport::getGates() {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return gates;
}

const unsigned long Airport::getGateCount() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return gates.size();
}

bool Airport::enterGate(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    REQUIRE(airplane != NULL, "Referenced Airplane was a nullpointer.");
    REQUIRE(airplane->getAirport() == this, "Referenced Airplane is not linked to Airport.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kWaitingForEmptyGate, "Referenced Airplane is not waiting for a gate.");
    REQUIRE(airplane->getGate() == -1, "Referenced Airplane already has a gate assigned.");

    AirplaneVector::iterator it_freeGate = std::find(gates.begin(), gates.end(), static_cast<Airplane*>(NULL));

    if (it_freeGate != gates.end()) {
        long freeGate = std::distance(gates.begin(), it_freeGate);
        airplane->setGate(freeGate);
        airplane->setStatus(AirplaneEnums::kStandingAtGate);
        gates[freeGate] = airplane;
        ENSURE(airplane->getGate() == std::distance(gates.begin(), std::find(gates.begin(), gates.end(), airplane)), "Airplane was not properly given a gate.");
        ENSURE(airplane->getStatus() == AirplaneEnums::kStandingAtGate, "Airplane status was not properly updated after entering gate.");
        return true;
    } else {
        return false;
    }
}

void Airport::exitGate(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    REQUIRE(airplane->getAirport() == this, "Airplane is not linked to Airport.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStandingAtGate, "Airplane is not ready to leave gate.");
    REQUIRE(airplane->getGate() != -1, "Airplane is not properly linked to gate.");
    REQUIRE(gates[airplane->getGate()] == airplane, "Airplane is not properly linked to gate.");

    long old_gate = airplane->getGate();
    gates[old_gate] = NULL;
    airplane->setGate(-1);
    airplane->setStatus(AirplaneEnums::kPushingBack);

    ENSURE(airplane->getGate() == -1, "Airplane was not properly ungated.");
    ENSURE(gates[old_gate] == NULL, "Airplane was not properly ungated.");
    ENSURE(airplane->getStatus() == AirplaneEnums::kPushingBack, "Airplane was not properly ungated.");
}

const Airplane* Airport::get3000ft() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return ft3000;
}

void Airport::set3000ft(Airplane* _airplane) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    REQUIRE(_airplane == NULL || airplanes[_airplane->getNumber()] == _airplane, "Airplane is not recognized by this airport");
    ft3000 = _airplane;
}

bool Airport::is3000ftVacant() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return ft3000 == NULL;
}

const Airplane* Airport::get5000ft() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return ft5000;
}

void Airport::set5000ft(Airplane* _airplane) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    REQUIRE(_airplane == NULL || airplanes[_airplane->getNumber()] == _airplane, "Airplane is not recognized by this airport");
    ft5000 = _airplane;
}

bool Airport::is5000ftVacant() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return ft5000 == NULL;
}

void Airport::printInfo(std::ostream& stream) const
{
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    stream << "Airport: " << getAirportName() << " (" << getIata() << ")" << std::endl;

    stream << " -> gates: " << gates.size() << std::endl;

    stream << " -> runways: " << runways.size() << std::endl;

    for (RunwayMap::const_iterator it_runway = runways.begin(); it_runway != runways.end(); it_runway++) {
        stream << "    ~ " << (*it_runway).second->getName() << std::endl;
    }

    stream << std::endl;
}