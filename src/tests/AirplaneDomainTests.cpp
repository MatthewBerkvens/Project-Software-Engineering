#include "../classes/Airplane.h"
#include "../classes/Airport.h"
#include "../classes/Runway.h"
#include "../classes/Location.h"

#include <gtest/gtest.h>

class AirplaneDomainTests: public ::testing::Test {
protected:
    friend class Airplane;

    virtual void SetUp() {
        ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    };

    virtual void TearDown() {

    };
};

TEST_F(AirplaneDomainTests, initializerAndGetters) {
    std::string airplaneNumber = "N11842";
    std::string airplaneCallsign = "Cessna 842";
    std::string airplaneModel = "Cessna 430";

    unsigned int airplaneSquawk = 01;
    unsigned int airplaneAltitude = 10000;

    unsigned int fuelCapacity = 20000;
    unsigned int passengerCapacity = 4;

    AirplaneEnums::EStatus airplaneStatus = AirplaneEnums::kStatus_Approaching;
    AirplaneEnums::EType airplaneType = AirplaneEnums::kType_Private;
    AirplaneEnums::ESize airplaneSize = AirplaneEnums::kSize_Small;
    AirplaneEnums::EEngine airplaneEngine = AirplaneEnums::kEngine_Propeller;

    Airplane airplane = Airplane(airplaneNumber, airplaneCallsign, airplaneModel, airplaneSquawk, airplaneAltitude, fuelCapacity, fuelCapacity, passengerCapacity, passengerCapacity, airplaneStatus, airplaneType, airplaneSize, airplaneEngine);

    EXPECT_EQ(airplane.getNumber(), airplaneNumber);
    EXPECT_EQ(airplane.getCallsign(), airplaneCallsign);
    EXPECT_EQ(airplane.getModel(), airplaneModel);
    EXPECT_EQ(airplane.getSquawk(), airplaneSquawk);
    EXPECT_EQ(airplane.getAltitude(), airplaneAltitude);
    EXPECT_EQ(airplane.getFuel(), fuelCapacity);
    EXPECT_EQ(airplane.getFuelCapacity(), fuelCapacity);
    EXPECT_EQ(airplane.getPassengers(), passengerCapacity);
    EXPECT_EQ(airplane.getPassengerCapacity(), passengerCapacity);
    EXPECT_EQ(airplane.getStatus(), airplaneStatus);
    EXPECT_EQ(airplane.getType(), airplaneType);
    EXPECT_EQ(airplane.getSize(), airplaneSize);
    EXPECT_EQ(airplane.getEngine(), airplaneEngine);

    EXPECT_EQ(airplane.getGate(), -1);
    EXPECT_TRUE(airplane.getTaxiRoute().empty());
    EXPECT_TRUE(airplane.getTaxiRouteCopy().empty());

    EXPECT_EQ(airplane.getActionTimer(), static_cast<unsigned int>(0));
    EXPECT_EQ(airplane.getCommunicationTimer(), static_cast<unsigned int>(0));
    EXPECT_FALSE(airplane.hasPermission());

    EXPECT_EQ(airplane.getAirport(), static_cast<Airport*>(NULL));
    EXPECT_EQ(airplane.getCurrentLocation(), static_cast<Location*>(NULL));
    EXPECT_EQ(airplane.getRunway(), static_cast<Runway*>(NULL));
    EXPECT_EQ(airplane.getFuelConsumption(), static_cast<unsigned int>(10));

    EXPECT_TRUE(airplane.isFlying());
}

