#include "Simulator.h"

bool Simulator::properlyInitialized() const {
    return this == init;
}

Simulator::Simulator(const Airport* _airport, std::ostream& _outputStream, std::ostream& _errorStream, std::ostream& _airTrafficControllerStream)
    : timer(1),
      airport(new Airport(_airport)),
      airTrafficController(new AirTrafficController(_airTrafficControllerStream)),
      outputStream(_outputStream),
      errorStream(_errorStream),
      init(this) {
    ENSURE(properlyInitialized(), "Simulator was not properly initialized");
}

Simulator::~Simulator() {
    delete airTrafficController;
    delete airport;
}

void Simulator::Simulate() {
    while (!SimulationFinished()) {
        std::priority_queue<Airplane*, std::vector<Airplane*>, CompareSquawk> airplaneQueue;
        for (AirplaneVectorIterator it_airplane = airport->getModifiableAirplanes().begin(); it_airplane != airport->getModifiableAirplanes().end(); it_airplane++) {
            airplaneQueue.push(*it_airplane);
        }

        while (!airplaneQueue.empty()) {
            Airplane* airplane = airplaneQueue.top();
            airplaneQueue.pop();

            switch(airplane->getStatus()) {
                case AirplaneEnums::kApproaching:
                    Approach(airplane);
                    break;
                case AirplaneEnums::kDescendingTo5000ft:
                    DescendTo5000ft(airplane);
                    break;
                case AirplaneEnums::kDescendingTo3000ft:
                    DescendTo3000ft(airplane);
                    break;
                case AirplaneEnums::kFinalApproach:
                    FinalApproach(airplane);
                    break;
                case AirplaneEnums::kLanding:
                    Land(airplane);
                    break;
                case AirplaneEnums::kFlyingWaitPattern:
                    FlyWaitingPattern(airplane);
                    break;
                case AirplaneEnums::kUnboarding:
                    UnboardAirplane(airplane);
                    break;
                case AirplaneEnums::kTechnicalCheckup:
                    TechnicalCheckup(airplane);
                    break;
                case AirplaneEnums::kRefueling:
                    Refuel(airplane);
                    break;
                case AirplaneEnums::kBoarding:
                    BoardAirplane(airplane);
                    break;
                case AirplaneEnums::kAscending:
                    Ascend(airplane);
                    break;
                default:
                    break;
            }
        }

        timer++;
        if (timer / 11520.0 > 1.0) break;
    }
}

bool Simulator::SimulationFinished() {
    for (AirplaneVectorConstIterator it_airplane = airport->getAirplanes().begin(); it_airplane != airport->getAirplanes().end(); it_airplane++) {
        if ((*it_airplane)->getStatus() != AirplaneEnums::kLeftAirport) return false;
    }

    return true;
}

std::string Simulator::getRealisticTimeStamp() const {
    std::stringstream result;

    unsigned int days = static_cast<unsigned int>(std::floor((timer + 720) / 1440.0));
    unsigned int hours = static_cast<unsigned int>(std::floor((timer + 720 - (days * 1440)) / 60.0));
    unsigned int minutes = static_cast<unsigned int>(std::floor((timer + 720) - (days * 1440) - (hours * 60)));

    if (days % 7 == 0) result << "Monday ";
    else if (days % 7 == 1) result << "Tuesday ";
    else if (days % 7 == 2) result << "Wednesday ";
    else if (days % 7 == 3) result << "Thursday ";
    else if (days % 7 == 4) result << "Friday ";
    else if (days % 7 == 5) result << "Saturday ";
    else if (days % 7 == 6) result << "Sunday ";

    result << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2) << std::setfill('0') << minutes;

    return result.str();
}

void Simulator::Approach(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kApproaching, "Airplane is not in the correct status.");

    airplane->fly();
    if (airport->is5000ftVacant()) {
        airplane->setStatus(AirplaneEnums::kDescendingTo5000ft);
        airport->set5000ft(airplane);

        outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getCallsign() << " is flying in a waiting pattern at 5000 ft" << std::endl;

        airplane->setInternalTimer(0);
    }
}

void Simulator::DescendTo5000ft(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kDescendingTo5000ft, "Airplane is not in the correct status.");
    REQUIRE(airplane == airport->get5000ft(), "Airplane is not the one descending to 5000 ft");

    airplane->fly();
    airplane->increaseInternalTimer();
    if (airplane->getInternalTimer() >= getTimeNeededForAction(airplane)) {
        airplane->setAltitude(airplane->getAltitude() - 1000);

        outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getCallsign() << " descended to " << airplane->getAltitude() << " ft." << std::endl;

        airplane->setInternalTimer(0);
    }

    if (airplane->getAltitude() <= 5000) {
        airplane->setStatus(AirplaneEnums::kFlyingWaitPattern);
        outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getCallsign() << " is flying in a waiting pattern at 5000 ft" << std::endl;

        airplane->setInternalTimer(0);
    }
}

