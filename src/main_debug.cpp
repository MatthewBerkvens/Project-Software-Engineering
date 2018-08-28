#include <iostream>
#include "parser/Parser.h"
#include "classes/Simulator.h"
#include "classes/Airport.h"
#include "classes/Runway.h"
#include "classes/Airplane.h"
#include "utils/Utils.h"

#include <iostream>
#include <sstream>

int main() {
    std::cout << "Hello, Debug!" << std::endl;

    Parser parser(std::cout);

    std::pair<ParseEnum::EResult, std::map<std::string, Airport*> > pairResult = parser.parseFile("./outputTests/1/input.xml");
    std::map<std::string, Airport*> allAirports = pairResult.second;
    
    for (int i = 0; i < 1; ++i) {
        ParseEnum::EResult parserResult = pairResult.first;
        if (parserResult == ParseEnum::kSuccess) {
            for (AirportMap::iterator it_airport = allAirports.begin(); it_airport != allAirports.end(); it_airport++) {
                Airport* airport = it_airport->second;

                /*for (unsigned int i = 0; i < airport->getAirplanes().size(); ++i) {
                    airport->getAirplanes()[i]->printInfo(std::cout);
                }

                for (unsigned int i = 0; i < airport->getRunways().size(); ++i) {
                    airport->getRunways()[i]->printInfo(std::cout);
                }

                airport->printInfo(std::cout);*/

                std::ofstream outputStream;
                std::ofstream errorStream;
                std::ofstream atcStream;
                std::ofstream floorplanStream;

                std::string outputName = "output_" + airport->getIata() + ".txt";
                std::string errorName = "error_" + airport->getIata() + ".txt";
                std::string atcName = "atc_" + airport->getIata() + ".txt";
                std::string floorplanName = "floorplan_" + airport->getIata() + ".txt";

                outputStream.open(outputName.c_str());
                errorStream.open(errorName.c_str());
                atcStream.open(atcName.c_str());
                floorplanStream.open(floorplanName.c_str());

                Simulator simulator(airport, outputStream, errorStream, atcStream, floorplanStream);
                simulator.Simulate();

                outputStream.close();
                errorStream.close();
                atcStream.close();
                floorplanStream.close();
            }
        }
    }

    for (AirportMap::iterator it_airport = allAirports.begin(); it_airport != allAirports.end(); it_airport++) {
        delete it_airport->second;
    }

    return 0;
}