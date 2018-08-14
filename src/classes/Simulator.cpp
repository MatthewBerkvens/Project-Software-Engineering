#include "Simulator.h"

bool Simulator::properlyInitialized() const {
    return this == init;
}

Simulator::Simulator(const Airport* _airport, std::ostream& _outputStream, std::ostream& _errorStream, std::ostream& _airTrafficControllerStream)
    : timer(0),
      airport(new Airport(_airport)),
      airTrafficController(_airTrafficControllerStream),
      outputStream(_outputStream),
      errorStream(_errorStream),
      init(this) {
    ENSURE(properlyInitialized(), "Simulator was not properly initialized");
}

Simulator::~Simulator() {
    delete airport;
}

void Simulator::Simulate() {
    for (AirplaneMap::iterator it_airplane = airport->getAirplanes().begin(); it_airplane != airport->getAirplanes().end(); it_airplane++) {
        if (it_airplane->second->getStatus() == AirplaneEnums::kStatus_Approaching) {
            outputStream << getRealisticTimeStamp() << ' ' << it_airplane->second->getCallsign() << " is approaching" << std::endl;
        } else if (it_airplane->second->getStatus() == AirplaneEnums::kStatus_StandingAtGate)  {
            outputStream << getRealisticTimeStamp() << ' ' << it_airplane->second->getCallsign() << " is standing at gate " << it_airplane->second->getGate() + 1 << std::endl;
        } else {
            errorStream << "Airport is not consistent? This should never happen.";
        }
    }
    
    timer++;

    while (!SimulationFinished()) {
        std::priority_queue<Airplane*, std::vector<Airplane*>, CompareSquawk> airplaneQueue;
        for (AirplaneMap::iterator it_airplane = airport->getAirplanes().begin(); it_airplane != airport->getAirplanes().end(); it_airplane++) {
            airplaneQueue.push(it_airplane->second);
        }

        while (!airplaneQueue.empty()) {
            Airplane* airplane = airplaneQueue.top();
            airplaneQueue.pop();

            if (airplane->isFlying()) {
                if (!airplane->fly()) {
                    if (airplane->getStatus() != AirplaneEnums::kStatus_Emergency &&
                        airplane->getStatus() != AirplaneEnums::kStatus_EmergencyLanding &&
                        airplane->getStatus() != AirplaneEnums::kStatus_EmergencyFinalApproach
                    ) {
                        airplane->setStatus(AirplaneEnums::kStatus_Emergency);
                        airplane->setCommunicationTimer(0);
                        airplane->setActionTimer(0);
                        airplane->setPermission(false);
                    }
                }
            }

            switch(airplane->getStatus()) {
                case AirplaneEnums::kStatus_Approaching:
                    Approach(airplane);
                    break;
                case AirplaneEnums::kStatus_DescendingTo5000ft:
                    DescendTo5000ft(airplane);
                    break;
                case AirplaneEnums::kStatus_DescendingTo3000ft:
                    DescendTo3000ft(airplane);
                    break;
                case AirplaneEnums::kStatus_FlyingWaitPattern:
                    FlyWaitingPattern(airplane);
                    break;
                case AirplaneEnums::kStatus_FinalApproach:
                    FinalApproach(airplane);
                    break;
                case AirplaneEnums::kStatus_Landing:
                    Land(airplane);
                    break;
                case AirplaneEnums::kStatus_Vacate:
                    Vacate(airplane);
                    break;
                case AirplaneEnums::kStatus_CrossingRunway:
                    CrossRunway(airplane);
                    break;
                case AirplaneEnums::kStatus_TaxiingToCrossing:
                    TaxiToCrossing(airplane);
                    break;
                case AirplaneEnums::kStatus_TaxiingToRunway:
                    TaxiToRunway(airplane);
                    break;
                case AirplaneEnums::kStatus_WaitingAtCrossing:
                    WaitingAtCrossing(airplane);
                    break;
                case AirplaneEnums::kStatus_TaxiingToApron:
                    TaxiToApron(airplane);
                    break;
                case AirplaneEnums::kStatus_Unboarding:
                    UnboardAirplane(airplane);
                    break;
                case AirplaneEnums::kStatus_TechnicalCheckup:
                    TechnicalCheckup(airplane);
                    break;
                case AirplaneEnums::kStatus_Refueling:
                    Refuel(airplane);
                    break;
                case AirplaneEnums::kStatus_Boarding:
                    BoardAirplane(airplane);
                    break;
                case AirplaneEnums::kStatus_StandingAtGate:
                    StandAtGate(airplane);
                    break;
                case AirplaneEnums::kStatus_PushingBack:
                    Pushback(airplane);
                    break;
                case AirplaneEnums::kStatus_HoldingShort:
                    HoldShort(airplane);
                    break;
                case AirplaneEnums::kStatus_LiningUp:
                    LineUp(airplane);
                    break;
                case AirplaneEnums::kStatus_ReadyForTakeoff:
                    ReadyForTakeoff(airplane);
                    break;
                case AirplaneEnums::kStatus_TakingOff:
                    Takeoff(airplane);
                    break;
                case AirplaneEnums::kStatus_Ascending:
                    Ascend(airplane);
                    break;
                case AirplaneEnums::kStatus_Emergency:
                    Emergency(airplane);
                    break;
                case AirplaneEnums::kStatus_EmergencyFinalApproach:
                    EmergencyFinalApproach(airplane);
                    break;
                case AirplaneEnums::kStatus_EmergencyLanding:
                    EmergencyLand(airplane);
                    break;
                case AirplaneEnums::kStatus_EmergencyEvacuation:
                    EmergencyEvacuate(airplane);
                    break;
                case AirplaneEnums::kStatus_EmergencyCheckup:
                    EmergencyCheckup(airplane);
                    break;
                case AirplaneEnums::kStatus_EmergencyRefueling:
                    EmergencyRefuel(airplane);
                    break;
                default:
                    break;
            }
        }

        airport->printGraphicalImpression(outputStream);

        timer++;
        outputStream << std::endl;
        airTrafficController << std::endl;
        if (timer / 11520.0 > 1.0) break;
    }
}

