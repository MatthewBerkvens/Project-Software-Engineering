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

    AirplaneVector gates;
    AirplaneVector airplanes;
    RunwayVector runways;

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

    /**
     * PRE: \n
     * REQUIRE(this->properlyInitialized(), "Airport was not properly initialized.");
     */
    const std::string& getAirportName() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
     */
    const std::string& getIata() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
     */
    const std::string& getCallsign() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
     */
    const AirplaneVector& getGates() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
     */
    AirplaneVector& getModifiableGates();

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
     */
    const unsigned long getGateCount() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
     * POST: \n
     *
     */
    const long getEmptyGate(Airplane* _airplane) const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
     * POST: \n
     *
     */
    void enterGate(Airplane* _airplane, unsigned long _gate);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
     * POST: \n
     *
     */
    void exitGate(Airplane* _airplane);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
     */
    const RunwayVector& getRunways() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
     */
    RunwayVector& getModifiableRunways();

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
     */
    const AirplaneVector& getAirplanes() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
     */
    AirplaneVector& getModifiableAirplanes();

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized."); \n
     * REQUIRE(_airplane != NULL, "Referenced Airplane was a nullpointer."); \n
     * REQUIRE(_airplane->getAirport() == this, "Referenced Airplane does not have a link to the current airport."); \n
     * REQUIRE(!vectorContains<Runway*>(airplanes, _airplane), "Referenced Airplane already added to the airport."); \n
     * POST: \n
     * ENSURE(vectorContains<Runway*>(airplanes, _airplane), "Referenced Airplane was not properly added.");
     */
    void addAirplane(Airplane* _airplane);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized."); \n
     * REQUIRE(_runway != NULL, "Referenced Runway was a nullpointer."); \n
     * REQUIRE(_runway->getAirport() == this, "Referenced Runway does not have a link to the current airport."); \n
     * REQUIRE(!vectorContains<Runway*>(runways, _runway), "Referenced Runway already added to the airport."); \n
     * POST: \n
     * ENSURE(vectorContains<Runway*>(runways, _runway), "Referenced Runway was not properly added.");
     */
    void addRunway(Runway* _runway);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized."); \n
     * REQUIRE(vectorContains<Runway*>(runways, _runway), "Referenced Runway was not found, can't be deleted."); \n
     * POST: \n
     * ENSURE(!vectorContains<Runway*>(runways, _runway), "Referenced Runway was not properly deleted."); \n
     * ENSURE(_runway->getAirport() == NULL, "Referenced Airport was not properly unlinked from this airport");
     */
    //void deleteRunway(Runway* _runway);

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
    Runway* getFreeCompatibleRunway(const Airplane* airplane);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
     */
    void printInfo(std::ostream& stream) const;
};


#endif //PROJECT_SOFTWARE_ENGINEERING_AIRPORT_H
