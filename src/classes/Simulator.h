#ifndef INC_SIMULATOR_H
#define INC_SIMULATOR_H
#include "Airport.h"
#include "Airplane.h"
#include "Runway.h"

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

    std::ostream& airTrafficController;

    std::ostream& floorplanStream;
    std::ostream& outputStream;
    std::ostream& errorStream;

    Simulator* init;

    bool properlyInitialized() const;
public:
    Simulator(const Airport* _airport, std::ostream& _outputStream, std::ostream& _errorStream, std::ostream& _airTrafficControllerStream, std::ostream& _floorplanStream);

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

    void Vacate(Airplane* airplane);

    void TaxiToCrossing(Airplane* airplane);

    void TaxiToRunway(Airplane* airplane);

    void TaxiToApron(Airplane* airplane);

    void WaitingAtCrossing(Airplane* airplane);

    void CrossRunway(Airplane* airplane);

    void UnboardAirplane(Airplane* airplane);

    void TechnicalCheckup(Airplane* airplane);

    void Refuel(Airplane* airplane);

    void BoardAirplane(Airplane* airplane);

    void StandAtGate(Airplane* airplane);

    void Pushback(Airplane* airplane);

    void HoldShort(Airplane* airplane);

    void LineUp(Airplane* airplane);

    void ReadyForTakeoff(Airplane* airplane);

    void Takeoff(Airplane* airplane);

    void Ascend(Airplane* airplane);

    void Emergency(Airplane* airplane);

    void EmergencyFinalApproach(Airplane* airplane);

    void EmergencyLand(Airplane* airplane);

    void EmergencyEvacuate(Airplane* airplane);

    void EmergencyCheckup(Airplane* airplane);

    void EmergencyRefuel(Airplane* airplane);

    unsigned int getTimeNeededForAction(const Airplane* airplane);
};



#endif //INC_SIMULATOR_H
