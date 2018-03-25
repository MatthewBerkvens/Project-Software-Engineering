#include "Airplane.h"

namespace AirplaneEnums {
    std::pair<std::string, EStatus> gStringToAirplaneStatusPairs[] =
            {
                    std::pair<std::string, EStatus>("Invalid Status", kInvalidStatus),
                    std::pair<std::string, EStatus>("Approaching", kApproaching),
                    std::pair<std::string, EStatus>("Standing at Gate", kStandingAtGate)
            };

    const std::map<std::string, EStatus>
            gStringToAirplaneStatus(
            &gStringToAirplaneStatusPairs[0],
            &gStringToAirplaneStatusPairs[sizeof(gStringToAirplaneStatusPairs) /
                                          sizeof(gStringToAirplaneStatusPairs[0])]
    );

    std::pair<EStatus, std::string> gAirplaneStatusToStringPairs[] =
            {
                    std::pair<EStatus, std::string>(kInvalidStatus, "Invalid Status"),
                    std::pair<EStatus, std::string>(kApproaching, "Approaching"),
                    std::pair<EStatus, std::string>(kStandingAtGate, "Standing at Gate")
            };

    const std::map<EStatus, std::string>
            gAirplaneStatusToString(
            &gAirplaneStatusToStringPairs[0],
            &gAirplaneStatusToStringPairs[sizeof(gAirplaneStatusToStringPairs) /
                                          sizeof(gAirplaneStatusToStringPairs[0])]
    );
}

bool Airplane::properlyInitialized() const {
    return this == init;
}

Airplane::Airplane(const std::string& _number,
                   const std::string& _callsign,
                   const std::string& _model,
                   AirplaneEnums::EStatus _status,
                   Airport* _airport,
                   Runway* _runway)
        : number(_number),
          callsign(_callsign),
          model(_model),
          status(_status),
          airport(_airport),
          runway(_runway),
          init(this) {
    ENSURE(properlyInitialized(), "Airplane was not properly initialized");
}

Airplane::Airplane(const Airplane* _airplane)
        : number(_airplane->getNumber()),
          callsign(_airplane->getCallsign()),
          model(_airplane->getModel()),
          status(_airplane->getStatus()),
          airport(_airplane->getAirport()),
          runway(_airplane->getRunway()),
          init(this) {
    REQUIRE(_airplane->properlyInitialized(), "References Airplane was not properly initialized");
    ENSURE(properlyInitialized(), "Airplane was not properly initialized");
}

const std::string& Airplane::getNumber() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return number;
}

const std::string& Airplane::getCallsign() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return callsign;
}

const std::string& Airplane::getModel() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return model;
}

AirplaneEnums::EStatus Airplane::getStatus() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return status;
}

Airport* Airplane::getAirport() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return airport;
}

Runway* Airplane::getRunway() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return runway;
}

void Airplane::setStatus(AirplaneEnums::EStatus _status) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    status = _status;
    ENSURE(status == _status, "Property 'status' was not correctly set in Airplane.");
}

void Airplane::setAirport(Airport* _airport) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    airport = _airport;
    runway = NULL;
    ENSURE(airport == _airport, "Property 'airport' was not correctly set.");
    ENSURE(runway == NULL, "Property 'runway' was not correctly reset.");
}

void Airplane::setRunway(Runway* _runway) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    REQUIRE(_runway->getAirport() != NULL, "Referenced Runway is not linked to any airport.");
    REQUIRE(_runway->getAirport() == getAirport(), "Referenced Runway is not linked to Airplane's airport.");
    runway = _runway;
    ENSURE(runway == _runway, "Property 'runway' was not correctly set.");
}

void Airplane::printInfo(std::ostream& stream) const
{
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    stream << "Airplane: " << getCallsign() << " (" << getNumber() << ") " << std::endl;
    stream << " -> model: " << getModel() << std::endl;
    stream << " -> status: " << AirplaneEnums::gAirplaneStatusToString.at(getStatus()) << std::endl;
    stream << std::endl;
}