bool Simulator::SimulationFinished() {
    for (AirplaneMap::iterator it_airplane = airport->getAirplanes().begin(); it_airplane != airport->getAirplanes().end(); it_airplane++) {
        if (it_airplane->second->getStatus() != AirplaneEnums::kStatus_LeftAirport && it_airplane->second->getStatus() != AirplaneEnums::kStatus_Crashed) return false;
    }

    return true;
}

std::string Simulator::getRealisticTimeStamp() const {
    std::stringstream result;

    unsigned int days = static_cast<unsigned int>(std::floor((timer + 720) / 1440.0));
    unsigned int hours = static_cast<unsigned int>(std::floor((timer + 720 - (days * 1440)) / 60.0));
    unsigned int minutes = static_cast<unsigned int>(std::floor((timer + 720) - (days * 1440) - (hours * 60)));

    result << '[';

    if (days % 7 == 0) result << "Monday ";
    else if (days % 7 == 1) result << "Tuesday ";
    else if (days % 7 == 2) result << "Wednesday ";
    else if (days % 7 == 3) result << "Thursday ";
    else if (days % 7 == 4) result << "Friday ";
    else if (days % 7 == 5) result << "Saturday ";
    else if (days % 7 == 6) result << "Sunday ";

    result << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2) << std::setfill('0') << minutes << ']';

    return result.str();
}

void Simulator::Approach(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_Approaching, "Airplane is not in the correct status.");

    if (airplane->getCommunicationTimer() == 0) {
        airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
        airTrafficController << "$ " << airport->getCallsign() << ", " << airplane->getCallsign() << ", arriving at " << airport->getAirportName() << '.' << std::endl;

        airplane->increaseCommunicationTimer();
    } else if (airplane->getCommunicationTimer() == 1) {
        if (airport->is5000ftVacant()) {
            airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
            airTrafficController << "$ " << airplane->getCallsign() << ", radar contact, descend and maintain five thousand feet, squawk " << std::oct << airplane->getSquawk() << std::dec << '.' << std::endl;

            airport->set5000ft(airplane);
            airplane->increaseCommunicationTimer();
        }
    } else if (airplane->getCommunicationTimer() == 2) {
        airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
        airTrafficController << "$ Descend and maintain five thousand feet, squawking " << std::oct << airplane->getSquawk() << std::dec << ", " << airplane->getCallsign() << "." << std::endl;

        outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " has begun its descent to 5000 ft." << std::endl;

        airplane->setStatus(AirplaneEnums::kStatus_DescendingTo5000ft);

        airplane->setActionTimer(0);
        airplane->setCommunicationTimer(0);
        airplane->setPermission(false);
    }
}

void Simulator::DescendTo5000ft(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_DescendingTo5000ft, "Airplane is not in the correct status.");
    REQUIRE(airplane == airport->get5000ft(), "Airplane descending to 5000 ft. is not linked to airport");

    if (airplane->getAltitude() > 5000) {
        airplane->increaseActionTimer();

        if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
            if (airplane->descend(1000)) {
                outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " descended to " << airplane->getAltitude() << " ft." << std::endl;

                airplane->setActionTimer(0);
            } else {
                errorStream << "Shouldn't happen" << std::endl;
            }
        }
    }

    if (airplane->getAltitude() <= 5000) {
        if (airplane->getCommunicationTimer() == 0) {
            airplane->increaseCommunicationTimer();

            if(airport->is3000ftVacant()) {
                airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                airTrafficController << "$ " << airplane->getCallsign() << ", descend and maintain three thousand feet." << std::endl;

                airplane->setPermission(true);
            } else {
                airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                airTrafficController << "$ " << airplane->getCallsign() << ", hold south on the one eighty radial, expect further clearance in " <<
                    static_cast<unsigned int>(std::floor(airport->get3000ft()->getAltitude() / (airport->get3000ft()->getEngine() == AirplaneEnums::kEngine_Jet ? 1000.0 : 500.0))) << " minutes." << std::endl;

                airplane->setPermission(false);
            }
        } else if (airplane->getCommunicationTimer() == 1) {
            if (airplane->hasPermission()) {
                airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
                airTrafficController << "$ Descend and maintain three thousand feet, " << airplane->getCallsign() << '.' << std::endl;

                outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " has begun its descent to 3000 ft." << std::endl;

                airplane->setStatus(AirplaneEnums::kStatus_DescendingTo3000ft);

                airport->set5000ft(NULL);
                airport->set3000ft(airplane);

                airplane->setActionTimer(0);
                airplane->setCommunicationTimer(0);
                airplane->setPermission(false);
            } else {
                airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
                airTrafficController << "$ Holding south on the one eighty radial, " << airplane->getCallsign() << '.' << std::endl;

                outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " is flying in a waiting pattern at 5000 ft." << std::endl;

                airplane->setStatus(AirplaneEnums::kStatus_FlyingWaitPattern);

                airplane->setActionTimer(0);
                airplane->setCommunicationTimer(0);
                airplane->setPermission(false);
            }
        }
    }
}