void Simulator::DescendTo3000ft(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kDescendingTo3000ft, "Airplane is not in the correct status.");
    REQUIRE(airplane == airport->get3000ft(), "Airplane is not the one descending to 5000 ft");

    airplane->fly();
    airplane->increaseInternalTimer();
    if (airplane->getInternalTimer() >= getTimeNeededForAction(airplane)) {
        airplane->setAltitude(airplane->getAltitude() - 1000);

        outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getCallsign() << " descended to " << airplane->getAltitude() << " ft." << std::endl;

        airplane->setInternalTimer(0);
    }

    if (airplane->getAltitude() <= 3000) {
        airplane->setStatus(AirplaneEnums::kFlyingWaitPattern);
        outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getCallsign() << " is flying in a waiting pattern at 3000 ft" << std::endl;

        airplane->setInternalTimer(0);
    }
}

void Simulator::FlyWaitingPattern(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kFlyingWaitPattern, "Airplane is not in the correct status.");

    airplane->fly();
    if (airplane->getAltitude() == 5000) {
        if (airport->is3000ftVacant()) {
            airport->set3000ft(airplane);
            airplane->setStatus(AirplaneEnums::kDescendingTo3000ft);

            outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getCallsign() << " has begun its descent to 3000 ft" << std::endl;
        }
    }

    if (airplane->getAltitude() == 3000) {
        Runway* runway = airport->getFreeCompatibleRunway(airplane);

        if (runway != NULL) {
            airport->set3000ft(NULL);
            airplane->setRunway(runway);
            runway->setAirplane(airplane);

            airplane->setStatus(AirplaneEnums::kFinalApproach);

            outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getCallsign() << " has begun its final approach to " << runway->getName() << " at " << airport->getAirportName() << std::endl;
        }
    }
}

void Simulator::FinalApproach(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kFinalApproach, "Airplane is not in the correct status.");

    airplane->fly();
    airplane->increaseInternalTimer();
    if (airplane->getInternalTimer() >= getTimeNeededForAction(airplane)) {
        airplane->setAltitude(airplane->getAltitude() - 1000);
        airplane->setInternalTimer(0);

        outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getCallsign() << " has descended to " << airplane->getAltitude() << " ft" << std::endl;
    }

    if (airplane->getAltitude() == 0)
    {
        airplane->setStatus(AirplaneEnums::kLanding);
        airplane->setInternalTimer(0);
    }
}

void Simulator::Land(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kLanding, "Airplane is not in the correct status.");

    airplane->increaseInternalTimer();
    if (airplane->getInternalTimer() >= getTimeNeededForAction(airplane)) {

        airplane->setStatus(AirplaneEnums::kUnboarding);
        airplane->setInternalTimer(0);
    }
}

void Simulator::UnboardAirplane(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kUnboarding, "Airplane is not in the correct status.");
    airplane->increaseInternalTimer();
    if (airplane->getInternalTimer() >= getTimeNeededForAction(airplane)) {
        outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getPassengers() << " passengers exited " << airplane->getCallsign()
                    << " at gate " << airplane->getGate() << " of " << airport->getAirportName() << std::endl;
        airplane->setPassengers(0);

        airplane->setStatus(AirplaneEnums::kTechnicalCheckup);
        airplane->setInternalTimer(0);
    }
}

void Simulator::TechnicalCheckup(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kTechnicalCheckup, "Airplane is not in the correct status.");
    airplane->increaseInternalTimer();
    if (airplane->getInternalTimer() >= getTimeNeededForAction(airplane)) {
        outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getCallsign() << " has been checked for technical malfunctions" << std::endl;

        airplane->setStatus(AirplaneEnums::kRefueling);
        airplane->setInternalTimer(0);
    }
}

void Simulator::Refuel(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kRefueling, "Airplane is not in the correct status.");

    airplane->increaseInternalTimer();
    if (airplane->getInternalTimer() >= getTimeNeededForAction(airplane)) {
        outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getCallsign() << " has been refueled (" << airplane->getFuelCapacity() - airplane->getFuel() << " units)" << std::endl;
        airplane->setFuel(airplane->getFuelCapacity());

        airplane->setStatus(AirplaneEnums::kBoarding);
        airplane->setInternalTimer(0);
    }
}

void Simulator::BoardAirplane(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kBoarding, "Airplane is not in the correct status.");

    airplane->increaseInternalTimer();
    if (airplane->getInternalTimer() >= getTimeNeededForAction(airplane)) {
        airplane->setPassengers(airplane->getPassengerCapacity());
        outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getPassengers() << " passengers boarded " << airplane->getCallsign() << " at gate "
                    << airplane->getGate() << " of " << airport->getAirportName() << std::endl;

        airplane->setStatus(AirplaneEnums::kStandingAtGate);
        airplane->setInternalTimer(0);
    }
}

/*void Simulator::StandAtGate(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kBoarding, "Airplane is not in the correct status.");


}*/

