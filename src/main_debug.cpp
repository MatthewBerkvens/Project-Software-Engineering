#include <iostream>
#include "parser/parser.h"
#include "classes/Airport.h"
#include "classes/Runway.h"
#include "classes/Airplane.h"
#include "utils/utils.h"

int main() {
    std::cout << "Hello, Debug!" << std::endl;

    Airport airport = Airport("airport", "iata", "callsign", 5);
    Runway runway = Runway("runway", &airport);
    airport.addRunway(&runway);

    Airplane airplane = Airplane("nr", "callsign", "model", AirplaneEnums::kApproaching);


    //const Airport* runwayAirport = runway.getAirport();
    //REQUIRE(runwayAirport == &airport, "Runway's linked airport does not match");

    airplane.setAirport(&airport);

    //REQUIRE(airplane.getAirport() == runwayAirport, "Airplane's linked airport does not match");

    airplane.setRunway(&runway);

    airport.printInfo(std::cout);
    runway.printInfo(std::cout);
    airplane.printInfo(std::cout);
    return 0;
}