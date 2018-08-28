#include "../classes/Airplane.h"
#include "../classes/Airport.h"
#include "../classes/Runway.h"
#include "../classes/Location.h"
#include "../classes/Simulator.h"
#include "../parser/Parser.h"

#include <gtest/gtest.h>

class OutputTests: public ::testing::Test {
protected:
    friend class Airport;

    virtual void SetUp() {
        ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    };

    virtual void TearDown() {

    };
};

TEST_F(OutputTests, outputs) {
    std::string path = "./outputTests/";
    std::vector<std::string> testNames = std::vector<std::string>();
    testNames.push_back("1/");
    testNames.push_back("2/");

    for (unsigned int i = 0; i < testNames.size(); i++) {
        if (DirectoryExists(path + testNames[i])) {
            EXPECT_TRUE(FileExists(path + testNames[i] + "input.xml"));
            EXPECT_TRUE(FileExists(path + testNames[i] + "expected_parser.txt"));

            std::ofstream parserOutputStream;
            std::string paserOutputName = path + testNames[i] + "output_parser.txt";
            parserOutputStream.open(paserOutputName.c_str());

            Parser parser = Parser(parserOutputStream);

            std::string completefilename = path + testNames[i] + "input.xml";
            std::pair<ParseEnum::EResult, std::map<std::string, Airport*> > pairResult = parser.parseFile(completefilename.c_str());

            EXPECT_TRUE(FileCompare(path + testNames[i] + "expected_parser.txt", path + testNames[i] + "output_parser.txt"));

            parserOutputStream.close();

            EXPECT_EQ(pairResult.first, ParseEnum::kSuccess);
            for (AirportMap::iterator it_airport = pairResult.second.begin(); it_airport != pairResult.second.end(); it_airport++) {
                Airport* airport = it_airport->second;

                std::string airportPath = path + testNames[i] + airport->getIata() + "/";

                EXPECT_TRUE(FileExists(airportPath + "expected_output.txt"));
                EXPECT_TRUE(FileExists(airportPath + "expected_error.txt"));
                EXPECT_TRUE(FileExists(airportPath + "expected_tower.txt"));

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

                for (AirplaneMap::const_iterator it_airplane = airport->getAirplanes().begin(); it_airplane != airport->getAirplanes().end(); it_airplane++) {
                    std::ofstream airplaneStream;
                    std::string airplaneMap = airportPath + "airplanes/" + it_airplane->second->getNumber() + ".txt";
                    airplaneStream.open(airplaneMap.c_str());

                    it_airplane->second->printInfo(airplaneStream);

                    EXPECT_TRUE(FileCompare(airportPath + "airplanes/expected_" + it_airplane->second->getNumber() + ".txt", airplaneMap));

                    airplaneStream.close();
                }

                Simulator simulator(airport, outputStream, errorStream, towerStream, floorplanStream);
                simulator.Simulate();

                EXPECT_TRUE(FileCompare(airportPath + "expected_output.txt", airportPath + "output_output.txt"));
                EXPECT_TRUE(FileCompare(airportPath + "expected_error.txt", airportPath + "output_error.txt"));
                EXPECT_TRUE(FileCompare(airportPath + "expected_tower.txt", airportPath + "output_tower.txt"));
                EXPECT_TRUE(FileCompare(airportPath + "expected_floorplan.txt", airportPath + "output_floorplan.txt"));
                EXPECT_TRUE(FileCompare(airportPath + "expected_airportinfo.txt", airportPath + "output_airportinfo.txt"));

                outputStream.close();
                errorStream.close();
                towerStream.close();
                floorplanStream.close();
                airportInfoStream.close();

                delete airport;
            }
        } else {
            EXPECT_TRUE(false);
        }
    }
}