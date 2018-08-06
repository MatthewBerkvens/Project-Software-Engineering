#include "consistency_checker.h"


bool isAirportStartConsistent(Airport* airport)
{
    std::vector<std::string> airplaneNames;

    const AirplaneVector& airplaneVector = airport->getAirplanes();
    for (AirplaneVectorConstIterator it_airplane = airplaneVector.begin(); it_airplane != airplaneVector.end(); it_airplane++) {
        ENSURE((*it_airplane)->getAirport() == airport, "Airplane: Airport property is not consistent.");
        ENSURE((*it_airplane)->getRunway() == NULL, "Airplane: Runway property is not consistent.");
        ENSURE((*it_airplane)->getStatus() == AirplaneEnums::kApproaching || (*it_airplane)->getStatus() == AirplaneEnums::kStandingAtGate, "Airplane: status property is not consistent.");

        ENSURE(!vectorContains(airplaneNames, (*it_airplane)->getNumber()), "Airplane: duplicate register number.");
        airplaneNames.push_back((*it_airplane)->getNumber());
    }

    const RunwayVector& runwayVector = airport->getRunways();
    for (RunwayVectorConstIterator it_runway = runwayVector.begin(); it_runway != runwayVector.end(); it_runway++) {
        ENSURE((*it_runway)->getAirport() == airport, "Runway: Airport property is not consistent.");
        ENSURE((*it_runway)->getLength() != 0, "Runway: Length property is not consistent.");
    }

    return true;
}