void Simulator::DescendTo3000ft(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_DescendingTo3000ft, "Airplane is not in the correct status.");
    REQUIRE(airplane == airport->get3000ft(), "Airplane is not the one descending to 5000 ft");

    if (airplane->getAltitude() > 3000) {
        airplane->increaseActionTimer();
        if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
            if(airplane->descend(1000)) {
                outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " descended to " << airplane->getAltitude() << " ft." << std::endl;

                airplane->setActionTimer(0);
            } else {
                errorStream << "Shouldn't happen" << std::endl;
            }
        }
    }

    if (airplane->getAltitude() <= 3000) {
        if (airplane->getCommunicationTimer() == 0) {
            airplane->increaseCommunicationTimer();

            Runway* runway = airport->getFreeCompatibleRunway(airplane);

            if(runway != NULL) {
                airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                airTrafficController << "$ " << airplane->getCallsign() << ", cleared ILS approach runway " << runway->getName() << '.' << std::endl;

                airplane->setRunway(runway);
                runway->setAirplane(airplane);

                airplane->setPermission(true);
            } else {
                airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                airTrafficController << "$ " << airplane->getCallsign() << ", hold south on the one eighty radial, expect further clearance in 3 minutes." << std::endl;

                airplane->setPermission(false);
            }
        } else if (airplane->getCommunicationTimer() == 1) {
            if (airplane->hasPermission()) {
                airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
                airTrafficController << "$ Cleared ILS approach runway " << airplane->getRunway()->getName() << ", " << airplane->getCallsign() << '.' << std::endl;

                outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " has begun its final approach to runway " << airplane->getRunway()->getName() << " at " << airport->getAirportName() << "." << std::endl;

                airplane->setStatus(AirplaneEnums::kStatus_FinalApproach);
                airport->set3000ft(NULL);

                airplane->setActionTimer(0);
                airplane->setCommunicationTimer(0);
                airplane->setPermission(false);
            } else {
                airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
                airTrafficController << "$ Holding south on the one eighty radial, " << airplane->getCallsign() << '.' << std::endl;

                outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " is flying in a waiting pattern at 3000 ft." << std::endl;

                airplane->setStatus(AirplaneEnums::kStatus_FlyingWaitPattern);

                airplane->setActionTimer(0);
                airplane->setCommunicationTimer(0);
                airplane->setPermission(false);
            }
        }
    }
}

void Simulator::FlyWaitingPattern(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_FlyingWaitPattern, "Airplane is not in the correct status.");

    if (airplane->getAltitude() == 5000) {
        if (airplane->getCommunicationTimer() == 0) {
            if (airport->is3000ftVacant()) {
                airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                airTrafficController << "$ " << airplane->getCallsign() << ", descend and maintain three thousand feet." << std::endl;

                airplane->setPermission(true);
                airplane->increaseCommunicationTimer();
            }
        } else if (airplane->getCommunicationTimer() == 1) {
            airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
            airTrafficController << "$ Descend and maintain three thousand feet, " << airplane->getCallsign() << '.' << std::endl;

            outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " has begun its descent to 3000 ft" << std::endl;

            airport->set3000ft(airplane);
            airport->set5000ft(NULL);
            airplane->setStatus(AirplaneEnums::kStatus_DescendingTo3000ft);

            airplane->setActionTimer(0);
            airplane->setCommunicationTimer(0);
            airplane->setPermission(false);
        }
    }

    if (airplane->getAltitude() == 3000) {
        if (airplane->getCommunicationTimer() == 0) {
            Runway* runway = airport->getFreeCompatibleRunway(airplane);

            if (runway != NULL) {
                airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                airTrafficController << "$ " << airplane->getCallsign() << ", cleared ILS approach runway " << runway->getName() << '.' << std::endl;

                airport->set3000ft(NULL);
                airplane->setRunway(runway);
                runway->setAirplane(airplane);

                airplane->increaseCommunicationTimer();
                airplane->setPermission(true);
            }
        } else if (airplane->getCommunicationTimer() == 1) {
            airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
            airTrafficController << "$ Cleared ILS approach runway " << airplane->getRunway()->getName() << ", " << airplane->getCallsign() << '.' << std::endl;

            outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " has begun its final approach to runway " << airplane->getRunway()->getName() << " at " << airport->getAirportName() << "." << std::endl;

            airplane->setStatus(AirplaneEnums::kStatus_FinalApproach);
            airport->set3000ft(NULL);

            airplane->setActionTimer(0);
            airplane->setCommunicationTimer(0);
            airplane->setPermission(false);
        }
    }
}

void Simulator::FinalApproach(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_FinalApproach, "Airplane is not in the correct status.");

    airplane->increaseActionTimer();
    if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
        if (airplane->descend(1000)) {
            airplane->setActionTimer(0);

            outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " has descended to " << airplane->getAltitude() << " ft" << std::endl;
        } else {
            errorStream << "Shouldn't happen" << std::endl;
        }
    }

    if (airplane->getAltitude() == 0) {
        airplane->setStatus(AirplaneEnums::kStatus_Landing);
        airplane->setActionTimer(0);
    }
}