TEST_F(AirplaneDomainTests, deepCopy) {
    std::string airplaneNumber = "N11842";
    std::string airplaneCallsign = "Cessna 842";
    std::string airplaneModel = "Cessna 430";

    unsigned int airplaneSquawk = 01;
    unsigned int airplaneAltitude = 10000;

    unsigned int fuelCapacity = 20000;
    unsigned int passengerCapacity = 4;

    AirplaneEnums::EStatus airplaneStatus = AirplaneEnums::kStatus_Approaching;
    AirplaneEnums::EType airplaneType = AirplaneEnums::kType_Private;
    AirplaneEnums::ESize airplaneSize = AirplaneEnums::kSize_Small;
    AirplaneEnums::EEngine airplaneEngine = AirplaneEnums::kEngine_Propeller;

    Airplane* airplane = new Airplane(airplaneNumber, airplaneCallsign, airplaneModel, airplaneSquawk, airplaneAltitude, fuelCapacity, fuelCapacity, passengerCapacity, passengerCapacity, airplaneStatus, airplaneType, airplaneSize, airplaneEngine);

    Airplane deepCopy = Airplane(airplane);

    delete airplane;

    EXPECT_EQ(deepCopy.getNumber(), airplaneNumber);
    EXPECT_EQ(deepCopy.getCallsign(), airplaneCallsign);
    EXPECT_EQ(deepCopy.getModel(), airplaneModel);
    EXPECT_EQ(deepCopy.getSquawk(), airplaneSquawk);
    EXPECT_EQ(deepCopy.getAltitude(), airplaneAltitude);
    EXPECT_EQ(deepCopy.getFuel(), fuelCapacity);
    EXPECT_EQ(deepCopy.getFuelCapacity(), fuelCapacity);
    EXPECT_EQ(deepCopy.getPassengers(), passengerCapacity);
    EXPECT_EQ(deepCopy.getPassengerCapacity(), passengerCapacity);
    EXPECT_EQ(deepCopy.getStatus(), airplaneStatus);
    EXPECT_EQ(deepCopy.getType(), airplaneType);
    EXPECT_EQ(deepCopy.getSize(), airplaneSize);
    EXPECT_EQ(deepCopy.getEngine(), airplaneEngine);

    EXPECT_EQ(deepCopy.getGate(), -1);
    EXPECT_TRUE(deepCopy.getTaxiRoute().empty());
    EXPECT_TRUE(deepCopy.getTaxiRouteCopy().empty());

    EXPECT_EQ(deepCopy.getActionTimer(), static_cast<unsigned int>(0));
    EXPECT_EQ(deepCopy.getCommunicationTimer(), static_cast<unsigned int>(0));
    EXPECT_FALSE(deepCopy.hasPermission());

    EXPECT_EQ(deepCopy.getAirport(), static_cast<Airport*>(NULL));
    EXPECT_EQ(deepCopy.getCurrentLocation(), static_cast<Location*>(NULL));
    EXPECT_EQ(deepCopy.getRunway(), static_cast<Runway*>(NULL));
    EXPECT_EQ(deepCopy.getFuelConsumption(), static_cast<unsigned int>(10));
}

