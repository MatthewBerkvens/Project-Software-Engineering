#ifndef INC_AIRTRAFFICCONTROLLER_H
#define INC_AIRTRAFFICCONTROLLER_H

#include "Airplane.h"

#include <iostream>
#include <string>

class AirTrafficController {
private:
    std::ostream& outputStream;

    AirTrafficController* init;

    bool properlyInitialized() const;
public:
    AirTrafficController(std::ostream& _outputStream);

    void RequestLanding(Airplane* airplane);
};

#endif