void Simulator::Land(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_Landing, "Airplane is not in the correct status.");

    airplane->increaseActionTimer();
    if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
        outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " has landed on runway " << airplane->getRunway()->getName() << std::endl;

        airplane->setStatus(AirplaneEnums::kStatus_Vacate);
        airplane->setCurrentLocation(airplane->getRunway());
    }
}

void Simulator::Vacate(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_Vacate, "Airplane is not in the correct status.");

    outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " has vacated runway " << airplane->getRunway()->getName() << std::endl;

    airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
    airTrafficController << "$ " << airport->getCallsign() << ", " << airplane->getCallsign() << " runway " << airplane->getRunway()->getName() << " vacated." << std::endl;

    airplane->setCurrentLocation(airplane->getRunway()->getPreviousLocation());
    airplane->getRunway()->setAirplane(NULL);
    airplane->setRunway(NULL);
    airplane->setTaxiRoute(airport->createTaxiRouteToGates(airplane->getCurrentLocation()));
    if (airplane->getTaxiRoute().size() == 0) {
        airplane->setStatus(AirplaneEnums::kStatus_TaxiingToApron);
    } else {
        airplane->setStatus(AirplaneEnums::kStatus_TaxiingToCrossing);
    }
}

void Simulator::TaxiToCrossing(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_TaxiingToCrossing, "Airplane is not in the correct status.");

    switch (airplane->getCommunicationTimer()) {
        case 0:
            if (airplane->getActionTimer() == 0) {
                airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
                airTrafficController << "$ Taxi to holding point " << airplane->getTaxiRoute().front()->getName() << " via " << airplane->getCurrentLocation()->getName() << '.' << std::endl;
            }

            airplane->increaseActionTimer();
            if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
                airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
                airTrafficController << "$ " << airport->getCallsign() << ", " << airplane->getCallsign() << ", holding short at " << airplane->getTaxiRoute().front()->getName() << '.' << std::endl;
                airplane->increaseCommunicationTimer();
            }
            break;
        case 1:
            Runway* crossingRunway = airport->getRunwayByName(airplane->getTaxiRoute().front()->getName());

            if (crossingRunway != NULL) {
                if (crossingRunway->canCross()) {
                    airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                    airTrafficController << "$ " << airplane->getCallsign() << ", cleared to cross " << airplane->getTaxiRoute().front()->getName() << '.' << std::endl;

                    crossingRunway->setCrossingAirplane(airplane);
                    airplane->setCurrentLocation(airplane->getTaxiRoute().front());
                    airplane->getTaxiRoute().pop();

                    airplane->setStatus(AirplaneEnums::kStatus_CrossingRunway);
                    airplane->setCommunicationTimer(0);
                    airplane->setActionTimer(0);
                    airplane->setPermission(false);
                } else {
                    airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                    airTrafficController << "$ " << airport->getCallsign() << ", hold position." << std::endl;

                    airplane->setStatus(AirplaneEnums::kStatus_WaitingAtCrossing);
                    airplane->setCommunicationTimer(0);
                    airplane->setActionTimer(0);
                    airplane->setPermission(false);
                }
            } else {
                errorStream << "shouldn't happen" << std::endl;
            }
            break;
    }
}

void Simulator::WaitingAtCrossing(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_WaitingAtCrossing, "Airplane is not in the correct status.");

    switch (airplane->getCommunicationTimer()) {
        case 0:
            airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
            airTrafficController << "$ Holding position, " << airplane->getCallsign() << '.' << std::endl;
            airplane->increaseCommunicationTimer();
        case 1:
            Runway* crossingRunway = airport->getRunwayByName(airplane->getTaxiRoute().front()->getName());

            if (crossingRunway != NULL) {
                if (crossingRunway->canCross()) {
                    airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                    airTrafficController << "$ " << airplane->getCallsign() << ", cleared to cross " << airplane->getTaxiRoute().front()->getName() << '.' << std::endl;

                    crossingRunway->setCrossingAirplane(airplane);
                    airplane->setCurrentLocation(airplane->getTaxiRoute().front());
                    airplane->getTaxiRoute().pop();

                    airplane->setStatus(AirplaneEnums::kStatus_CrossingRunway);
                    airplane->setCommunicationTimer(0);
                    airplane->setActionTimer(0);
                    airplane->setPermission(false);
                }
            } else {
                errorStream << "shouldn't happen" << std::endl;
            }
            break;
    }
}

