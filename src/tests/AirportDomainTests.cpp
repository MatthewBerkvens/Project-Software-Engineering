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

    EXPECT_EQ(airport.getConnectionToApron(), static_cast<Location*>(NULL));
}

TEST_F(AirportDomainTests, setters) {
    std::string airportName = "Antwerp International Airport";
    std::string airportIATA = "ANR";
    std::string airportCallsign = "Antwerp Tower";

    unsigned int gates = 10;

    Airport airport = Airport(airportName, airportIATA, airportCallsign, gates);


    EXPECT_DEATH(airport.addRunway(NULL), "Referenced Runway is a nullpointer.");
    EXPECT_DEATH(airport.addLocation(NULL), "Referenced Location is a nullpointer.");
    EXPECT_DEATH(airport.addAirplane(NULL), "Referenced Airplane is a nullpointer.");

    Airplane* airplane3000ft = new Airplane("N123", "Dummy Airplane", "Dummy", 01, 10000, 10000, 10000, 4, 4, AirplaneEnums::kStatus_Approaching, AirplaneEnums::kType_Private, AirplaneEnums::kSize_Small, AirplaneEnums::kEngine_Propeller);

    EXPECT_EQ(airport.get3000ft(), static_cast<Airplane*>(NULL));
    EXPECT_TRUE(airport.is3000ftVacant());
    EXPECT_DEATH(airport.set3000ft(airplane3000ft), "Airplane is not recognized by this airport.");
    EXPECT_DEATH(airport.addAirplane(airplane3000ft), "Referenced Airplane does not have a link to the current airport.");
    EXPECT_TRUE(airport.getAirplanes().empty());
    EXPECT_NO_THROW(airplane3000ft->setAirport(&airport));
    EXPECT_NO_THROW(airport.addAirplane(airplane3000ft));
    EXPECT_FALSE(airport.getAirplanes().empty());
    EXPECT_NO_THROW(airport.set3000ft(airplane3000ft));
    EXPECT_EQ(airport.get3000ft(), airplane3000ft);
    EXPECT_FALSE(airport.is3000ftVacant());

    Airplane* airplane5000ft = new Airplane("N124", "Dummy Airplane", "Dummy", 01, 10000, 10000, 10000, 4, 4, AirplaneEnums::kStatus_Approaching, AirplaneEnums::kType_Private, AirplaneEnums::kSize_Small, AirplaneEnums::kEngine_Propeller);
    airplane5000ft->setAirport(&airport);
    airport.addAirplane(airplane5000ft);

    EXPECT_EQ(airport.get5000ft(), static_cast<Airplane*>(NULL));
    EXPECT_TRUE(airport.is5000ftVacant());
    EXPECT_NO_THROW(airport.set5000ft(airplane5000ft));
    EXPECT_EQ(airport.get5000ft(), airplane5000ft);
    EXPECT_FALSE(airport.is5000ftVacant());

    Airplane* airplaneRegistrationNumber = new Airplane("N125", "Dummy Airplane", "Dummy", 01, 10000, 10000, 10000, 4, 4, AirplaneEnums::kStatus_Approaching, AirplaneEnums::kType_Private, AirplaneEnums::kSize_Small, AirplaneEnums::kEngine_Propeller);
    airplaneRegistrationNumber->setAirport(&airport);
    airport.addAirplane(airplaneRegistrationNumber);

    EXPECT_EQ(airport.getAirplanes().size(), static_cast<unsigned int>(3));
    EXPECT_EQ(airport.getAirplanesByRegistrationNumber("N125"), airplaneRegistrationNumber);
    EXPECT_EQ(airport.getAirplanesByRegistrationNumber("N124"), airplane5000ft);
    EXPECT_EQ(airport.getAirplanesByRegistrationNumber("N123"), airplane3000ft);

    Airplane* airplaneDuplicateRegistration = new Airplane("N125", "Dummy Airplane", "Dummy", 01, 10000, 10000, 10000, 4, 4, AirplaneEnums::kStatus_Approaching, AirplaneEnums::kType_Private, AirplaneEnums::kSize_Small, AirplaneEnums::kEngine_Propeller);
    airplaneDuplicateRegistration->setAirport(&airport);
    EXPECT_DEATH(airport.addAirplane(airplaneDuplicateRegistration), "Found Airplanes with duplicate registration.");
    EXPECT_EQ(airport.getAirplanes().size(), static_cast<unsigned int>(3));

    Location* location = new Location("Dummy Location");

    EXPECT_TRUE(airport.getLocations().empty());
    EXPECT_DEATH(airport.addLocation(location), "Referenced Location does not have a link to the current airport.");
    EXPECT_NO_THROW(location->setAirport(&airport));
    EXPECT_NO_THROW(airport.addLocation(location));
    EXPECT_FALSE(airport.getLocations().empty());
    EXPECT_EQ(airport.getConnectionToApron(), location);
    EXPECT_EQ(airport.getLocationByName("Dummy Location"), location);

    Runway* runway = new Runway("Dummy Runway", 1000, RunwayEnums::kGrass);
    EXPECT_TRUE(airport.getRunways().empty());
    EXPECT_DEATH(airport.addRunway(runway), "Referenced Runway does not have a link to the current airport.");
    EXPECT_NO_THROW(runway->setAirport(&airport));
    EXPECT_NO_THROW(airport.addRunway(runway));
    EXPECT_FALSE(airport.getRunways().empty());
    EXPECT_EQ(airport.getRunwayByName("Dummy Runway"), runway);
}


