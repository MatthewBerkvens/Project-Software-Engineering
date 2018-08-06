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
          gates(AirplaneVector(_gates)),
          airplanes(AirplaneVector()),
          runways(RunwayVector()),
          ft5000(NULL),
          ft3000(NULL),
          init(this) {
    ENSURE(properlyInitialized(), "Airport was not properly initialized.");
}

Airport::Airport(const Airport* _airport)
        : airportName(_airport->getAirportName()),
          iata(_airport->getIata()),
          callsign(_airport->getCallsign()),
          gates(_airport->getGateCount()),
          ft5000(_airport->get5000ft()),
          ft3000(_airport->get3000ft()),
          init(this) {
    REQUIRE(_airport->properlyInitialized(), "Reference Airport was not properly initialized.");

    const RunwayVector& _runways = _airport->getRunways();
    for (RunwayVectorConstIterator it_runway = _runways.begin(); it_runway != _runways.end(); it_runway++) {
        Runway* newRunway = new Runway(*it_runway);
        newRunway->setAirport(this);
        runways.push_back(newRunway);
    }

    const AirplaneVector& _airplanes = _airport->getAirplanes();
    for (AirplaneVectorConstIterator it_airplane = _airplanes.begin(); it_airplane != _airplanes.end(); it_airplane++) {
        Airplane* newAirplane = new Airplane(*it_airplane);
        newAirplane->setAirport(this);

        if (newAirplane->getRunway() != NULL) {
            for (RunwayVectorConstIterator it_runway = runways.begin(); it_runway != runways.end(); it_runway++) {
                if ((*it_runway)->getName() == newAirplane->getRunway()->getName()) {
                    newAirplane->setRunway(*it_runway);
                    break;
                }
            }
        }

        airplanes.push_back(newAirplane);

        const AirplaneVector& _gates = _airport->getGates();
        for (unsigned int i = 0; i < gates.size() && i < _gates.size(); i++) {
            if (_gates[i] != NULL && _gates[i]->getCallsign() == newAirplane->getCallsign()) {
                gates[i] = newAirplane;
            }
        }
    }

    ENSURE(properlyInitialized(), "Airport was not properly initialized.");
}

Airport::~Airport() {
    for (unsigned int i = 0; i < airplanes.size(); ++i) {
        delete airplanes[i];
    }
    airplanes.clear();

    for (unsigned int i = 0; i < runways.size(); ++i) {
        delete runways[i];
    }
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

const AirplaneVector& Airport::getGates() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return gates;
}

AirplaneVector& Airport::getModifiableGates() {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return gates;
}

const unsigned long Airport::getGateCount() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return gates.size();
}

const long Airport::getEmptyGate(Airplane* _airplane) const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    REQUIRE(_airplane != NULL, "Referenced Airplane was a nullpointer.");
    REQUIRE(_airplane->getAirport() == this, "Referenced Airplane is not linked to Airport.");
    REQUIRE(_airplane->getStatus() == AirplaneEnums::kWaitingForEmptyGate, "Referenced Airplane is not waiting for a gate.");
    REQUIRE(_airplane->getGate() == -1, "Referenced Airplane already has a gate assigned.");
    AirplaneVectorConstIterator it_gate =  std::find(gates.begin(), gates.end(), static_cast<Airplane*>(NULL));
    long distance = std::distance(gates.begin(), it_gate);
    if (it_gate == gates.end()) {
        distance = -1;
    }
    return distance;
}

void Airport::enterGate(Airplane* _airplane, const unsigned long _gate) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    REQUIRE(_airplane != NULL, "Referenced Airplane was a nullpointer.");
    REQUIRE(_airplane->getAirport() == this, "Referenced Airplane is not linked to Airport.");
    REQUIRE(_airplane->getStatus() == AirplaneEnums::kWaitingForEmptyGate, "Referenced Airplane is not waiting for a gate.");
    REQUIRE(_airplane->getGate() == -1, "Referenced Airplane already has a gate assigned.");
    REQUIRE(_gate < gates.size(), "Requested gate does not exist.");
    _airplane->setGate(_gate);
    gates[_gate] = _airplane;
    ENSURE(_airplane->getGate() == std::distance(gates.begin(), std::find(gates.begin(), gates.end(), _airplane)), "Airplane was not properly given a gate.");
}