void Simulator::CrossRunway(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_CrossingRunway, "Airplane is not in the correct status.");

    switch (airplane->getCommunicationTimer()) {
        case 0:
            airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
            airTrafficController << "$ Cleared to cross " << airplane->getTaxiRoute().front()->getName() << airplane->getCallsign() << '.' << std::endl;
            airplane->increaseCommunicationTimer();
            break;
        case 1:
            airplane->increaseActionTimer();
            if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
                Runway* crossingRunway = airport->getRunwayByName(airplane->getCurrentLocation()->getName());

                crossingRunway->setCrossingAirplane(NULL);
                airplane->setCurrentLocation(airplane->getTaxiRoute().front());
                outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " is now on taxipoint " << airplane->getCurrentLocation()->getName() << std::endl;
                airplane->getTaxiRoute().pop();

                airplane->setCommunicationTimer(0);
                airplane->setActionTimer(0);
                airplane->setPermission(false);

                if (airplane->getTaxiRoute().empty()) {
                    if (airplane->getRunway() == NULL) {
                        airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                        airTrafficController << "$ " << airplane->getCallsign() << ", taxi to apron via " << airplane->getCurrentLocation()->getName() << '.' << std::endl;

                        airplane->setStatus(AirplaneEnums::kStatus_TaxiingToApron);
                    } else {
                        airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                        airTrafficController << "$ " << airplane->getCallsign() << ", taxi to runway " << airplane->getRunway()->getName() << " via " << airplane->getCurrentLocation()->getName() << '.' << std::endl;

                        airplane->setStatus(AirplaneEnums::kStatus_TaxiingToRunway);
                    }
                } else {
                    airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                    airTrafficController << "$ " << airplane->getCallsign() << ", taxi to holding point " << airplane->getTaxiRoute().front()->getName() << " via " << airplane->getCurrentLocation()->getName() << '.' << std::endl;

                    airplane->setStatus(AirplaneEnums::kStatus_TaxiingToCrossing);
                }
            }
            break;
    }
}

void Simulator::TaxiToApron(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_TaxiingToApron, "Airplane is not in the correct status.");

    switch (airplane->getCommunicationTimer()) {
        case 0:
            airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
            airTrafficController << "$ Taxi to apron via " << airplane->getCurrentLocation()->getName() << ", " << airplane->getCallsign() << '.' << std::endl;
            airplane->increaseCommunicationTimer();
            break;
        case 1:
            airplane->increaseActionTimer();
            if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
                if (airport->reserveGate(airplane)) {
                    airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                    airTrafficController << "$ " << airplane->getCallsign() << ", taxi to gate " << airplane->getGate() + 1 << '.' << std::endl;
                    airplane->increaseCommunicationTimer();
                }
            }
            break;
        case 2:
            airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
            airTrafficController << "$ Taxi to gate " << airplane->getGate() + 1 << ", " << airplane->getCallsign() << '.' << std::endl;

            outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " has entered gate " << airplane->getGate() + 1 << std::endl;

            airplane->setCurrentLocation(NULL);
            airplane->setStatus(AirplaneEnums::kStatus_Unboarding);
            airplane->setCommunicationTimer(0);
            airplane->setActionTimer(0);
            airplane->setPermission(false);
            break;
    }
}

void Simulator::TaxiToRunway(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_TaxiingToRunway, "Airplane is not in the correct status.");

    switch (airplane->getCommunicationTimer()) {
        case 0:
            airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
            airTrafficController << "$ " << airplane->getCallsign() << ", taxi to runway " << airplane->getRunway()->getName() << " via " << airplane->getCurrentLocation()->getName() << '.' << std::endl;
            airplane->increaseCommunicationTimer();
            break;
        case 1:
            if (airplane->getActionTimer() == 0) {
                airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
                airTrafficController << "$ Taxi to runway " << airplane->getRunway()->getName() << " via " << airplane->getCurrentLocation()->getName() << ", " << airplane->getCallsign() << '.' << std::endl;
            }

            airplane->increaseActionTimer();
            if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
                airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
                airTrafficController << "$ " << airport->getCallsign() << ", " << airplane->getCallsign() << ", holding short at " << airplane->getRunway()->getName() << '.' << std::endl;

                outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " is holding short at runway " << airplane->getRunway()->getName() << std::endl;

                airplane->setStatus(AirplaneEnums::kStatus_HoldingShort);
                airplane->setCommunicationTimer(0);
                airplane->setActionTimer(0);
                airplane->setPermission(false);
            }
            break;
    }
}

void Simulator::UnboardAirplane(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_Unboarding, "Airplane is not in the correct status.");
    airplane->increaseActionTimer();
    if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
        outputStream << getRealisticTimeStamp() << ' ' << airplane->getPassengers() << " passengers exited " << airplane->getCallsign()
                    << " at gate " << airplane->getGate() + 1 << " of " << airport->getAirportName() << std::endl;
        airplane->setPassengers(0);

        airplane->setStatus(AirplaneEnums::kStatus_TechnicalCheckup);
        airplane->setActionTimer(0);
    }
}

void Simulator::TechnicalCheckup(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_TechnicalCheckup, "Airplane is not in the correct status.");
    airplane->increaseActionTimer();
    if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
        outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " has been checked for technical malfunctions" << std::endl;

        airplane->setStatus(AirplaneEnums::kStatus_Refueling);
        airplane->setActionTimer(0);
    }
}

void Simulator::Refuel(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_Refueling, "Airplane is not in the correct status.");

    airplane->increaseActionTimer();
    if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
        outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " has been refueled (" << airplane->getFuelCapacity() - airplane->getFuel() << " units)" << std::endl;
        airplane->setFuel(airplane->getFuelCapacity());

        airplane->setStatus(AirplaneEnums::kStatus_Boarding);
        airplane->setActionTimer(0);
    }
}

void Simulator::BoardAirplane(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_Boarding, "Airplane is not in the correct status.");

    airplane->increaseActionTimer();
    if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
        airplane->setPassengers(airplane->getPassengerCapacity());
        outputStream << getRealisticTimeStamp() << ' ' << airplane->getPassengers() << " passengers boarded " << airplane->getCallsign() << " at gate " << airplane->getGate() + 1 << " of " << airport->getAirportName() << std::endl;

        airplane->setStatus(AirplaneEnums::kStatus_StandingAtGate);
        airplane->setActionTimer(0);
    }
}

