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
        const Runway* originalRunway = it_runway->second;
        Runway* newRunway = new Runway(originalRunway);
        newRunway->setAirport(this);
        runways[newRunway->getName()] = newRunway;
        locations[newRunway->getName()] = newRunway;
    }

    const LocationMap& _locations = _airport->getLocations();
    for (LocationMap::const_iterator it_location = _locations.begin(); it_location != _locations.end(); it_location++) {
        const Location* originalLocation = it_location->second;

        if (this->getLocationByName(originalLocation->getName()) == NULL) {
            Location* newLocation = new Location(originalLocation);

            newLocation->setAirport(this);

            locations[newLocation->getName()] = newLocation;
        }
    }

    const AirplaneMap& _airplanes = _airport->getAirplanes();
    for (AirplaneMap::const_iterator it_airplane = _airplanes.begin(); it_airplane != _airplanes.end(); it_airplane++) {
        const Airplane* originalAirplane = it_airplane->second;
        Airplane* newAirplane = new Airplane(originalAirplane);
        newAirplane->setAirport(this);

        if (originalAirplane->getRunway() != NULL) {
            newAirplane->setRunway(this->getRunwayByName(originalAirplane->getRunway()->getName()));
        }

        if (originalAirplane->getCurrentLocation() != NULL) {
            newAirplane->setCurrentLocation(this->getLocationByName(originalAirplane->getCurrentLocation()->getName()));
        }

        std::queue<Location*> originalQueue = originalAirplane->getTaxiRouteCopy();
        while (!originalQueue.empty()) {
            const Location* originalLocation = originalQueue.front();
            newAirplane->getTaxiRoute().push(this->getLocationByName(originalLocation->getName()));
            originalQueue.pop();
        }

        airplanes[newAirplane->getNumber()] = newAirplane;

        const AirplaneVector& _gates = _airport->getGates();
        for (unsigned int i = 0; i < gates.size() && i < _gates.size(); i++) {
            if (_gates[i] != NULL && _gates[i]->getNumber() == newAirplane->getNumber()) {
                gates[i] = newAirplane;
            }
        }
    }

    for (LocationMap::const_iterator it_location = this->getLocations().begin(); it_location != this->getLocations().end(); it_location++) {
        Location* newLocation = it_location->second;

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
        const Runway* originalRunway = it_runway->second;

        if (originalRunway->getAirplane() != NULL) {
            this->getRunwayByName(originalRunway->getName())->setAirplane(this->getAirplanesByRegistrationNumber(originalRunway->getAirplane()->getNumber()));
        }

        if (originalRunway->getCrossingAirplane() != NULL) {
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
        Runway* runway = it_runway->second;
        if (!runway->isVacant()) continue;
        if (airplane->getSize() == AirplaneEnums::kSize_Small) {
            if (airplane->getEngine() == AirplaneEnums::kEngine_Propeller) {
                if (runway->getLength() >= 500) {
                    return runway;
                }
            } else if (airplane->getEngine() == AirplaneEnums::kEngine_Jet) {
                if (runway->getLength() >= 1000 && runway->getType() == RunwayEnums::kAsphalt) {
                    return runway;
                }
            }
        } else if (airplane->getSize() == AirplaneEnums::kSize_Medium) {
            if (airplane->getEngine() == AirplaneEnums::kEngine_Propeller) {
                if (runway->getLength() >= 1000 && runway->getType() == RunwayEnums::kAsphalt) {
                    return runway;
                }
            } else if (airplane->getEngine() == AirplaneEnums::kEngine_Jet) {
                if (runway->getLength() >= 2000 && runway->getType() == RunwayEnums::kAsphalt) {
                    return runway;
                }
            }
        } else if (airplane->getSize() == AirplaneEnums::kSize_Large) {
            if (airplane->getEngine() == AirplaneEnums::kEngine_Propeller) {
                if (runway->getLength() >= 1500 && runway->getType() == RunwayEnums::kAsphalt) {
                    return runway;
                }
            } else if (airplane->getEngine() == AirplaneEnums::kEngine_Jet) {
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

Location* Airport::getConnectionToApron() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");

    for (LocationMap::const_iterator it_location = locations.begin(); it_location != locations.end() ; it_location++) {
        if (it_location->second->getPreviousLocation() == NULL) {
            return it_location->second;
        }
    }

    return NULL;
}

std::queue<Location*> Airport::createTaxiRouteToGates(const Location* location) const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    std::queue<Location*> taxiRoute;

    while(location->getPreviousLocation() != NULL) {
        taxiRoute.push(location->getPreviousLocation());
        location = location->getPreviousLocation();
    }

    return taxiRoute;
}

std::queue<Location*> Airport::createTaxiRouteToRunway(const Runway* runway) const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    std::queue<Location*> taxiRoute;

    Location* currentLocation = getConnectionToApron()->getNextLocation();

    while(currentLocation != runway) {
        taxiRoute.push(currentLocation);
        currentLocation = currentLocation->getNextLocation();
    }

    return taxiRoute;
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

bool Airport::reserveGate(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    REQUIRE(airplane != NULL, "Referenced Airplane was a nullpointer.");
    REQUIRE(airplane->getAirport() == this, "Referenced Airplane is not linked to Airport.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_TaxiingToApron, "Referenced Airplane is not waiting for a gate.");
    REQUIRE(airplane->getGate() == -1, "Referenced Airplane already has a gate assigned.");

    AirplaneVector::iterator it_freeGate = std::find(gates.begin(), gates.end(), static_cast<Airplane*>(NULL));

    if (it_freeGate != gates.end()) {
        long freeGate = std::distance(gates.begin(), it_freeGate);
        airplane->setGate(freeGate);
        gates[freeGate] = airplane;
        ENSURE(airplane->getGate() == std::distance(gates.begin(), std::find(gates.begin(), gates.end(), airplane)), "Airplane was not properly given a gate.");
        return true;
    } else {
        return false;
    }
}

void Airport::exitGate(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    REQUIRE(airplane->getAirport() == this, "Airplane is not linked to Airport.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_PushingBack, "Airplane is not ready to leave gate.");
    REQUIRE(airplane->getGate() != -1, "Airplane is not properly linked to gate.");
    REQUIRE(gates[airplane->getGate()] == airplane, "Airplane is not properly linked to gate.");

    long old_gate = airplane->getGate();
    gates[old_gate] = NULL;
    airplane->setGate(-1);
    airplane->setStatus(AirplaneEnums::kStatus_PushingBack);

    ENSURE(airplane->getGate() == -1, "Airplane was not properly ungated.");
    ENSURE(gates[old_gate] == NULL, "Airplane was not properly ungated.");
    ENSURE(airplane->getStatus() == AirplaneEnums::kStatus_PushingBack, "Airplane was not properly ungated.");
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

void Airport::printInfo(std::ostream& stream) const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    stream << "Airport: " << getAirportName() << " (" << getIata() << ")" << std::endl;

    stream << " -> gates: " << gates.size() << std::endl;

    stream << " -> runways: " << runways.size() << std::endl;

    for (RunwayMap::const_iterator it_runway = runways.begin(); it_runway != runways.end(); it_runway++) {
        stream << "    ~ " << it_runway->second->getName() << std::endl;
    }

    stream << std::endl;
}

void Airport::printGraphicalImpression(std::ostream& stream) const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");

    unsigned int maxNameLength = 5;

    Location* currentLocation = NULL;

    for (LocationMap::const_iterator it_location = locations.begin(); it_location != locations.end(); it_location++) {
        maxNameLength = std::max<unsigned int>(maxNameLength, it_location->second->getName().length());
        if (it_location->second->getNextLocation() == NULL) {
            currentLocation = it_location->second;
        }
    }



    while(currentLocation != NULL) {
        const Runway* runway = getRunwayByName(currentLocation->getName());

        stream << currentLocation->getName();

        for (unsigned int i = currentLocation->getName().length(); i < maxNameLength; i++) {
            stream << ' ';
        }

        stream << " | ";

        if (runway != NULL) {
            char runwayCharacter = (runway->getType() == RunwayEnums::kGrass ? '-' : '=');

            char crossingCharacter = runwayCharacter;
            char takeoffCharacter = runwayCharacter;
            char busyCharacter = runwayCharacter;
            bool approaching = false;

            if (runway->getCrossingAirplane() != NULL) crossingCharacter = 'V';

            if (runway->getAirplane() != NULL) {
                switch (runway->getAirplane()->getStatus()) {
                    case AirplaneEnums::kStatus_ReadyForTakeoff:
                    case AirplaneEnums::kStatus_LiningUp:
                        takeoffCharacter = 'V';
                        break;
                    case AirplaneEnums::kStatus_TakingOff:
                    case AirplaneEnums::kStatus_Landing:
                    case AirplaneEnums::kStatus_EmergencyLanding:
                    case AirplaneEnums::kStatus_EmergencyEvacuation:
                    case AirplaneEnums::kStatus_EmergencyCheckup:
                    case AirplaneEnums::kStatus_EmergencyRefueling:
                    case AirplaneEnums::kStatus_Vacate:
                        busyCharacter = 'V';
                        break;
                    case AirplaneEnums::kStatus_FinalApproach:
                    case AirplaneEnums::kStatus_EmergencyFinalApproach:
                        approaching = true;
                        break;
                    default:
                        break;
                }
            }

            stream << takeoffCharacter << runwayCharacter << runwayCharacter << crossingCharacter << busyCharacter;

            for (unsigned int i = 4; i < static_cast<unsigned int>(runway->getLength()/100.0); ++i) {
                stream << runwayCharacter;
            }

            if (approaching) stream << "   V";

            stream << std::endl;
        } else {
            unsigned int holdingShortAirplanes = 0;
            unsigned int taxiingAirplanes = 0;

            for (AirplaneMap::const_iterator it_airplane = airplanes.begin(); it_airplane != airplanes.end(); it_airplane++) {
                if (currentLocation == it_airplane->second->getCurrentLocation()) {
                    switch (it_airplane->second->getStatus()) {
                        case AirplaneEnums::kStatus_HoldingShort:
                            holdingShortAirplanes++;
                            break;
                        case AirplaneEnums::kStatus_TaxiingToCrossing:
                        case AirplaneEnums::kStatus_TaxiingToApron:
                        case AirplaneEnums::kStatus_TaxiingToRunway:
                        case AirplaneEnums::kStatus_WaitingAtCrossing:
                            taxiingAirplanes++;
                            break;
                        default:
                            break;
                    }
                }
            }

            if (holdingShortAirplanes != 0) {
                for (unsigned int i = 0; i < holdingShortAirplanes; ++i) {
                    stream << 'V';
                }

                for (unsigned int i = holdingShortAirplanes; i < 3; ++i) {
                    stream << ' ';
                }
            } else {
                stream << "/  ";
            }

            if (taxiingAirplanes != 0) {
                for (unsigned int i = 0; i < taxiingAirplanes; ++i) {
                    stream << 'V';
                }
            } else {
                stream << '/';
            }
            stream << std::endl;
        }

        currentLocation = currentLocation->getPreviousLocation();
    }

    stream << "Gates [ ";

    for (AirplaneVector::const_iterator it_gate = gates.begin(); it_gate != gates.end(); it_gate++) {
        Airplane* airplane = *it_gate;
        std::string occupiedString = "  ";

        if (airplane != NULL) {
            if (airplane->getStatus() == AirplaneEnums::kStatus_Unboarding ||
                airplane->getStatus() == AirplaneEnums::kStatus_TechnicalCheckup ||
                airplane->getStatus() == AirplaneEnums::kStatus_Refueling ||
                airplane->getStatus() == AirplaneEnums::kStatus_Boarding ||
                airplane->getStatus() == AirplaneEnums::kStatus_StandingAtGate ||
                airplane->getStatus() == AirplaneEnums::kStatus_PushingBack
            ) {
                occupiedString = "V ";
            }
        }

        stream << occupiedString;
    }

    stream << ']' <<  std::endl;
}