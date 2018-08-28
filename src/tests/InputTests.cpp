#include "../classes/Airplane.h"
#include "../classes/Airport.h"
#include "../classes/Runway.h"
#include "../classes/Location.h"
#include "../parser/Parser.h"

#include <gtest/gtest.h>

class InputTests: public ::testing::Test {
protected:
    friend class Airport;

    virtual void SetUp() {
        ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    };

    virtual void TearDown() {

    };
};

TEST_F(InputTests, abort) {
    std::string path = "./inputTests/Abort/";
    std::vector<std::string> testNames = std::vector<std::string>();
    testNames.push_back("empty_file/");
    testNames.push_back("empty_tags/");
    testNames.push_back("tag_mismatch/");

    for (unsigned int i = 0; i < testNames.size(); i++) {
        if (DirectoryExists(path + testNames[i])) {
            EXPECT_TRUE(FileExists(path + testNames[i] + "test.xml"));
            EXPECT_TRUE(FileExists(path + testNames[i] + "expected.txt"));
            std::ofstream parserOutputStream;
            std::string paserOutputName = path + testNames[i] + "output.txt";

            parserOutputStream.open(paserOutputName.c_str());

            Parser parser = Parser(parserOutputStream);

            std::string completefilename = path + testNames[i] + "test.xml";
            std::pair<ParseEnum::EResult, std::map<std::string, Airport*> > pairResult = parser.parseFile(completefilename.c_str());

            EXPECT_EQ(pairResult.first, ParseEnum::kAborted);

            EXPECT_TRUE(FileCompare(path + testNames[i] + "expected.txt", path + testNames[i] + "output.txt"));

            parserOutputStream.close();
        } else {
            EXPECT_TRUE(false);
        }
    }
}

TEST_F(InputTests, partial) {
    std::string path = "./inputTests/Partial/";
    std::vector<std::string> testNames = std::vector<std::string>();
    testNames.push_back("Missing/");
    testNames.push_back("Invalid/");
    testNames.push_back("Empty/");

    for (unsigned int i = 0; i < testNames.size(); i++) {
        if (DirectoryExists(path + testNames[i])) {
            EXPECT_TRUE(FileExists(path + testNames[i] + "test.xml"));
            EXPECT_TRUE(FileExists(path + testNames[i] + "expected.txt"));
            std::ofstream parserOutputStream;
            std::string paserOutputName = path + testNames[i] + "output.txt";

            parserOutputStream.open(paserOutputName.c_str());

            Parser parser = Parser(parserOutputStream);

            std::string completefilename = path + testNames[i] + "test.xml";
            std::pair<ParseEnum::EResult, std::map<std::string, Airport*> > pairResult = parser.parseFile(completefilename.c_str());

            EXPECT_EQ(pairResult.first, ParseEnum::kPartial);

            EXPECT_TRUE(FileCompare(path + testNames[i] + "expected.txt", path + testNames[i] + "output.txt"));

            parserOutputStream.close();
        } else {
            EXPECT_TRUE(false);
        }
    }
}

TEST_F(InputTests, success) {
    std::string path = "./inputTests/Success/";

    if (DirectoryExists(path)) {
        EXPECT_TRUE(FileExists(path + "test.xml"));
        EXPECT_TRUE(FileExists(path + "expected.txt"));
        std::ofstream parserOutputStream;
        std::string paserOutputName = path + "output.txt";

        parserOutputStream.open(paserOutputName.c_str());

        Parser parser = Parser(parserOutputStream);

        std::string completefilename = path + "test.xml";
        std::pair<ParseEnum::EResult, std::map<std::string, Airport*> > pairResult = parser.parseFile(completefilename.c_str());

        EXPECT_EQ(pairResult.first, ParseEnum::kSuccess);

        EXPECT_TRUE(FileCompare(path + "expected.txt", path + "output.txt"));

        parserOutputStream.close();
    } else {
        EXPECT_TRUE(false);
    }
}