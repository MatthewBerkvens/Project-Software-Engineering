#ifndef INC_AIRPORT_H
#define INC_AIRPORT_H

#include "../lib/DesignByContract.h"
#include "../utils/utils.h"
#include "Location.h"
#include "Runway.h"
#include "Airplane.h"

#include <string>
#include <vector>
#include <iostream>

class Airport {
private:
    const std::string airportName;
    const std::string iata;
    const std::string callsign;

    AirplaneMap airplanes;
    RunwayMap runways;
    LocationMap locations;
    AirplaneVector gates;

    const Airplane* ft5000;
    const Airplane* ft3000;

    const Airport* init;

    bool properlyInitialized() const;

public:
    /**
     * POST: \n
     * ENSURE(properlyInitialized(), "Airport was not properly initialized.");
     */
    Airport(const std::string& _airportName,
            const std::string& _iata,
            const std::string& _callsign,
            unsigned int _gateCount);

    /**
     * PRE: \n
     * REQUIRE(airport->properlyInitialized(), "Reference Airport was not properly initialized."); \n
     * POST: \n
     * ENSURE(properlyInitialized(), "Airport was not properly initialized.");
     */
    Airport(const Airport* _airport);

    ~Airport();

    const std::string& getAirportName() const;

    const std::string& getIata() const;

    const std::string& getCallsign() const;


    const AirplaneMap& getAirplanes() const;

    AirplaneMap& getAirplanes();

    const Airplane* getAirplanesByRegistrationNumber(const std::string& number) const;

    Airplane* getAirplanesByRegistrationNumber(const std::string& number);

    void addAirplane(Airplane* _airplane);


    const RunwayMap& getRunways() const;

    RunwayMap& getRunways();

    const Runway* getRunwayByName(const std::string& name) const;

    Runway* getRunwayByName(const std::string& name);

    Runway* getFreeCompatibleRunway(const Airplane* airplane);

    void addRunway(Runway* _runway);


    const LocationMap& getLocations() const;

    LocationMap& getLocations();

    const Location* getLocationByName(const std::string& name) const;

    Location* getLocationByName(const std::string& name);

    void addLocation(Location* _location);


    const AirplaneVector& getGates() const;

    AirplaneVector& getGates();

    const unsigned long getGateCount() const;

    bool enterGate(Airplane* _airplane);

    void exitGate(Airplane* _airplane);


    const Airplane* get3000ft() const;

    void set3000ft(Airplane* _airplane);

    bool is3000ftVacant() const;

    void set5000ft(Airplane* _airplane);

    const Airplane* get5000ft() const;

    bool is5000ftVacant() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
     */
    void printInfo(std::ostream& stream) const;
};


#endif //PROJECT_SOFTWARE_ENGINEERING_AIRPORT_H
