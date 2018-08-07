#ifndef INC_LOCATION_H
#define INC_LOCATION_H

#include "../lib/DesignByContract.h"
#include <string>

class Airport;

class Location {
protected:
    const std::string name;

    bool properlyInitialized() const;

    Location* previousLocation;
    Location* nextLocation;

    const Airport* airport;

    Location* init;

public:
    Location(const std::string& _name);

    Location(const Location* _location);

    const std::string& getName() const;

    Location* getPreviousLocation() const;

    void setPreviousLocation(Location* _previousLocation);

    Location* getNextLocation() const;

    void setNextLocation(Location* _nextLocation);

    const Airport* getAirport() const;

    void setAirport(const Airport* _airport);
};


#endif