TEST_F(AirportDomainTests, taxiRoute) {
    std::string airportName = "Antwerp International Airport";
    std::string airportIATA = "ANR";
    std::string airportCallsign = "Antwerp Tower";

    unsigned int gates = 10;

    Airport airport = Airport(airportName, airportIATA, airportCallsign, gates);


    Location* locationAlpha = new Location("Alpha");
    locationAlpha->setAirport(&airport);
    Runway* runwayR11 = new Runway("R11", 1000, RunwayEnums::kGrass);
    runwayR11->setAirport(&airport);
    Location* locationBravo = new Location("Bravo");
    locationBravo->setAirport(&airport);
    Runway* runwayL11 = new Runway("L11", 1000, RunwayEnums::kAsphalt);
    runwayL11->setAirport(&airport);

    locationAlpha->setNextLocation(runwayR11);

    runwayR11->setPreviousLocation(locationAlpha);
    runwayR11->setNextLocation(locationBravo);

    locationBravo->setPreviousLocation(runwayR11);
    locationBravo->setNextLocation(runwayL11);

    runwayL11->setPreviousLocation(locationBravo);

    airport.addLocation(locationAlpha);
    airport.addLocation(runwayR11);
    airport.addRunway(runwayR11);
    airport.addLocation(locationBravo);
    airport.addLocation(runwayL11);
    airport.addRunway(runwayL11);

    std::queue<Location*> taxiroute = airport.createTaxiRouteToRunway(runwayL11);

    unsigned int steps = 0;

    while (!taxiroute.empty()) {
        Location* front = taxiroute.front();

        switch(steps) {
            case 0:
                EXPECT_EQ(front, runwayR11);
                break;
            case 1:
                EXPECT_EQ(front, locationBravo);
                break;
            default:
                EXPECT_TRUE(false);
        }

        taxiroute.pop();
        steps++;
    }

    EXPECT_EQ(steps, static_cast<unsigned int>(2));
}

TEST_F(AirportDomainTests, gates) {
    std::string airportName = "Antwerp International Airport";
    std::string airportIATA = "ANR";
    std::string airportCallsign = "Antwerp Tower";

    unsigned int gates = 10;

    Airport airport = Airport(airportName, airportIATA, airportCallsign, gates);

    Airplane* airplane = new Airplane("N123", "Dummy Airplane", "Dummy", 01, 10000, 10000, 10000, 4, 4, AirplaneEnums::kStatus_Approaching, AirplaneEnums::kType_Private, AirplaneEnums::kSize_Small, AirplaneEnums::kEngine_Propeller);
    airplane->setAirport(&airport);

    EXPECT_DEATH(airport.exitGate(NULL), "Referenced Airplane is a nullpointer.");
    EXPECT_DEATH(airport.exitGate(airplane), "Referenced airplane is not linked to Airport.");

    EXPECT_DEATH(airport.reserveGate(NULL), "Referenced Airplane is a nullpointer.");
    EXPECT_DEATH(airport.reserveGate(airplane), "Referenced airplane is not linked to Airport.");
    EXPECT_NO_THROW(airport.addAirplane(airplane));

    EXPECT_DEATH(airport.exitGate(airplane), "Airplane is not properly linked to gate.");

    EXPECT_DEATH(airport.reserveGate(airplane), "Referenced Airplane is not waiting for a gate.");
    EXPECT_NO_THROW(airplane->setStatus(AirplaneEnums::kStatus_TaxiingToApron));
    EXPECT_TRUE(airport.reserveGate(airplane));
    EXPECT_DEATH(airport.reserveGate(airplane), "Referenced Airplane already has a gate assigned.");
    EXPECT_EQ(airport.getGates()[0], airplane);

    EXPECT_DEATH(airport.exitGate(airplane), "Airplane is not ready to leave gate.");
    EXPECT_NO_THROW(airplane->setStatus(AirplaneEnums::kStatus_PushingBack));
    EXPECT_NO_THROW(airport.exitGate(airplane));
    EXPECT_DEATH(airport.exitGate(airplane), "Airplane is not properly linked to gate.");
}