TEST_F(AirplaneDomainTests, setters) {
    std::string airplaneNumber = "N11842";
    std::string airplaneCallsign = "Cessna 842";
    std::string airplaneModel = "Cessna 430";

    unsigned int airplaneSquawk = 01;
    unsigned int airplaneAltitude = 10000;

    unsigned int fuelCapacity = 20000;
    unsigned int passengerCapacity = 4;

    AirplaneEnums::EStatus airplaneStatus = AirplaneEnums::kStatus_Approaching;
    AirplaneEnums::EType airplaneType = AirplaneEnums::kType_Private;
    AirplaneEnums::ESize airplaneSize = AirplaneEnums::kSize_Small;
    AirplaneEnums::EEngine airplaneEngine = AirplaneEnums::kEngine_Propeller;

    Airplane airplane = Airplane(airplaneNumber, airplaneCallsign, airplaneModel, airplaneSquawk, airplaneAltitude, fuelCapacity, fuelCapacity, passengerCapacity, passengerCapacity, airplaneStatus, airplaneType, airplaneSize, airplaneEngine);

    EXPECT_EQ(airplane.getSquawk(), airplaneSquawk);
    unsigned int newAirplaneSquawk = 03;
    airplane.setSquawk(newAirplaneSquawk);
    EXPECT_EQ(airplane.getSquawk(), newAirplaneSquawk);

    EXPECT_EQ(airplane.getAltitude(), airplaneAltitude);
    unsigned int newAirplaneAltitude = 5000;
    airplane.setAltitude(newAirplaneAltitude);
    EXPECT_EQ(airplane.getAltitude(), newAirplaneAltitude);

    EXPECT_EQ(airplane.getFuel(), fuelCapacity);
    unsigned int newFuel = 10000;
    airplane.setFuel(newFuel);
    EXPECT_EQ(airplane.getFuel(), newFuel);
    unsigned int newFuelOverCapacity = 30000;
    EXPECT_DEATH(airplane.setFuel(newFuelOverCapacity), "Airplane attempted to set property 'fuel' higher than the capacity");
    EXPECT_EQ(airplane.getFuel(), newFuel);

    EXPECT_EQ(airplane.getPassengers(), passengerCapacity);
    unsigned int newPassengers = 3;
    airplane.setPassengers(newPassengers);
    EXPECT_EQ(airplane.getPassengers(), newPassengers);
    unsigned int newPassengersOverCapacity = 5;
    EXPECT_DEATH(airplane.setPassengers(newPassengersOverCapacity), "Airplane attempted to set property 'passengers' higher than the capacity");
    EXPECT_EQ(airplane.getPassengers(), newPassengers);

    EXPECT_EQ(airplane.getStatus(), airplaneStatus);
    AirplaneEnums::EStatus newAirplaneStatus = AirplaneEnums::kStatus_Landing;
    airplane.setStatus(newAirplaneStatus);
    EXPECT_EQ(airplane.getStatus(), newAirplaneStatus);

    EXPECT_EQ(airplane.getGate(), -1);
    airplane.setGate(5);
    EXPECT_EQ(airplane.getGate(), 5);

    EXPECT_TRUE(airplane.getTaxiRoute().empty());




    EXPECT_TRUE(airplane.getTaxiRouteCopy().empty());





    EXPECT_EQ(airplane.getActionTimer(), static_cast<unsigned int>(0));
    airplane.setActionTimer(static_cast<unsigned int>(5));
    EXPECT_EQ(airplane.getActionTimer(), static_cast<unsigned int>(5));
    airplane.increaseActionTimer();
    EXPECT_EQ(airplane.getActionTimer(), static_cast<unsigned int>(6));
    airplane.increaseActionTimer(3);
    EXPECT_EQ(airplane.getActionTimer(), static_cast<unsigned int>(9));

    EXPECT_EQ(airplane.getCommunicationTimer(), static_cast<unsigned int>(0));
    airplane.setCommunicationTimer(static_cast<unsigned int>(5));
    EXPECT_EQ(airplane.getCommunicationTimer(), static_cast<unsigned int>(5));
    airplane.increaseCommunicationTimer();
    EXPECT_EQ(airplane.getCommunicationTimer(), static_cast<unsigned int>(6));
    airplane.increaseCommunicationTimer(3);
    EXPECT_EQ(airplane.getCommunicationTimer(), static_cast<unsigned int>(9));

    EXPECT_FALSE(airplane.hasPermission());
    airplane.setPermission(true);
    EXPECT_TRUE(airplane.hasPermission());

    EXPECT_EQ(airplane.getAirport(), static_cast<Airport*>(NULL));
    Airport airport = Airport("Dummy Airport", "DUM", "Dummy", 3);
    airplane.setAirport(&airport);
    EXPECT_EQ(airplane.getAirport(), &airport);

    EXPECT_EQ(airplane.getCurrentLocation(), static_cast<Location*>(NULL));
    Location location = Location("Dummy Airport");
    airplane.setCurrentLocation(&location);
    EXPECT_EQ(airplane.getCurrentLocation(), &location);

    EXPECT_EQ(airplane.getRunway(), static_cast<Runway*>(NULL));
    Runway runway = Runway("Dummy Airport", 1000, RunwayEnums::kGrass);
    airplane.setRunway(&runway);
    EXPECT_EQ(airplane.getRunway(), &runway);
    airplane.setCurrentLocation(&runway);
    EXPECT_EQ(airplane.getRunway(), &runway);
}

TEST_F(AirplaneDomainTests, complexMethods) {
    std::string airplaneNumber = "N11842";
    std::string airplaneCallsign = "Cessna 842";
    std::string airplaneModel = "Cessna 430";

    unsigned int airplaneSquawk = 01;
    unsigned int airplaneAltitude = 10000;

    unsigned int fuelCapacity = 20000;
    unsigned int passengerCapacity = 4;

    AirplaneEnums::EStatus airplaneStatus = AirplaneEnums::kStatus_Approaching;
    AirplaneEnums::EType airplaneType = AirplaneEnums::kType_Private;
    AirplaneEnums::ESize airplaneSize = AirplaneEnums::kSize_Small;
    AirplaneEnums::EEngine airplaneEngine = AirplaneEnums::kEngine_Propeller;

    Airplane airplane = Airplane(airplaneNumber, airplaneCallsign, airplaneModel, airplaneSquawk, airplaneAltitude, fuelCapacity, fuelCapacity, passengerCapacity, passengerCapacity, airplaneStatus, airplaneType, airplaneSize, airplaneEngine);

    EXPECT_EQ(airplane.getAltitude(), static_cast<unsigned int>(10000));
    EXPECT_TRUE(airplane.descend(1000));
    EXPECT_EQ(airplane.getAltitude(), static_cast<unsigned int>(9000));
    EXPECT_TRUE(airplane.descend(2000));
    EXPECT_EQ(airplane.getAltitude(), static_cast<unsigned int>(7000));
    EXPECT_TRUE(airplane.descend(7000));
    EXPECT_EQ(airplane.getAltitude(), static_cast<unsigned int>(0));
    EXPECT_FALSE(airplane.descend(1));
    EXPECT_EQ(airplane.getAltitude(), static_cast<unsigned int>(0));
    airplane.setAltitude(1000);
    EXPECT_EQ(airplane.getAltitude(), static_cast<unsigned int>(1000));
    //Airplane is supposed to descend as far as possible, and return true if starting altitude is not 0
    EXPECT_TRUE(airplane.descend(2000));
    EXPECT_EQ(airplane.getAltitude(), static_cast<unsigned int>(0));

    airplane.ascend(1000);
    EXPECT_EQ(airplane.getAltitude(), static_cast<unsigned int>(1000));
    airplane.ascend(2000);
    EXPECT_EQ(airplane.getAltitude(), static_cast<unsigned int>(3000));

    EXPECT_TRUE(airplane.fly());
    EXPECT_EQ(airplane.getAltitude(), static_cast<unsigned int>(3000));
    EXPECT_EQ(airplane.getFuel(), static_cast<unsigned int>(19990));
    airplane.setFuel(8);
    EXPECT_FALSE(airplane.fly());
    EXPECT_EQ(airplane.getFuel(), static_cast<unsigned int>(0));
    EXPECT_EQ(airplane.getAltitude(), static_cast<unsigned int>(2500));
    EXPECT_FALSE(airplane.fly());
    EXPECT_EQ(airplane.getFuel(), static_cast<unsigned int>(0));
    EXPECT_EQ(airplane.getAltitude(), static_cast<unsigned int>(2000));
}

