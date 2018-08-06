#ifndef INC_SIMULATOR_H
#define INC_SIMULATOR_H
#include "Airport.h"
#include "Airplane.h"
#include "Runway.h"
#include "AirTrafficController.h"

#include <iomanip>
#include <sstream>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <math.h>

class Simulator {
private:
    unsigned int timer;

    Airport* airport;

    AirTrafficController* airTrafficController;

    std::ostream& outputStream;
    std::ostream& errorStream;

    Simulator* init;

    bool properlyInitialized() const;
public:
    Simulator(const Airport* _airport, std::ostream& _outputStream, std::ostream& _errorStream, std::ostream& _airTrafficControllerStream);

    ~Simulator();

    void Simulate();

    bool SimulationFinished();

    std::string getRealisticTimeStamp() const;

    void Approach(Airplane* airplane);

    void DescendTo5000ft(Airplane* airplane);

    void DescendTo3000ft(Airplane* airplane);

    void FlyWaitingPattern(Airplane* airplane);

    void FinalApproach(Airplane* airplane);

    void Land(Airplane* airplane);

    void UnboardAirplane(Airplane* airplane);

    void TechnicalCheckup(Airplane* airplane);

    void Refuel(Airplane* airplane);

    void BoardAirplane(Airplane* airplane);

    void Ascend(Airplane* airplane);

    void Pushback(Airplane* airplane);

    void LineUp(Airplane* airplane);

    void Taxiing(Airplane* airplane);

    void SendAirplaneToGate(Airplane* airplane);

    void SendAirplaneToRunway(Airplane* airplane);

    unsigned int getTimeNeededForAction(const Airplane* airplane);
};



#endif //INC_SIMULATOR_H