void Airport::exitGate(Airplane* _airplane) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    REQUIRE(_airplane->getAirport() == this, "Airplane is not linked to Airport.");
    REQUIRE(_airplane->getStatus() == AirplaneEnums::kTaxiingToRunway, "Airplane is not linked to Airport.");
    REQUIRE(_airplane->getGate() != -1, "Airplane is not linked to Airport.");
    REQUIRE(gates[_airplane->getGate()] == _airplane, "Airplane is not properly linked to gate.");

    gates[_airplane->getGate()] = NULL;
    _airplane->setGate(-1);

    ENSURE(_airplane->getGate() == -1, "Airplane was not properly ungated.");
}

const RunwayVector& Airport::getRunways() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return runways;
}

RunwayVector& Airport::getModifiableRunways() {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return runways;
}

const AirplaneVector& Airport::getAirplanes() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return airplanes;
}

AirplaneVector& Airport::getModifiableAirplanes() {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return airplanes;
}

void Airport::addAirplane(Airplane* _airplane) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    REQUIRE(_airplane != NULL, "Referenced Airplane was a nullpointer.");
    REQUIRE(_airplane->getAirport() == this, "Referenced Airplane does not have a link to the current airport.");
    REQUIRE(!vectorContains<Airplane*>(airplanes, _airplane), "Referenced Airplane already added to the airport.");
    airplanes.push_back(_airplane);
    ENSURE(vectorContains<Airplane*>(airplanes, _airplane), "Referenced Airplane was not properly added.");
}

void Airport::addRunway(Runway* _runway) {
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

const Airplane* Airport::get3000ft() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return ft3000;
}

void Airport::set3000ft(Airplane* _airplane) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    REQUIRE(vectorContains<Airplane*>(airplanes, _airplane) || _airplane == NULL, "Airplane is not recognized by this airport");
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
    REQUIRE(vectorContains<Airplane*>(airplanes, _airplane) || _airplane == NULL, "Airplane is not recognized by this airport");
    ft5000 = _airplane;
}

bool Airport::is5000ftVacant() const {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    return ft5000 == NULL;
}

Runway* Airport::getFreeCompatibleRunway(const Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");

    for (RunwayVectorConstIterator it_runway = runways.begin(); it_runway != runways.end(); it_runway++) {
        if (airplane->getSize() == AirplaneEnums::kSmall) {
            if (airplane->getEngine() == AirplaneEnums::kPropeller) {
                if ((*it_runway)->getLength() >= 500) {
                    return *it_runway;
                }
            } else if (airplane->getEngine() == AirplaneEnums::kJet) {
                if ((*it_runway)->getLength() >= 1000 && (*it_runway)->getType() == RunwayEnums::kAsphalt) {
                    return *it_runway;
                }
            }
        } else if (airplane->getSize() == AirplaneEnums::kMedium) {
            if (airplane->getEngine() == AirplaneEnums::kPropeller) {
                if ((*it_runway)->getLength() >= 1000 && (*it_runway)->getType() == RunwayEnums::kAsphalt) {
                    return *it_runway;
                }
            } else if (airplane->getEngine() == AirplaneEnums::kJet) {
                if ((*it_runway)->getLength() >= 2000 && (*it_runway)->getType() == RunwayEnums::kAsphalt) {
                    return *it_runway;
                }
            }
        } else if (airplane->getSize() == AirplaneEnums::kLarge) {
            if (airplane->getEngine() == AirplaneEnums::kPropeller) {
                if ((*it_runway)->getLength() >= 1500 && (*it_runway)->getType() == RunwayEnums::kAsphalt) {
                    return *it_runway;
                }
            } else if (airplane->getEngine() == AirplaneEnums::kJet) {
                if ((*it_runway)->getLength() >= 3000 && (*it_runway)->getType() == RunwayEnums::kAsphalt) {
                    return *it_runway;
                }
            }
        }
    }

    return NULL;
}

void Airport::printInfo(std::ostream& stream) const
{
    REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
    stream << "Airport: " << getAirportName() << " (" << getIata() << ")" << std::endl;

    stream << " -> gates: " << gates.size() << std::endl;

    stream << " -> runways: " << runways.size() << std::endl;

    for (RunwayVectorConstIterator it_runway = runways.begin(); it_runway != runways.end(); it_runway++) {
        stream << "    ~ " << (*it_runway)->getName() << std::endl;
    }

    stream << std::endl;
}