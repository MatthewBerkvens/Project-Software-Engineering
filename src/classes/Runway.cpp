#include "Runway.h"

namespace RunwayEnums {
    #define etype(x) #x
        const char* strEType[] = { ERUNWAYTYPES };
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
}

Runway::Runway(const std::string& _name, const unsigned int _length, const RunwayEnums::EType _type) : Location(_name), length(_length), type(_type), airplane(NULL), crossingAirplane(NULL) {
    ENSURE(properlyInitialized(), "Runway was not properly initialized.");
}

Runway::Runway(const Runway* _runway) : Location(_runway), length(_runway->getLength()), type(_runway->getType()), airplane(_runway->getAirplane()), crossingAirplane(_runway->getCrossingAirplane()) {
    REQUIRE(_runway->properlyInitialized(), "Reference Runway was not properly initialized.");
    ENSURE(properlyInitialized(), "Runway was not properly initialized.");
}

const unsigned int Runway::getLength() const {
    REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
    return length;
}

const RunwayEnums::EType Runway::getType() const {
    REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
    return type;
}

Airplane* Runway::getAirplane() const {
    REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
    return airplane;
}

void Runway::setAirplane(Airplane* _airplane) {
    REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
    airplane = _airplane;
    ENSURE(airplane == _airplane, "Referenced Airplane was not properly set.");
}

Airplane* Runway::getCrossingAirplane() const {
    REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
    return crossingAirplane;
}

void Runway::setCrossingAirplane(Airplane* _crossingAirplane) {
    REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
    crossingAirplane = _crossingAirplane;
    ENSURE(crossingAirplane == _crossingAirplane, "Referenced Crossing Airplane was not properly set.");
}

bool Runway::isVacant() const {
    REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
    return crossingAirplane == NULL && airplane == NULL;
}

bool Runway::canCross() const {
    REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
    return crossingAirplane == NULL &&
            (airplane == NULL ||
            airplane->getStatus() == AirplaneEnums::kStatus_ReadyForTakeoff ||
            airplane->getStatus() == AirplaneEnums::kStatus_EmergencyEvacuation ||
            airplane->getStatus() == AirplaneEnums::kStatus_EmergencyCheckup ||
            airplane->getStatus() == AirplaneEnums::kStatus_EmergencyRefueling);
}

void Runway::printInfo(std::ostream& stream) const
{
    REQUIRE(properlyInitialized(), "Runway was not properly initialized.");
    stream << "Runway " << getName() << ": " << getLength() << "m, " << (getType() == RunwayEnums::kGrass ? "Grass" : "Asphalt") << std::endl;
    stream << std::endl;
}