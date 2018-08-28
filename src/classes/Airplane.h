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
     * POST: \n
     * Airplane must be properly initialized
     * @param _number The Airplane number
     * @param _callsign The Airplane callsign
     * @param _model The Airplane model
     * @param _squawk The Airplane starting squawk
     * @param _altitude The Airplane starting altitude
     * @param _fuel The Airplane starting fuel
     * @param _fuelCapacity The Airplane starting fuel capacity
     * @param _passengers The Airplane starting fuel
     * @param _passengerCapacity The Airplane starting fuel
     * @param _status The Airplane starting fuel
     * @param _type The Airplane type
     * @param _size The Airplane size
     * @param _engine The Airplane engine
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
     * @param _airplane The Referenced airplane
     * PRE: \n
     * Referenced Airplane must be properly initialized \n
     * POST: \n
     * New airplane must be properly initialized \n
     */
    explicit Airplane(const Airplane* _airplane);

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's number
     */
    const std::string& getNumber() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's callsign
     */
    const std::string& getCallsign() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's model
     */
    const std::string& getModel() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's squawk
     */
    unsigned int getSquawk() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * The new squawk must be valid (in octal: above or equal to 1, below or equal to 6777, including 7500, 7600, 7700)
     * Post: \n
     * The Airplane's squawk must match the new squawk
     * @param _squawk The new squawk
     */
    void setSquawk(const unsigned int _squawk);

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's status not properly initialized.");
>>>>>>> 6eb14750fdc607f884990d05ca429917a49fe81d
     */
    AirplaneEnums::EStatus getStatus() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * Post: \n
     * The Airplane's status must match the new status
     * @param _status The new status
     */
    void setStatus(const AirplaneEnums::EStatus _status);

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's type
     */
    AirplaneEnums::EType getType() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's size
     */
    AirplaneEnums::ESize getSize() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's engine
     */
    AirplaneEnums::EEngine getEngine() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's altitude
     */
    unsigned int getAltitude() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * Post: \n
     * The Airplane's altitude must match the new altitude \n
     * @param _altitude The new altitude
     */
    void setAltitude(const unsigned int _altitude);

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's fuel
     */
    unsigned int getFuel() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * The new Fuel must be under or above the fuel capacity \n
     * Post: \n
     * The Airplane's altitude must match the new altitude \n
     * @param _fuel The new fuel
     */
    void setFuel(const unsigned int _fuel);

    /**
     * Calculates the fuel consumption based on engine and size \n
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's consumption rate
     */
    unsigned int getFuelConsumption() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's fuel capacity
     */
    unsigned int getFuelCapacity() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's current passenger count
     */
    unsigned int getPassengers() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * The new passenger count must be under or equal to the passenger capacity \n
     * Post: \n
     * The Airplane's passenger must match the new passenger count \n
     * @param _fuel The new passenger count
     */
    void setPassengers(const unsigned int _passengers);

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's current passenger count
     */
    unsigned int getPassengerCapacity() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's current permission
     */
    bool hasPermission() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * Post: \n
     * The Airplane's permission must match the new permission \n
     * @param t_permission The new permission
     */
    void setPermission(const bool t_permission);

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's action timer
     */
    unsigned int getActionTimer() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * Post: \n
     * The Airplane's action timer must match the new action timer \n
     * @param t_permission The new action timer
     */
    void setActionTimer(const unsigned int t_actionTimer);

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * Post: \n
     * @param addition The addition to the action timer
     */
    void increaseActionTimer(const unsigned int addition = 1);

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's communication timer
     */
    unsigned int getCommunicationTimer() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * Post: \n
     * The Airplane's communication timer must match the new communication timer \n
     * @param t_communicationTimer The new communication timer
     */
    void setCommunicationTimer(const unsigned int t_communicationTimer);

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * Post: \n
     * @param addition The addition to the communication timer
     */
    void increaseCommunicationTimer(const unsigned int addition = 1);

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's linked airport
     */
    Airport* getAirport() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * Post: \n
     * The Airplane's gate, current location, runway, must be reset \n
     * The Airplane's linked airport must match the new linked airport \n
     * @param t_communicationTimer The new linked airport
     */
    void setAirport(Airport* _airport);

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's gate number
     */
    const long getGate() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * Post: \n
     * The Airplane's new gate must match the new gate \n
     * @param t_communicationTimer The new linked airport
     */
    void setGate(unsigned long _gate);

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's current runway
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
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's current location
     */
    Location* getCurrentLocation() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    void setCurrentLocation(Location* _location);

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's taxi route (pointer)
     */
    std::queue<Location*>& getTaxiRoute();

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's taxi route (copy)
     */
    std::queue<Location*> getTaxiRouteCopy() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    void setTaxiRoute(const std::queue<Location*>& p_taxiRoute);

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's flying status
     */
    bool isFlying() const;

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's current location
     */
    bool fly();

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @return the Airplane's current location
     */
    bool descend(const unsigned int t_subtraction);

    void ascend(const unsigned int t_addition);

    /**
     * Pre: \n
     * Airplane must be properly initialized \n
     * @@param stream the output stream for the Airplan's information
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