void Simulator::StandAtGate(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_StandingAtGate, "Airplane is not in the correct status.");

    switch(airplane->getCommunicationTimer()) {
        case 0:
            {
                Runway* runway = airport->getFreeCompatibleRunway(airplane);

                if (runway != NULL) {
                    airplane->setTaxiRoute(airport->createTaxiRouteToRunway(runway));
                    airplane->setRunway(runway);
                    airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
                    airTrafficController << "$ " << airport->getCallsign() << ", " << airplane->getCallsign() << ", requesting IFR clearancy to <my destination - no flightplan implemented>." << std::endl;
                    airplane->increaseCommunicationTimer();
                }
            }
            break;
        case 1:
            airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
            airTrafficController << "$ " << airplane->getCallsign() << ", " << airport->getCallsign()
                                 << ", cleared to <your destination - no flightplan implemented>, maintain five thousand, expect flight level one zero zero - ten minutes after departure, squawk "
                                 << std::oct << airplane->getSquawk() << std::dec << '.' << std::endl;

            airplane->increaseCommunicationTimer();
            break;
        case 2:
            airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
            airTrafficController << "$ Cleared to <my destination - no flightplan implemented>, initial altitude five thousand, expecting one zero zero in ten, squawking "
                                 << std::oct << airplane->getSquawk() << std::dec << '.' << std::endl;

            airplane->setStatus(AirplaneEnums::kStatus_PushingBack);
            airplane->setActionTimer(0);
            airplane->setCommunicationTimer(0);
            airplane->setPermission(false);
            break;
    }
}

void Simulator::Pushback(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_PushingBack, "Airplane is not in the correct status.");

    switch(airplane->getCommunicationTimer()) {
        case 0:
            airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
            airTrafficController << "$ " << airport->getCallsign() << ", " << airplane->getCallsign() << ", at gate " << airplane->getGate() + 1 << ", requesting pushback." << std::endl;
            airplane->increaseCommunicationTimer();
            break;
        case 1:
            airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
            airTrafficController << "$ " << airplane->getCallsign() << ", " << airport->getCallsign() << ", pushback approved." << std::endl;
            airplane->increaseCommunicationTimer();
            break;
        case 2:
            if (airplane->getActionTimer() == 0) {
                airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
                airTrafficController << "$ Pushback approved, " << airplane->getCallsign() << '.' << std::endl;

                outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " is pushing back from gate " << airplane->getGate() + 1 << std::endl;
            }

            airplane->increaseActionTimer();
            if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
                outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " succesfully pushed back from gate " << airplane->getGate() + 1 << std::endl;
                airport->exitGate(airplane);
                airplane->setCurrentLocation(airport->getConnectionToApron());
                airplane->setActionTimer(0);
                airplane->increaseCommunicationTimer();
            }
            break;
        case 3:
            airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
            airTrafficController << "$ " << airplane->getCallsign() << " is ready to taxi." << std::endl;

            airplane->setActionTimer(0);
            airplane->setCommunicationTimer(0);
            airplane->setPermission(false);

            if (airplane->getTaxiRoute().size() == 0) {
                airplane->setStatus(AirplaneEnums::kStatus_TaxiingToRunway);
            } else {
                airplane->setStatus(AirplaneEnums::kStatus_TaxiingToCrossing);
            }
            break;
    }
}

void Simulator::HoldShort(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_HoldingShort, "Airplane is not in the correct status.");

    switch(airplane->getCommunicationTimer()) {
        case 0:
            airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
            airTrafficController << "$ " << airport->getCallsign() << ", " << airplane->getCallsign() << ", holding short at " << airplane->getRunway()->getName() << '.' << std::endl;
            airplane->increaseCommunicationTimer();
            break;
        case 1:
            if (airplane->getRunway()->isVacant()) {
                airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                airTrafficController << "$ " << airplane->getCallsign() << ", line-up at runway " << airplane->getRunway()->getName() << " and wait." << std::endl;

                outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " is now lining up on runway " << airplane->getRunway()->getName() << std::endl;

                airplane->setCurrentLocation(airplane->getRunway());
                airplane->getRunway()->setAirplane(airplane);

                airplane->setStatus(AirplaneEnums::kStatus_LiningUp);
                airplane->setCommunicationTimer(0);
                airplane->setActionTimer(0);
                airplane->setPermission(false);
            }
            break;
    }
}

void Simulator::LineUp(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_LiningUp, "Airplane is not in the correct status.");

    /*if (airplane->getActionTimer() == 0) {
        airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
        airTrafficController << "$ " << airport->getCallsign() << ", " << airplane->getCallsign() << ", holding short at " << airplane->getRunway()->getName() << '.' << std::endl;
    }*/
    //?

    airplane->increaseActionTimer();
    if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
        airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
        airTrafficController << "$ " << airport->getCallsign() << ", " << airplane->getCallsign() << ", finished line-up at " << airplane->getRunway()->getName() << " and ready for take off." << std::endl;

        outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " has lined up on runway " << airplane->getRunway()->getName() << std::endl;

        airplane->setStatus(AirplaneEnums::kStatus_ReadyForTakeoff);
        airplane->setCommunicationTimer(0);
        airplane->setActionTimer(0);
        airplane->setPermission(false);
    }
}

