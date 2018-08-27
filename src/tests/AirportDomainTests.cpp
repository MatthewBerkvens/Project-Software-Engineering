#include "../classes/Airplane.h"
#include "../classes/Airport.h"
#include "../classes/Runway.h"
#include "../classes/Location.h"

#include <gtest/gtest.h>

class AirportDomainTests: public ::testing::Test {
protected:
    friend class Airport;

    virtual void SetUp() {
        ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    };

    virtual void TearDown() {

    };
};

TEST_F(AirportDomainTests, initializerAndGetters) {
    std::string airportName = "Antwerp International Airport";
    std::string airportIATA = "ANR";
    std::string airportCallsign = "Antwerp Tower";

    unsigned int gates = 10;

    Airport airport = Airport(airportName, airportIATA, airportCallsign, gates);

    EXPECT_EQ(airport.getAirportName(), airportName);
    EXPECT_EQ(airport.getIata(), airportIATA);
    EXPECT_EQ(airport.getCallsign(), airportCallsign);

    EXPECT_EQ(airport.getGateCount(), static_cast<unsigned int>(10));

    for (unsigned int i = 0; i < gates; i++) {
        EXPECT_EQ(airport.getGates()[i], static_cast<Airplane*>(NULL));
    }

    EXPECT_EQ(airport.get3000ft(), static_cast<Airplane*>(NULL));
    EXPECT_TRUE(airport.is3000ftVacant());
    EXPECT_EQ(airport.get5000ft(), static_cast<Airplane*>(NULL));
    EXPECT_TRUE(airport.is5000ftVacant());

    EXPECT_TRUE(airport.getLocations().empty());
    EXPECT_TRUE(airport.getRunways().empty());
    EXPECT_TRUE(airport.getAirplanes().empty());

    airport.
}

TEST_F(AirportDomainTests, searchPlaneByName) {
    std::string airportName = "Antwerp International Airport";
    std::string airportIATA = "ANR";
    std::string airportCallsign = "Antwerp Tower";

    unsigned int gates = 10;

    Airport airport = Airport(airportName, airportIATA, airportCallsign, gates);

    airport.getAirplanesByRegistrationNumber();
}

TEST_F(AirportDomainTests, searchRunwayByName) {
    std::string airportName = "Antwerp International Airport";
    std::string airportIATA = "ANR";
    std::string airportCallsign = "Antwerp Tower";

    unsigned int gates = 10;

    Airport airport = Airport(airportName, airportIATA, airportCallsign, gates);

    airport.getRunwayByName();
}

TEST_F(AirportDomainTests, searchLocationByName) {
    std::string airportName = "Antwerp International Airport";
    std::string airportIATA = "ANR";
    std::string airportCallsign = "Antwerp Tower";

    unsigned int gates = 10;

    Airport airport = Airport(airportName, airportIATA, airportCallsign, gates);

    airport.getLocationByName();
}

TEST_F(AirportDomainTests, taxiRoute) {
    std::string airportName = "Antwerp International Airport";
    std::string airportIATA = "ANR";
    std::string airportCallsign = "Antwerp Tower";

    unsigned int gates = 10;

    Airport airport = Airport(airportName, airportIATA, airportCallsign, gates);

    airport.createTaxiRouteToGates();
}

TEST_F(AirportDomainTests, gates) {
    std::string airportName = "Antwerp International Airport";
    std::string airportIATA = "ANR";
    std::string airportCallsign = "Antwerp Tower";

    unsigned int gates = 10;

    Airport airport = Airport(airportName, airportIATA, airportCallsign, gates);

    airport.createTaxiRouteToGates();
}


TEST_F(AirportDomainTests, compatibleRunways) {
    std::string airportName = "Antwerp International Airport";
    std::string airportIATA = "ANR";
    std::string airportCallsign = "Antwerp Tower";

    unsigned int gates = 10;

    Airport airport = Airport(airportName, airportIATA, airportCallsign, gates);

    airport.getFreeCompatibleRunway();
}