TEST_F(AirplaneDomainTests, fuelConsumption) {
    std::string airplaneNumber = "N11842";
    std::string airplaneCallsign = "Cessna 842";
    std::string airplaneModel = "Cessna 430";

    unsigned int airplaneSquawk = 01;
    unsigned int airplaneAltitude = 10000;

    unsigned int fuelCapacity = 20000;
    unsigned int passengerCapacity = 4;

    AirplaneEnums::EStatus airplaneStatus = AirplaneEnums::kStatus_Approaching;
    AirplaneEnums::EType airplaneType = AirplaneEnums::kType_Private;

    AirplaneEnums::ESize small = AirplaneEnums::kSize_Small;
    AirplaneEnums::ESize medium = AirplaneEnums::kSize_Medium;
    AirplaneEnums::ESize large = AirplaneEnums::kSize_Large;

    AirplaneEnums::EEngine propeller = AirplaneEnums::kEngine_Propeller;
    AirplaneEnums::EEngine jet = AirplaneEnums::kEngine_Jet;


    Airplane smallPropeller = Airplane(airplaneNumber, airplaneCallsign, airplaneModel, airplaneSquawk, airplaneAltitude, fuelCapacity, fuelCapacity, passengerCapacity, passengerCapacity,
            airplaneStatus, airplaneType, small, propeller);
    EXPECT_EQ(smallPropeller.getFuelConsumption(), static_cast<unsigned int>(10));

    Airplane smallJet = Airplane(airplaneNumber, airplaneCallsign, airplaneModel, airplaneSquawk, airplaneAltitude, fuelCapacity, fuelCapacity, passengerCapacity, passengerCapacity,
                                       airplaneStatus, airplaneType, small, jet);
    EXPECT_EQ(smallJet.getFuelConsumption(), static_cast<unsigned int>(25));

    Airplane mediumPropeller = Airplane(airplaneNumber, airplaneCallsign, airplaneModel, airplaneSquawk, airplaneAltitude, fuelCapacity, fuelCapacity, passengerCapacity, passengerCapacity,
                                       airplaneStatus, airplaneType, medium, propeller);
    EXPECT_EQ(mediumPropeller.getFuelConsumption(), static_cast<unsigned int>(50));

    Airplane mediumJet = Airplane(airplaneNumber, airplaneCallsign, airplaneModel, airplaneSquawk, airplaneAltitude, fuelCapacity, fuelCapacity, passengerCapacity, passengerCapacity,
                                       airplaneStatus, airplaneType, medium, jet);
    EXPECT_EQ(mediumJet.getFuelConsumption(), static_cast<unsigned int>(175));

    Airplane largePropeller = Airplane(airplaneNumber, airplaneCallsign, airplaneModel, airplaneSquawk, airplaneAltitude, fuelCapacity, fuelCapacity, passengerCapacity, passengerCapacity,
                                 airplaneStatus, airplaneType, large, propeller);
    EXPECT_EQ(largePropeller.getFuelConsumption(),  static_cast<unsigned int>(100));

    Airplane largeJet = Airplane(airplaneNumber, airplaneCallsign, airplaneModel, airplaneSquawk, airplaneAltitude, fuelCapacity, fuelCapacity, passengerCapacity, passengerCapacity,
                                  airplaneStatus, airplaneType, large, jet);
    EXPECT_EQ(largeJet.getFuelConsumption(), static_cast<unsigned int>(250));
}