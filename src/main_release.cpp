#include <iostream>

#include "classes/Simulator.h"
#include "parser/Parser.h"
#include "utils/Utils.h"

int main(int argc, char **argv) {
    std::cout << "Hello, Release!" << std::endl;
    if (argc == 2) {
        std::cout << "Running scenario " << argv[1] << std::endl;
        std::string scenarioName = argv[1];
        std::string path = "scenarios/" + scenarioName + "/";
        if (DirectoryExists(path)) {
            if (!FileExists(path + "input.xml")) return 0;

            std::ofstream parserOutputStream;
            std::string paserOutputName = path + "output_parser.txt";
            parserOutputStream.open(paserOutputName.c_str());

            Parser parser = Parser(parserOutputStream);

            std::string completefilename = path + "input.xml";
            std::pair<ParseEnum::EResult, std::map<std::string, Airport *> > pairResult = parser.parseFile(completefilename.c_str());

            parserOutputStream.close();

            if (pairResult.first != ParseEnum::kSuccess) return 0;
            for (AirportMap::iterator it_airport = pairResult.second.begin(); it_airport != pairResult.second.end(); it_airport++)
            {
                Airport *airport = it_airport->second;

                std::string airportPath = path + airport->getIata() + "/";

                std::ofstream outputStream;
                std::string outputStreamName = airportPath + "output_output.txt";
                outputStream.open(outputStreamName.c_str());

                std::ofstream errorStream;
                std::string errorStreamName = airportPath + "output_error.txt";
                errorStream.open(errorStreamName.c_str());

                std::ofstream towerStream;
                std::string towerStreamName = airportPath + "output_tower.txt";
                towerStream.open(towerStreamName.c_str());

                std::ofstream floorplanStream;
                std::string floorplanStreamName = airportPath + "output_floorplan.txt";
                floorplanStream.open(floorplanStreamName.c_str());

                std::ofstream airportInfoStream;
                std::string fairportInfoStreamName = airportPath + "output_airportinfo.txt";
                airportInfoStream.open(fairportInfoStreamName.c_str());

                airport->printInfo(airportInfoStream);

                for (AirplaneMap::const_iterator it_airplane = airport->getAirplanes().begin(); it_airplane != airport->getAirplanes().end(); it_airplane++)
                {
                    std::ofstream airplaneStream;
                    std::string airplaneMap = airportPath + "airplanes/" + it_airplane->second->getNumber() + ".txt";
                    airplaneStream.open(airplaneMap.c_str());

                    it_airplane->second->printInfo(airplaneStream);

                    airplaneStream.close();
                }

                Simulator simulator(airport, outputStream, errorStream, towerStream, floorplanStream);
                simulator.Simulate();

                outputStream.close();
                errorStream.close();
                towerStream.close();
                floorplanStream.close();
                airportInfoStream.close();

                delete airport;
            }
        }
    }
    return 0;
}