void Simulator::ReadyForTakeoff(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_ReadyForTakeoff, "Airplane is not in the correct status.");

    switch(airplane->getCommunicationTimer()) {
        case 0:
            airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
            airTrafficController << "$ " << airplane->getCallsign() << ", runway " << airplane->getRunway()->getName() << " cleared for take-off." << std::endl;
            airplane->increaseCommunicationTimer();
            break;
        case 1:
            airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
            airTrafficController << "$ Runway " << airplane->getRunway()->getName() << " cleared for take-off, " << airplane->getCallsign() << '.' << std::endl;

            outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " is taking off" << std::endl;

            airplane->setStatus(AirplaneEnums::kStatus_TakingOff);
            break;
    }
}

void Simulator::Takeoff(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_TakingOff, "Airplane is not in the correct status.");

    airplane->increaseActionTimer();
    if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
        outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " has taken off from runway " << airplane->getRunway()->getName() << std::endl;

        airplane->getRunway()->setAirplane(NULL);
        airplane->setCurrentLocation(NULL);
        airplane->setRunway(NULL);

        airplane->setStatus(AirplaneEnums::kStatus_Ascending);
        airplane->setActionTimer(0);
    }
}

void Simulator::Ascend(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_Ascending, "Airplane is not in the correct status.");

    airplane->increaseActionTimer();
    if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
        airplane->ascend(1000);

        if (airplane->getAltitude() >= 5000) {
            outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " ascended to " << airplane->getAltitude() << " ft. and has left " << airport->getAirportName() << std::endl;

            airplane->setStatus(AirplaneEnums::kStatus_LeftAirport);
            airplane->setActionTimer(0);
        } else {
            outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " ascended to " << airplane->getAltitude() << " ft." << std::endl;
        }
    }
}


void Simulator::Emergency(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_Emergency, "Airplane is not in the correct status.");

    switch (airplane->getCommunicationTimer()) {
        case 0:
            if (airplane->getAltitude() < 3000) {
                if (airplane->getRunway() == NULL) {
                    errorStream << "Shouldn't happen" << std::endl;
                }

                airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
                airTrafficController << "$ Mayday mayday mayday, " << airport->getCallsign() << ", " << airplane->getCallsign() << ", out of fuel, performing emergency landing, "
                                     << airplane->getPassengers() << " passengers on board." << std::endl;

                airplane->setStatus(AirplaneEnums::kStatus_FinalApproach);
                airplane->setCommunicationTimer(0);
                airplane->setActionTimer(0);
                airplane->setPermission(false);
            } else {
                airTrafficController << getRealisticTimeStamp() << '[' << airplane->getNumber() << ']' << std::endl;
                airTrafficController << "$ Mayday mayday mayday, " << airport->getCallsign() << ", " << airplane->getCallsign() << ", out of fuel, request immediate landing, "
                                     << airplane->getPassengers() << " passengers on board." << std::endl;
                airplane->increaseCommunicationTimer();
            }
            break;
        case 1:
            {
                Runway* runway = airport->getFreeCompatibleRunway(airplane);

                if (runway != NULL) {
                    airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                    airTrafficController << "$ " << airplane->getCallsign() << " roger mayday, squawk seven seven zero zero, cleared ILS landing runway " << runway->getName() << '.' << std::endl;

                    airplane->setRunway(runway);
                    runway->setAirplane(airplane);

                    airplane->setStatus(AirplaneEnums::kStatus_EmergencyFinalApproach);
                    airplane->setCommunicationTimer(0);
                    airplane->setActionTimer(0);
                    airplane->setPermission(false);
                } else {
                    airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                    airTrafficController << "$ " << airplane->getCallsign() << " roger mayday, squawk seven seven zero zero, glide until a free runway is available." << std::endl;
                    airplane->increaseCommunicationTimer();
                }
            }
            break;
        case 2:
            Runway* runway = airport->getFreeCompatibleRunway(airplane);

            if (airplane->getAltitude() <= 0) {
                outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " crashed with " << airplane->getPassengers() << " passengers aboard." << std::endl;

                airplane->setStatus(AirplaneEnums::kStatus_Crashed);
                airplane->setCommunicationTimer(0);
                airplane->setActionTimer(0);
                airplane->setPermission(false);
            }

            if (runway != NULL) {
                airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
                airTrafficController << "$ " << airplane->getCallsign() << ", cleared ILS landing runway " << runway->getName() << '.' << std::endl;

                airplane->setStatus(AirplaneEnums::kStatus_FinalApproach);
                airplane->setCommunicationTimer(0);
                airplane->setActionTimer(0);
                airplane->setPermission(false);
            }
            break;
    }
}

void Simulator::EmergencyFinalApproach(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_EmergencyFinalApproach, "Airplane is not in the correct status.");

    //airplane glides by default
    /*airplane->increaseActionTimer();
    if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
        if (airplane->descend(1000)) {
            outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " descended to " << airplane->getAltitude() << " ft. while having a fuel emergency" << std::endl;

            airplane->setActionTimer(0);
        } else {
            errorStream << "Shouldn't happen" << std::endl;
        }
    }*/

    if (airplane->getCommunicationTimer() == 0) {
        airTrafficController << getRealisticTimeStamp() << '[' << airport->getIata() << ']' << std::endl;
        airTrafficController << "$ " << airplane->getCallsign() << ", emergency personel on standby, good luck!" << std::endl;
        airplane->increaseCommunicationTimer();
    }

    if (airplane->getAltitude() == 0) {
        outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " is making an emergency landing on " << airplane->getRunway()->getName() << std::endl;

        airplane->setStatus(AirplaneEnums::kStatus_EmergencyLanding);
        airplane->setCommunicationTimer(0);
        airplane->setActionTimer(0);
        airplane->setPermission(false);
    }
}

