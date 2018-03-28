#ifndef INC_AIRPORT_H
#define INC_AIRPORT_H

#include "../lib/DesignByContract.h"
#include "../utils/utils.h"
#include "Runway.h"

#include <string>
#include <vector>
#include <iostream>

class Airport {
private:
    const std::string airportName;
    const std::string iata;
    const std::string callsign;
    const unsigned int gates;

    RunwayVector runways;

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
            const unsigned int _gates);

    /**
     * PRE: \n
     * REQUIRE(airport->properlyInitialized(), "Reference Airport was not properly initialized."); \n
     * POST: \n
     * ENSURE(properlyInitialized(), "Airport was not properly initialized.");
     */
    Airport(Airport* _airport);

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
    const unsigned int getGates() const;

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

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airport was not properly initialized.");
     */
    void printInfo(std::ostream& stream) const;
};


#endif //PROJECT_SOFTWARE_ENGINEERING_AIRPORT_H
