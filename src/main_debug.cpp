#include <iostream>
#include "parser/parser.h"
#include "classes/Simulator.h"
#include "classes/Airport.h"
#include "classes/Runway.h"
#include "classes/Airplane.h"
#include "utils/utils.h"

#include <iostream>
#include <sstream>

int main() {
    std::cout << "Hello, Debug!" << std::endl;

    Parser parser(std::cout);

    std::pair<ParseEnum::EResult, std::map<std::string, Airport*> > pairResult = parser.parseFile("simpleScenario.xml");
    std::map<std::string, Airport*> allAirports = pairResult.second;

    for (int i = 0; i < 1; ++i) {
        ParseEnum::EResult parserResult = pairResult.first;
        if (parserResult == ParseEnum::kSuccess) {
            for (AirportMap::iterator it_airport = allAirports.begin(); it_airport != allAirports.end(); it_airport++) {
                Airport* airport = (*it_airport).second;

                /*for (unsigned int i = 0; i < airport->getAirplanes().size(); ++i) {
                    airport->getAirplanes()[i]->printInfo(std::cout);
                }

                for (unsigned int i = 0; i < airport->getRunways().size(); ++i) {
                    airport->getRunways()[i]->printInfo(std::cout);
                }

                airport->printInfo(std::cout);*/

                Simulator simulator(airport, std::cout, std::cout, std::cout);
                simulator.Simulate();
            }
        }
    }

    for (AirportMap::iterator it_airport = allAirports.begin(); it_airport != allAirports.end(); it_airport++) {
        delete (*it_airport).second;
    }

    return 0;
}