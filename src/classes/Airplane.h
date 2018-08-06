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
        estatus(LeftAirport), \
        estatus(StandingAtGate), \
        estatus(TaxiingToGate), \
        estatus(TaxiingToRunway), \
        estatus(ReadyForTakeoff), \
        estatus(WaitingForEmptyGate), \
        estatus(Unboarding), \
        estatus(Boarding), \
        estatus(Refueling), \
        estatus(TechnicalCheckup), \
        estatus(PushingBack), \
        estatus(HoldingShort), \
        estatus(LiningUp), \
        estatus(TakingOff), \
        estatus(CrossingRunway), \
        estatus(Ascending), \
        estatus(DescendingTo5000ft), \
        estatus(DescendingTo3000ft), \
        estatus(FlyingWaitPattern), \
        estatus(FinalApproach), \
        estatus(EmergencyApproach), \
        estatus(Landing), \
        estatus(EmergencyLanding)

    #define estatus(x) k##x
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

    #define etype(x) k##x
        enum EType { EAIRPLANETYPES };
    #undef etype

    const char* EnumToString(EType f);

    EType StringToTypeEnum(const char *f);



    #define ESIZES \
        esize(InvalidSize), \
        esize(Small), \
        esize(Medium), \
        esize(Large)

    #define esize(x) k##x
        enum ESize { ESIZES };
    #undef esize

    const char* EnumToString(ESize f);

    ESize StringToSizeEnum(const char *f);



    #define EENGINES \
        eengine(InvalidEngine), \
        eengine(Propeller), \
        eengine(Jet)

    #define eengine(x) k##x
        enum EEngine { EENGINES };
    #undef eengine

    const char* EnumToString(EEngine f);

    EEngine StringToEngineEnum(const char *f);

    /*enum EStatus {
        kInvalidStatus,
        kApproaching,
        kStandingAtGate,
        kTaxiingToGate,
        kTaxiingToRunway,
        kReadyForTakeoff,
        kFinalApproach,
        kWaitingForEmptyGate,
        kFinishedBoarding
    };

    enum EType {
        kInvalidType,
        kPrivate,
        kAirline,
        kMilitary,
        kEmergency
    };

    enum ESize {
        kInvalidSize,
        kSmall,
        kMedium,
        kLarge
    };

    enum EEngine {
        kInvalidEngine,
        kPropeller,
        kJet
    };

    extern const std::map<std::string, EStatus> gStringToAirplaneStatus;

    extern const std::map<EStatus, std::string> gAirplaneStatusToString;*/
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

    bool waitingForInstructions;
    unsigned int internalTimer;

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

    bool getWaitingForInstructions() const;

    void setWaitingForInstructions(const bool _waitingForInstructions);

    unsigned int getInternalTimer() const;

    void setInternalTimer(const unsigned int _internalTimer);

    void increaseInternalTimer(const unsigned int addition = 1);

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
    Location* getCurrentLocation();

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    void setCurrentLocation(Location* _location);

    void fly();

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
