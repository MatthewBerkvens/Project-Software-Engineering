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
        return kStatus_InvalidStatus;
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
        return kType_InvalidType;
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
        return kSize_InvalidSize;
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
        return kEngine_InvalidEngine;
    }
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
          permission(false),
          actionTimer(0),
          communicationTimer(0),
          airport(NULL),
          gate(-1),
          runway(NULL),
          currentLocation(NULL),
          taxiRoute(std::queue<Location*>()),
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
          permission(_airplane->hasPermission()),
          actionTimer(_airplane->getActionTimer()),
          communicationTimer(_airplane->getCommunicationTimer()),
          airport(_airplane->getAirport()),
          gate(_airplane->getGate()),
          runway(_airplane->getRunway()),
          currentLocation(_airplane->getCurrentLocation()),
          taxiRoute(std::queue<Location*>()),
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

void Airplane::setSquawk(const unsigned int p_squawk) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    REQUIRE((p_squawk >= 001 && p_squawk <= 06777) || p_squawk == 07500 || p_squawk == 07600 || p_squawk == 07700, "Property 'squawk' has to lie between 0001-6777 (7500, 7600, 7700 included) in base 8.");
    squawk = p_squawk;
    ENSURE(squawk == p_squawk, "Property 'squawk' was not correctly set in Airplane.");
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
    if (size == AirplaneEnums::kSize_Small) {
        if (engine == AirplaneEnums::kEngine_Propeller) { return 10; }
        else if (engine == AirplaneEnums::kEngine_Jet) { return 25; }
        else { return 0; }
    } else if (size == AirplaneEnums::kSize_Medium) {
        if (engine == AirplaneEnums::kEngine_Propeller) { return 50; }
        else if (engine == AirplaneEnums::kEngine_Jet) { return 175; }
        else { return 0; }
    } else if (size == AirplaneEnums::kSize_Large) {
        if (engine == AirplaneEnums::kEngine_Propeller) { return 100; }
        else if (engine == AirplaneEnums::kEngine_Jet) { return 250; }
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

bool Airplane::hasPermission() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return permission;
}

void Airplane::setPermission(const bool t_permission) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    permission = t_permission;
    ENSURE(permission == t_permission, "Property 'permission' was not correctly set in Airplane.");
}

unsigned int Airplane::getActionTimer() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return actionTimer;
}

void Airplane::setActionTimer(const unsigned int t_actionTimer) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    actionTimer = t_actionTimer;
    ENSURE(actionTimer == t_actionTimer, "Property 'actionTimer' was not correctly set in Airplane.");
}

void Airplane::increaseActionTimer(const unsigned int t_addition) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    actionTimer += t_addition;
};

unsigned int Airplane::getCommunicationTimer() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return communicationTimer;
}

void Airplane::setCommunicationTimer(const unsigned int t_communicationTimer) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    communicationTimer = t_communicationTimer;
    ENSURE(communicationTimer == t_communicationTimer, "Property 'communicationTimer' was not correctly set in Airplane.");
}

void Airplane::increaseCommunicationTimer(const unsigned int addition) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    communicationTimer += addition;
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

std::queue<Location*>& Airplane::getTaxiRoute() {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return taxiRoute;
}

std::queue<Location*> Airplane::getTaxiRouteCopy() const {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    return taxiRoute;
}

void Airplane::setTaxiRoute(const std::queue<Location*>& p_taxiRoute) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    taxiRoute = p_taxiRoute;
    ENSURE(taxiRoute == p_taxiRoute, "Property 'taxiRoute' was not correctly set.");
}

bool Airplane::isFlying() const {
    return status == AirplaneEnums::kStatus_Approaching ||
        status == AirplaneEnums::kStatus_DescendingTo5000ft ||
        status == AirplaneEnums::kStatus_DescendingTo3000ft ||
        status == AirplaneEnums::kStatus_FlyingWaitPattern ||
        status == AirplaneEnums::kStatus_FinalApproach ||
        status == AirplaneEnums::kStatus_Landing ||
        status == AirplaneEnums::kStatus_LeftAirport ||
        status == AirplaneEnums::kStatus_TakingOff ||
        status == AirplaneEnums::kStatus_EmergencyLanding ||
        status == AirplaneEnums::kStatus_EmergencyFinalApproach;
}

bool Airplane::fly() {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    REQUIRE(isFlying(), "Airplane is currently not in the air");
    if (getFuel() > getFuelConsumption()) {
        setFuel(getFuel() - getFuelConsumption());
        return true;
    } else {
        setSquawk(07700);
        setFuel(0);
        altitude -= 500;
        return false;
    }
}

bool Airplane::descend(const unsigned int t_subtraction) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");

    bool result = true;


    if (altitude != 0) {
        if (altitude > t_subtraction) {
            altitude -= t_subtraction;
        } else {
            altitude = 0;
        }
    } else {
        result = false;
    }

    return result;
}

void Airplane::ascend(const unsigned int t_addition) {
    REQUIRE(properlyInitialized(), "Airplane was not properly initialized.");
    altitude += t_addition;
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