void Simulator::Pushback(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kPushingBack, "Airplane is not in the correct status.");
    airplane->increaseInternalTimer();
    if (airplane->getInternalTimer() >= getTimeNeededForAction(airplane)) {
        airplane->setPassengers(airplane->getPassengerCapacity());
        outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getCallsign() << " pushed back from gate "  << airplane->getGate() << " at " << airport->getAirportName() << std::endl;

        airplane->setStatus(AirplaneEnums::kTaxiingToRunway);
        airplane->setInternalTimer(0);
    }
}

void Simulator::Ascend(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kAscending, "Airplane is not in the correct status.");

    airplane->fly();
    airplane->increaseInternalTimer();
    if (airplane->getInternalTimer() >= getTimeNeededForAction(airplane)) {
        airplane->setAltitude(airplane->getAltitude() + 1000);

        outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getCallsign() << " ascended to " << airplane->getAltitude() << " ft." << std::endl;

        airplane->setInternalTimer(0);
    }

    if (airplane->getAltitude() >= 5000) {
        airplane->setStatus(AirplaneEnums::kLeftAirport);
        outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getCallsign() << " has left " << airport->getAirportName();

        airplane->setInternalTimer(0);
    }
}

void Simulator::LineUp(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kLiningUp, "Airplane is not in the correct status.");
    airplane->increaseInternalTimer();
    if (airplane->getInternalTimer() >= getTimeNeededForAction(airplane)) {
        outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getCallsign() << " has lined up on runway " << airplane->getRunway()->getName() << " and is ready for take off" << std::endl;

        airplane->setStatus(AirplaneEnums::kReadyForTakeoff);
        airplane->setInternalTimer(0);
    }
}

/*void Simulator::TaxiingToGate(Airplane* airplane) {
    REQUIRE(properlyInitialized(), "Simulator was not properly initialized.");
    REQUIRE(airplane->getStatus() == AirplaneEnums::kTaxiingToGate, "Airplane is not in the correct status.");

    airplane->increaseInternalTimer();
    if (airplane->getInternalTimer() >= getTimeNeededForAction(airplane)) {
        Location* nextStep = airplane->getCurrentLocation()->getPreviousLocation();

        if (nextStep == NULL) {
            long gate = airport->getEmptyGate();

            outputStream << "[" << getRealisticTimeStamp() << "] " << airplane->getCallsign() << " has arrived at the gate " << std::endl;
        } else {
            if (nextStep != NULL) {
                airplane->
            }
        }



        airplane->setWaitingForInstructions(true);
        airplane->setInternalTimer(0);
    }
}*/



void Simulator::SendAirplaneToGate(Airplane* airplane) {

}

void Simulator::SendAirplaneToRunway(Airplane* airplane) {

}

unsigned int Simulator::getTimeNeededForAction(const Airplane* airplane) {
    if (airplane->getWaitingForInstructions()) return 1;

    switch (airplane->getStatus()) {
        case AirplaneEnums::kAscending:
        case AirplaneEnums::kDescendingTo3000ft:
        case AirplaneEnums::kDescendingTo5000ft:
        case AirplaneEnums::kFinalApproach:
            if (airplane->getEngine() == AirplaneEnums::kPropeller) return 2;
            else if (airplane->getEngine() == AirplaneEnums::kJet) return 1;
            break;
        case AirplaneEnums::kLanding:
            return 2;
        case AirplaneEnums::kTakingOff:
            if (airplane->getEngine() == AirplaneEnums::kPropeller) return 3;
            else if (airplane->getEngine() == AirplaneEnums::kJet) return 2;
            break;
        case AirplaneEnums::kPushingBack:
            if (airplane->getSize() == AirplaneEnums::kSmall) return 1;
            else if (airplane->getSize() == AirplaneEnums::kMedium) return 2;
            else if (airplane->getSize() == AirplaneEnums::kLarge) return 3;
            break;
        case AirplaneEnums::kTaxiingToGate:
        case AirplaneEnums::kTaxiingToRunway:
            return 5;
        case AirplaneEnums::kLiningUp:
            return 1;
        case AirplaneEnums::kCrossingRunway:
            return 1;
        case AirplaneEnums::kBoarding:
            if (airplane->getSize() == AirplaneEnums::kSmall) return 5;
            else if (airplane->getSize() == AirplaneEnums::kMedium) return 10;
            else if (airplane->getSize() == AirplaneEnums::kLarge) return 15;
            break;
        case AirplaneEnums::kUnboarding:
            if (airplane->getSize() == AirplaneEnums::kSmall) return 5;
            else if (airplane->getSize() == AirplaneEnums::kMedium) return 10;
            else if (airplane->getSize() == AirplaneEnums::kLarge) return 15;
            break;
        case AirplaneEnums::kRefueling:
            return static_cast<unsigned int>(std::ceil(static_cast<double>(airplane->getFuelCapacity() - airplane->getFuel()) / 10000.0));
        case AirplaneEnums::kTechnicalCheckup:
            if (airplane->getSize() == AirplaneEnums::kSmall) return 1;
            else if (airplane->getSize() == AirplaneEnums::kMedium) return 2;
            else if (airplane->getSize() == AirplaneEnums::kLarge) return 3;
            break;
        default:
            break;
    }
    return 1;
}