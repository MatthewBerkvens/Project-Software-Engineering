#include "../classes/Airplane.h"
#include "../classes/Airport.h"
#include "../classes/Runway.h"
#include "../classes/Location.h"

#include <gtest/gtest.h>

class LocationDomainTests: public ::testing::Test {
protected:
    friend class Location;

    virtual void SetUp() {
        ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    };

    virtual void TearDown() {

    };
};

TEST_F(LocationDomainTests, initializerAndGetters) {
    std::string locationName = "Alpha";

    Location location = Location(locationName);

    EXPECT_EQ(location.getName(), locationName);

    EXPECT_EQ(location.getAirport(), static_cast<Airport*>(NULL));
    EXPECT_EQ(location.getPreviousLocation(), static_cast<Location*>(NULL));
    EXPECT_EQ(location.getNextLocation(), static_cast<Location*>(NULL));
}

TEST_F(LocationDomainTests, setters) {
    std::string locationName = "Alpha";

    Location location = Location(locationName);

    Airport airport = Airport("Dummy Airport", "DUM", "Dummy", 3);

    EXPECT_EQ(location.getAirport(), static_cast<Airport*>(NULL));
    location.setAirport(&airport);
    EXPECT_EQ(location.getAirport(), &airport);

    Location previousLocation = Location("Dummy Previous Location");

    EXPECT_EQ(location.getPreviousLocation(), static_cast<Location*>(NULL));
    location.setPreviousLocation(&previousLocation);
    EXPECT_EQ(location.getPreviousLocation(), &previousLocation);

    Location nextLocation = Location("Dummy Next Location");

    EXPECT_EQ(location.getNextLocation(), static_cast<Location*>(NULL));
    location.setNextLocation(&nextLocation);
    EXPECT_EQ(location.getNextLocation(), &nextLocation);
}