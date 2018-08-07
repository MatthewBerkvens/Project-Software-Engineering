#include "Airplane.h"

namespace AirplaneEnums {
    #define estatus(x) #x
        const char* strEStatus[] = { ESTATUS };
    #undef estatus

    const char* EnumToString(EStatus f)
    {
        return strEStatus[static_cast<int>(f)];
    }

    EStatus StringToStatusEnum(const char *f)
    {
        const int n = sizeof(strEStatus) / sizeof(strEStatus[0]);
        for (int i = 0; i < n; ++i)
        {
            if (strcasecmp(strEStatus[i], f) == 0) return (EStatus)i;
        }
        return kInvalidStatus;
    }



    #define etype(x) #x
        const char* strEType[] = { EAIRPLANETYPES };
    #undef etype

    const char* EnumToString(EType f)
    {
        return strEType[static_cast<int>(f)];
    }

    EType StringToTypeEnum(const char *f)
    {
        const int n = sizeof(strEType) / sizeof(strEType[0]);
        for (int i = 0; i < n; ++i)
        {
            if (strcasecmp(strEType[i], f) == 0) return (EType)i;
        }
        return kInvalidType;
    }



    #define esize(x) #x
        const char* strESize[] = { ESIZES };
    #undef esize

    const char* EnumToString(ESize f)
    {
        return strESize[static_cast<int>(f)];
    }

    ESize StringToSizeEnum(const char *f)
    {
        const int n = sizeof(strESize) / sizeof(strESize[0]);
        for (int i = 0; i < n; ++i)
        {
            if (strcasecmp(strESize[i], f) == 0) return (ESize)i;
        }
        return kInvalidSize;
    }



    #define eengine(x) #x
        const char* strEEngine[] = { EENGINES };
    #undef eengine

    const char* EnumToString(EEngine f)
    {
        return strEEngine[static_cast<int>(f)];
    }

    EEngine StringToEngineEnum(const char *f)
    {
        const int n = sizeof(strEEngine) / sizeof(strEEngine[0]);
        for (int i = 0; i < n; ++i)
        {
            if (strcasecmp(strEEngine[i], f) == 0) return (EEngine)i;
        }
        return kInvalidEngine;
    }

    /*std::pair<std::string, EStatus> gStringToAirplaneStatusPairs[] =
            {
                    std::pair<std::string, EStatus>("Approaching", kApproaching),
                    std::pair<std::string, EStatus>("Standing at Gate", kStandingAtGate)
            };

    std::pair<EStatus, std::string> gAirplaneStatusToStringPairs[] =
            {
                    std::pair<EStatus, std::string>(kApproaching, "Approaching"),
                    std::pair<EStatus, std::string>(kStandingAtGate, "Standing at gate"),
                    std::pair<EStatus, std::string>(kTaxiingToGate, "Taxiing to gate"),
                    std::pair<EStatus, std::string>(kTaxiingToRunway, "Taxiing to runway"),
                    std::pair<EStatus, std::string>(kReadyForTakeoff, "Ready for takeoff"),
                    std::pair<EStatus, std::string>(kFinalApproach, "Final approach"),
                    std::pair<EStatus, std::string>(kWaitingForEmptyGate, "Waiting for empty gate"),
            };

    const std::map<std::string, EStatus> gStringToAirplaneStatus(
            &gStringToAirplaneStatusPairs[0],
            &gStringToAirplaneStatusPairs[sizeof(gStringToAirplaneStatusPairs) / sizeof(gStringToAirplaneStatusPairs[0])]
    );

    const std::map<EStatus, std::string> gAirplaneStatusToString(
            &gAirplaneStatusToStringPairs[0],
            &gAirplaneStatusToStringPairs[sizeof(gAirplaneStatusToStringPairs) / sizeof(gAirplaneStatusToStringPairs[0])]
    );*/
}

bool Airplane::properlyInitialized() const {
    return this == init;
}