void Simulator::EmergencyLand(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_EmergencyLanding, "Airplane is not in the correct status.");

    airplane->increaseActionTimer();
    if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
        outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " has successfully performed an emergency landed on runway " << airplane->getRunway()->getName() << std::endl;

        airplane->setStatus(AirplaneEnums::kStatus_EmergencyEvacuation);
        airplane->setCurrentLocation(airplane->getRunway());
    }
}

void Simulator::EmergencyEvacuate(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_EmergencyEvacuation, "Airplane is not in the correct status.");
    airplane->increaseActionTimer();
    if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
        outputStream << getRealisticTimeStamp() << ' ' << airplane->getPassengers() << " passengers evacuated " << airplane->getCallsign()
                     << " after an emergency landing on runway " << airplane->getRunway()->getName() << std::endl;
        airplane->setPassengers(0);

        airplane->setStatus(AirplaneEnums::kStatus_EmergencyCheckup);
        airplane->setActionTimer(0);
    }
}

void Simulator::EmergencyCheckup(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_EmergencyCheckup, "Airplane is not in the correct status.");
    airplane->increaseActionTimer();
    if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
        outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " has been checked for technical malfunctions on runway " << airplane->getRunway()->getName() << " after an emergency landing" << std::endl;

        airplane->setStatus(AirplaneEnums::kStatus_EmergencyRefueling);
        airplane->setActionTimer(0);
    }
}

void Simulator::EmergencyRefuel(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kStatus_EmergencyRefueling, "Airplane is not in the correct status.");

    airplane->increaseActionTimer();
    if (airplane->getActionTimer() >= getTimeNeededForAction(airplane)) {
        outputStream << getRealisticTimeStamp() << ' ' << airplane->getCallsign() << " has been refueled (" << airplane->getFuelCapacity() - airplane->getFuel()
                     << " units) on runway " << airplane->getRunway()->getName() << " after an emergency landing" << std::endl;
        airplane->setFuel(airplane->getFuelCapacity());

        airplane->setStatus(AirplaneEnums::kStatus_Vacate);
    }
}


unsigned int Simulator::getTimeNeededForAction(const Airplane* airplane) {
    switch (airplane->getStatus()) {
        case AirplaneEnums::kStatus_Ascending:
        case AirplaneEnums::kStatus_DescendingTo3000ft:
        case AirplaneEnums::kStatus_DescendingTo5000ft:
        case AirplaneEnums::kStatus_FinalApproach:
        case AirplaneEnums::kStatus_EmergencyFinalApproach:
            if (airplane->getEngine() == AirplaneEnums::kEngine_Propeller) return 2;
            else if (airplane->getEngine() == AirplaneEnums::kEngine_Jet) return 1;
            break;
        case AirplaneEnums::kStatus_EmergencyLanding:
        case AirplaneEnums::kStatus_Landing:
            return 2;
        case AirplaneEnums::kStatus_TaxiingToApron:
        case AirplaneEnums::kStatus_TaxiingToRunway:
        case AirplaneEnums::kStatus_TaxiingToCrossing:
            return 5;
        case AirplaneEnums::kStatus_LiningUp:
            return 1;
        case AirplaneEnums::kStatus_CrossingRunway:
            return 1;
        case AirplaneEnums::kStatus_EmergencyEvacuation:
        case AirplaneEnums::kStatus_Boarding:
        case AirplaneEnums::kStatus_Unboarding:
            if (airplane->getSize() == AirplaneEnums::kSize_Small) return 5;
            else if (airplane->getSize() == AirplaneEnums::kSize_Medium) return 10;
            else if (airplane->getSize() == AirplaneEnums::kSize_Large) return 15;
            break;
        case AirplaneEnums::kStatus_EmergencyCheckup:
        case AirplaneEnums::kStatus_TechnicalCheckup:
            if (airplane->getSize() == AirplaneEnums::kSize_Small) return 1;
            else if (airplane->getSize() == AirplaneEnums::kSize_Medium) return 2;
            else if (airplane->getSize() == AirplaneEnums::kSize_Large) return 3;
            break;
        case AirplaneEnums::kStatus_EmergencyRefueling:
        case AirplaneEnums::kStatus_Refueling:
            return static_cast<unsigned int>(std::ceil((airplane->getFuelCapacity() - airplane->getFuel()) / 10000.0));
        case AirplaneEnums::kStatus_PushingBack:
            if (airplane->getSize() == AirplaneEnums::kSize_Small) return 1;
            else if (airplane->getSize() == AirplaneEnums::kSize_Medium) return 2;
            else if (airplane->getSize() == AirplaneEnums::kSize_Large) return 3;
            break;
        case AirplaneEnums::kStatus_TakingOff:
            if (airplane->getEngine() == AirplaneEnums::kEngine_Propeller) return 3;
            else if (airplane->getEngine() == AirplaneEnums::kEngine_Jet) return 2;
            break;
        default:
            break;
    }

    return 1;
}