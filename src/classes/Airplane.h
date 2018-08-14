#ifndef INC_AIRPLANE_H
#define INC_AIRPLANE_H

#include "../lib/DesignByContract.h"

#include <queue>
#include <iostream>
#include <string>
#include <string.h>
#include <map>

namespace AirplaneEnums {
    #define ESTATUS \
        estatus(InvalidStatus), \
        estatus(Approaching), \
        estatus(DescendingTo5000ft), \
        estatus(DescendingTo3000ft), \
        estatus(FlyingWaitPattern), \
        estatus(FinalApproach), \
        estatus(Landing), \
        estatus(Vacate), \
        estatus(TaxiingToApron), \
        estatus(TaxiingToRunway), \
        estatus(TaxiingToCrossing), \
        estatus(WaitingAtCrossing), \
        estatus(CrossingRunway), \
        estatus(Unboarding), \
        estatus(TechnicalCheckup), \
        estatus(Refueling), \
        estatus(Boarding), \
        estatus(StandingAtGate), \
        estatus(PushingBack), \
        estatus(HoldingShort), \
        estatus(LiningUp), \
        estatus(ReadyForTakeoff), \
        estatus(TakingOff), \
        estatus(Ascending), \
        estatus(LeftAirport), \
        estatus(Emergency), \
        estatus(EmergencyFinalApproach), \
        estatus(EmergencyLanding), \
        estatus(EmergencyEvacuation), \
        estatus(EmergencyCheckup), \
        estatus(EmergencyRefueling), \
        estatus(Crashed)

    #define estatus(x) kStatus_##x
        enum EStatus { ESTATUS };
    #undef estatus

    const char* EnumToString(EStatus f);

    EStatus StringToStatusEnum(const char *f);



    #define EAIRPLANETYPES \
        etype(InvalidType), \
        etype(Private), \
        etype(Airline), \
        etype(Military), \
        etype(Emergency)

    #define etype(x) kType_##x
        enum EType { EAIRPLANETYPES };
    #undef etype

    const char* EnumToString(EType f);

    EType StringToTypeEnum(const char *f);



    #define ESIZES \
        esize(InvalidSize), \
        esize(Small), \
        esize(Medium), \
        esize(Large)

    #define esize(x) kSize_##x
        enum ESize { ESIZES };
    #undef esize

    const char* EnumToString(ESize f);

    ESize StringToSizeEnum(const char *f);



    #define EENGINES \
        eengine(InvalidEngine), \
        eengine(Propeller), \
        eengine(Jet)

    #define eengine(x) kEngine_##x
        enum EEngine { EENGINES };
    #undef eengine

    const char* EnumToString(EEngine f);

    EEngine StringToEngineEnum(const char *f);
}


class Airport;
class Location;
class Runway;

class Airplane {
private:
    const std::string number;
    const std::string callsign;
    const std::string model;

    unsigned int squawk;
    unsigned int altitude;
    unsigned int fuel;
    const unsigned int fuelCapacity;
    unsigned int passengers;
    const unsigned int passengerCapacity;

    AirplaneEnums::EStatus status;
    AirplaneEnums::EType type;
    AirplaneEnums::ESize size;
    AirplaneEnums::EEngine engine;

    bool permission;
    unsigned int actionTimer;
    unsigned int communicationTimer;

    Airport* airport;
    unsigned long gate;

    Runway* runway;
    Location* currentLocation;
    std::queue<Location*> taxiRoute;

    const Airplane* init;

    bool properlyInitialized() const;

public:
    /**
     * PRE: \n
     * ENSURE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    Airplane(const std::string& _number,
             const std::string& _callsign,
             const std::string& _model,
             unsigned int _squawk,
             const unsigned int _altitude,
             const unsigned int _fuel,
             const unsigned int _fuelCapacity,
             const unsigned int _passengers,
             const unsigned int _passengerCapacity,
             const AirplaneEnums::EStatus _status,
             const AirplaneEnums::EType _type,
             const AirplaneEnums::ESize _size,
             const AirplaneEnums::EEngine _engine);

    /**
     * PRE: \n
     * REQUIRE(runway->properlyInitialized(), "Reference Airplane was not properly initialized."); \n
     * POST: \n
     * ENSURE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    explicit Airplane(const Airplane* _airplane);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    const std::string& getNumber() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    const std::string& getCallsign() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    const std::string& getModel() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    unsigned int getSquawk() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     * POST: \n
     * ENSURE(squawk == _squawk, "Property 'squawk' was not correctly set in Airplane.");
     */
    void setSquawk(const unsigned int _squawk);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    AirplaneEnums::EStatus getStatus() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized."); \n
     * POST: \n
     * ENSURE(status == _status, "Property 'status' was not correctly set in Airplane.");
     */
    void setStatus(const AirplaneEnums::EStatus _status);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    AirplaneEnums::EType getType() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    AirplaneEnums::ESize getSize() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    AirplaneEnums::EEngine getEngine() const;

    unsigned int getAltitude() const;

    void setAltitude(const unsigned int _altitude);

    unsigned int getFuel() const;

    void setFuel(const unsigned int _fuel);

    unsigned int getFuelConsumption() const;

    unsigned int getFuelCapacity() const;

    unsigned int getPassengers() const;

    void setPassengers(const unsigned int _passengers);

    unsigned int getPassengerCapacity() const;

    bool hasPermission() const;

    void setPermission(const bool t_permission);

    unsigned int getActionTimer() const;

    void setActionTimer(const unsigned int t_actionTimer);

    void increaseActionTimer(const unsigned int addition = 1);

    unsigned int getCommunicationTimer() const;

    void setCommunicationTimer(const unsigned int t_communicationTimer);

    void increaseCommunicationTimer(const unsigned int addition = 1);


    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    Airport* getAirport() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized."); \n
     * POST: \n
     * ENSURE(airport == _airport, "Property 'airport' was not correctly set in Airplane.");
     */
    void setAirport(Airport* _airport);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    const long getGate() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    void setGate(unsigned long _gate);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    Runway* getRunway() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized."); \n
     * POST: \n
     * ENSURE(runway == _runway, "Property 'runway' was not correctly set in Airplane.");
     */
    void setRunway(Runway* _runway);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    Location* getCurrentLocation() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    void setCurrentLocation(Location* _location);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    std::queue<Location*>& getTaxiRoute();

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    std::queue<Location*> getTaxiRouteCopy() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    void setTaxiRoute(const std::queue<Location*>& p_taxiRoute);

    bool isFlying() const;

    bool fly();

    bool descend(const unsigned int t_subtraction);

    void ascend(const unsigned int t_addition);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    void printInfo(std::ostream& stream) const;
};


/**
 * Compares the squawks of 2 airplanes.
 */
struct CompareSquawk {
    /**
     * @param p1 The first airplane.
     * @param p2 The second airplane.
     * @return True if squawk of second airplane is larger than the first airplane, false if not.
     */
    bool operator()(const Airplane* p1, const Airplane* p2) {
        return p1->getSquawk() < p2->getSquawk();
    }
};

#endif