Airplane::Airplane(const std::string& _number,
                   const std::string& _callsign,
                   const std::string& _model,
                   const unsigned int _squawk,
                   const unsigned int _altitude,
                   const unsigned int _fuel,
                   const unsigned int _fuelCapacity,
                   const unsigned int _passengers,
                   const unsigned int _passengerCapacity,
                   const AirplaneEnums::EStatus _status,
                   const AirplaneEnums::EType _type,
                   const AirplaneEnums::ESize _size,
                   const AirplaneEnums::EEngine _engine)
        : number(_number),
          callsign(_callsign),
          model(_model),
          squawk(_squawk),
          altitude(_altitude),
          fuel(_fuel),
          fuelCapacity(_fuelCapacity),
          passengers(_passengers),
          passengerCapacity(_passengerCapacity),
          status(_status),
          type(_type),
          size(_size),
          engine(_engine),
          waitingForInstructions(false),
          internalTimer(0),
          airport(NULL),
          gate(-1),
          runway(NULL),
          currentLocation(NULL),
          takeoffRunway(NULL),
          init(this) {
    ENSURE(properlyInitialized(), "Airplane was not properly initialized");
}

Airplane::Airplane(const Airplane* _airplane)
        : number(_airplane->getNumber()),
          callsign(_airplane->getCallsign()),
          model(_airplane->getModel()),
          squawk(_airplane->getSquawk()),
          altitude(_airplane->getAltitude()),
          fuel(_airplane->getFuel()),
          fuelCapacity(_airplane->getFuelCapacity()),
          passengers(_airplane->getPassengers()),
          passengerCapacity(_airplane->getPassengerCapacity()),
          status(_airplane->getStatus()),
          type(_airplane->getType()),
          size(_airplane->getSize()),
          engine(_airplane->getEngine()),
          waitingForInstructions(_airplane->getWaitingForInstructions()),
          internalTimer(_airplane->getInternalTimer()),
          airport(_airplane->getAirport()),
          gate(_airplane->getGate()),
          runway(_airplane->getRunway()),
          currentLocation(_airplane->getCurrentLocation()),
          takeoffRunway(_airplane->getTakeoffRunway()),
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

unsigned int Airplane::getSquawk() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return squawk;
}

void Airplane::setSquawk(const unsigned int _squawk) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    squawk = _squawk;
    ENSURE(squawk == _squawk, "Property 'squawk' was not correctly set in Airplane.");
}

AirplaneEnums::EStatus Airplane::getStatus() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return status;
}

void Airplane::setStatus(const AirplaneEnums::EStatus _status) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    status = _status;
    ENSURE(status == _status, "Property 'status' was not correctly set in Airplane.");
}

AirplaneEnums::EType Airplane::getType() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return type;
}

AirplaneEnums::ESize Airplane::getSize() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return size;
}

AirplaneEnums::EEngine Airplane::getEngine() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return engine;
}

unsigned int Airplane::getAltitude() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return altitude;
}

void Airplane::setAltitude(const unsigned int _altitude) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    altitude = _altitude;
    ENSURE(altitude == _altitude, "Property 'altitude' was not correctly set in Airplane.");
}

unsigned int Airplane::getFuel() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return fuel;
}

void Airplane::setFuel(const unsigned int _fuel) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    fuel = _fuel;
    ENSURE(fuel == _fuel, "Property 'fuel' was not correctly set in Airplane.");
}

unsigned int Airplane::getFuelCapacity() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return fuelCapacity;
}

unsigned int Airplane::getFuelConsumption() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    if (size == AirplaneEnums::kSmall) {
        if (engine == AirplaneEnums::kPropeller) { return 10; }
        else if (engine == AirplaneEnums::kJet) { return 25; }
        else { return 0; }
    } else if (size == AirplaneEnums::kMedium) {
        if (engine == AirplaneEnums::kPropeller) { return 50; }
        else if (engine == AirplaneEnums::kJet) { return 175; }
        else { return 0; }
    } else if (size == AirplaneEnums::kLarge) {
        if (engine == AirplaneEnums::kPropeller) { return 100; }
        else if (engine == AirplaneEnums::kJet) { return 250; }
        else { return 0; }
    } else {
        return 0;
    }
}

