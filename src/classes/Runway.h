#ifndef INC_RUNWAY_H
#define INC_RUNWAY_H

#include "../lib/DesignByContract.h"
#include "Airplane.h"
#include "Location.h"

#include <string>
#include <iostream>
#include <string.h>
#include <stdio.h>

namespace RunwayEnums {
    #define ERUNWAYTYPES \
                etype(InvalidType), \
                etype(Asphalt), \
                etype(Grass)

    #define etype(x) k##x
        enum EType { ERUNWAYTYPES };
    #undef etype

    const char* EnumToString(EType f);

    EType StringToTypeEnum(const char *f);

    /*enum EType {
        kInvalidType,
        kAsphalt,
        kGrass
    };*/
}

class Airport;

class Runway : public Location {
private:
    unsigned int length;
    RunwayEnums::EType type;

    const Airplane* airplane;
    const Airplane* crossingAirplane;

public:
    /**
     * POST: \n
     * ENSURE(properlyInitialized(), "Runway was not properly initialized.");
     */
    Runway(const std::string& _name, const unsigned int length, const RunwayEnums::EType type);

    /**
     * PRE: \n
     * REQUIRE(runway->properlyInitialized(), "Reference Runway was not properly initialized."); \n
     * POST: \n
     * ENSURE(properlyInitialized(), "Runway was not properly initialized.");
     */
    Runway(const Runway* const _runway);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
     */
    const unsigned int getLength() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
     */
    const RunwayEnums::EType getType() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
     */
    const Airplane* getAirplane() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
     */
    void setAirplane(const Airplane* _airplane);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
     */
    const Airplane* getCrossingAirplane() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
     */
    void setCrossingAirplane(const Airplane* _crossingAirplane);

    bool isVacant() const;

    bool canCross() const;
    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
     */
    void printInfo(std::ostream& stream) const;
};

#endif
