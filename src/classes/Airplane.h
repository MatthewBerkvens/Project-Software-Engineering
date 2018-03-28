#ifndef INC_AIRPLANE_H
#define INC_AIRPLANE_H

#include "../lib/DesignByContract.h"
#include "Airport.h"
#include "Runway.h"

#include <string>
#include <map>

namespace AirplaneEnums {
    enum EStatus {
        kInvalidStatus,
        kApproaching,
        kStandingAtGate
    };

    extern const std::map<std::string, EStatus> gStringToAirplaneStatus;

    extern const std::map<EStatus, std::string> gAirplaneStatusToString;
}


class Airplane {
private:
    const std::string number;
    const std::string callsign;
    const std::string model;
    AirplaneEnums::EStatus status;

    Airport* airport;
    Runway* runway;

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
             AirplaneEnums::EStatus _status = AirplaneEnums::kInvalidStatus);

    /**
     * PRE: \n
     * REQUIRE(runway->properlyInitialized(), "Reference Airplane was not properly initialized."); \n
     * POST: \n
     * ENSURE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    Airplane(const Airplane* _airplane);

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
    AirplaneEnums::EStatus getStatus() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    Airport* getAirport() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    Runway* getRunway() const;

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized."); \n
     * POST: \n
     * ENSURE(status == _status, "Property 'status' was not correctly set in Airplane.");
     */
    void setStatus(AirplaneEnums::EStatus _status);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized."); \n
     * POST: \n
     * ENSURE(airport == _airport, "Property 'airport' was not correctly set in Airplane.");
     */
    void setAirport(Airport* _airport);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized."); \n
     * REQUIRE(_runway != NULL, "Referenced Runway is a NULLPTR."); \n
     * REQUIRE(runway->getAirport() != NULL, "Referenced Runway is not linked to any airport."); \n
     * REQUIRE(runway->getAirport() == getAirport(), "Referenced Runway is not linked to Airplane's airport."); \n
     * POST: \n
     * ENSURE(runway == _runway, "Property 'runway' was not correctly set in Airplane.");
     */
    void setRunway(Runway* _runway);

    /**
     * PRE: \n
     * REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
     */
    void printInfo(std::ostream& stream) const;
};

#endif