unsigned int Airplane::getPassengers() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return passengers;
}

void Airplane::setPassengers(const unsigned int _passengers) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    passengers = _passengers;
    ENSURE(passengers == _passengers, "Property 'passengers' was not correctly set in Airplane.");
}

unsigned int Airplane::getPassengerCapacity() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return passengerCapacity;
}

bool Airplane::getWaitingForInstructions() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return waitingForInstructions;
}

void Airplane::setWaitingForInstructions(const bool _waitingForInstructions) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    waitingForInstructions = _waitingForInstructions;
    ENSURE(waitingForInstructions == _waitingForInstructions, "Property 'waitingForInstructions' was not correctly set in Airplane.");
}

unsigned int Airplane::getInternalTimer() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return internalTimer;
}

void Airplane::setInternalTimer(const unsigned int _internalTimer) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    internalTimer = _internalTimer;
    ENSURE(internalTimer == _internalTimer, "Property 'internalTimer' was not correctly set in Airplane.");
}

void Airplane::increaseInternalTimer(const unsigned int _addition) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    internalTimer += _addition;
};

Airport* Airplane::getAirport() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return airport;
}

void Airplane::setAirport(Airport* _airport) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    airport = _airport;
    runway = NULL;
    ENSURE(airport == _airport, "Property 'airport' was not correctly set.");
    ENSURE(runway == NULL, "Property 'runway' was not correctly reset.");
}

const long Airplane::getGate() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return gate;
}

void Airplane::setGate(unsigned const long _gate) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    gate = _gate;
    ENSURE(gate == _gate, "Property 'status' was not correctly set in Airplane.");
}

Runway* Airplane::getRunway() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return runway;
}

void Airplane::setRunway(Runway* _runway) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    runway = _runway;
    ENSURE(runway == _runway, "Property 'runway' was not correctly set.");
}

Location* Airplane::getCurrentLocation() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return currentLocation;
}

void Airplane::setCurrentLocation(Location* _currentLocation) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    currentLocation = _currentLocation;
    ENSURE(currentLocation == _currentLocation, "Property 'currentLocation' was not correctly set.");
}

Runway* Airplane::getTakeoffRunway() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return takeoffRunway;
}

void Airplane::setTakeoffRunway(Runway* _takeoffRunway) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    takeoffRunway = _takeoffRunway;
    ENSURE(takeoffRunway == _takeoffRunway, "Property 'currentLocation' was not correctly set.");
}

void Airplane::fly() {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    if (getFuel() > getFuelConsumption()) {
        setFuel(getFuel() - getFuelConsumption());
    } else {
        setSquawk(7700);
        setFuel(0);
        altitude -= 500;
    }
}

void Airplane::printInfo(std::ostream& stream) const
{
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    stream << "Airplane: " << getCallsign() << " (" << getNumber() << ") " << std::endl;
    stream << " -> model: " << getModel() << std::endl;
    stream << " -> squawk: " << std::oct << getSquawk() << std::dec << std::endl;
    stream << " -> altitude: " << getAltitude() << std::endl;
    stream << " -> fuel: " << getFuel() << "/" << getFuelCapacity() << " consumption: " << getFuelConsumption() << std::endl;
    stream << " -> passengers: " << getPassengers() << "/" << getPassengerCapacity() << std::endl;
    stream << " -> status: " << AirplaneEnums::EnumToString(status) << std::endl;
    stream << " -> type: " << AirplaneEnums::EnumToString(type) << std::endl;
    stream << " -> size: " << AirplaneEnums::EnumToString(size) << std::endl;
    stream << " -> engine: " << AirplaneEnums::EnumToString(engine) << std::endl;
    stream << std::endl;
}