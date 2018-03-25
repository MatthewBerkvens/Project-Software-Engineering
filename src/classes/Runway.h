#ifndef INC_RUNWAY_H
#define INC_RUNWAY_H

#include "../lib/DesignByContract.h"

#include <string>
#include <iostream>

class Airport;


class Runway {
private:
    const std::string runwayName;
    const Airport* airport;

    const Runway* init;

    bool properlyInitialized() const;

public:
    /**
     * POST: \n
     * ENSURE(properlyInitialized(), "Runway was not properly initialized.");
     */
    Runway(const std::string& _runwayName,
           const Airport* _airport);

    /**
     * PRE: \n
     * REQUIRE(runway->properlyInitialized(), "Reference Runway was not properly initialized."); \n
     * POST: \n
     * ENSURE(properlyInitialized(), "Runway was not properly initialized.");
     */
    Runway(const Runway* _runway);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
     */
    const std::string& getRunwayName() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
     */
    const Airport* getAirport() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Runway was not properly initialized."); \n
     * POST: \n
     * ENSURE(airport == _airport, "References Airport was not properly set.");
     */
    //void Runway::setAirport(const Airport* _airport);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
     */
    void printInfo(std::ostream& stream) const;
};

#endif
