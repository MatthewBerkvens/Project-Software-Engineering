#include "../classes/Airplane.h"
#include "../classes/Airport.h"
#include "../classes/Runway.h"
#include "../classes/Location.h"

#include <gtest/gtest.h>

class RunwayDomainTests: public ::testing::Test {
protected:
    friend class Runway;

    virtual void SetUp() {
        ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    };

    virtual void TearDown() {

    };
};

TEST_F(RunwayDomainTests, initializerAndGetters) {
    std::string runwayName = "R11";
    unsigned int runwayLength = 1000;
    RunwayEnums::EType runwayType = RunwayEnums::kGrass;

    Runway runway = Runway(runwayName, runwayLength, runwayType);

    EXPECT_EQ(runway.getName(), runwayName);
    EXPECT_EQ(runway.getLength(), runwayLength);
    EXPECT_EQ(runway.getType(), runwayType);

    EXPECT_EQ(runway.getAirport(), static_cast<Airport*>(NULL));
    EXPECT_EQ(runway.getAirplane(), static_cast<Airplane*>(NULL));
    EXPECT_EQ(runway.getCrossingAirplane(), static_cast<Airplane*>(NULL));
    EXPECT_EQ(runway.getPreviousLocation(), static_cast<Location*>(NULL));
    EXPECT_EQ(runway.getNextLocation(), static_cast<Location*>(NULL));
}

TEST_F(RunwayDomainTests, setters) {
    std::string runwayName = "R11";
    unsigned int runwayLength = 1000;
    RunwayEnums::EType runwayType = RunwayEnums::kGrass;

    Runway runway = Runway(runwayName, runwayLength, runwayType);

    Airport airport = Airport("Dummy Airport", "DUM", "Dummy", 3);

    EXPECT_EQ(runway.getAirport(), static_cast<Airport*>(NULL));
    runway.setAirport(&airport);
    EXPECT_EQ(runway.getAirport(), &airport);

    Airplane airplane = Airplane("N123", "Dummy Airplane", "Dummy", 01, 10000, 10000, 10000, 4, 4, AirplaneEnums::kStatus_Approaching, AirplaneEnums::kType_Private, AirplaneEnums::kSize_Small, AirplaneEnums::kEngine_Propeller);

    EXPECT_EQ(runway.getAirplane(), static_cast<Airplane*>(NULL));
    runway.setAirplane(&airplane);
    EXPECT_EQ(runway.getAirplane(), &airplane);

    Airplane crossingAirplane = Airplane("N123", "Dummy Crossing Airplane", "Dummy", 01, 10000, 10000, 10000, 4, 4, AirplaneEnums::kStatus_Approaching, AirplaneEnums::kType_Private, AirplaneEnums::kSize_Small, AirplaneEnums::kEngine_Propeller);

    EXPECT_EQ(runway.getCrossingAirplane(), static_cast<Airplane*>(NULL));
    runway.setCrossingAirplane(&crossingAirplane);
    EXPECT_EQ(runway.getCrossingAirplane(), &crossingAirplane);

    Location previousLocation = Location("Dummy Previous Location");

    EXPECT_EQ(runway.getPreviousLocation(), static_cast<Location*>(NULL));
    runway.setPreviousLocation(&previousLocation);
    EXPECT_EQ(runway.getPreviousLocation(), &previousLocation);

    Location nextLocation = Location("Dummy Next Location");

    EXPECT_EQ(runway.getNextLocation(), static_cast<Location*>(NULL));
    runway.setNextLocation(&nextLocation);
    EXPECT_EQ(runway.getNextLocation(), &nextLocation);
}