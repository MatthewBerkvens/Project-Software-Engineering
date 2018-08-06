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

    std::pair<ParseEnum::EResult, Airport*> pairResult = parser.parseFile("simpleScenario.xml");

    ParseEnum::EResult parserResult = pairResult.first;
    if (parserResult == ParseEnum::kSuccess) {
        Airport* airport = pairResult.second;

        /*for (unsigned int i = 0; i < airport->getAirplanes().size(); ++i) {
            airport->getAirplanes()[i]->printInfo(std::cout);
        }

        for (unsigned int i = 0; i < airport->getRunways().size(); ++i) {
            airport->getRunways()[i]->printInfo(std::cout);
        }

        airport->printInfo(std::cout);*/
        airport->getModifiableGates()[2] = airport->getAirplanes()[0];

        Simulator simulator(airport, std::cout, std::cout, std::cout);
        delete airport;
        simulator.Simulate();

        //delete airport;
    }

    return 0;
}