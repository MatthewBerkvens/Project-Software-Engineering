#include "AirTrafficController.h"

bool AirTrafficController::properlyInitialized() const {
    return this == init;
}

AirTrafficController::AirTrafficController(std::ostream& _outputStream) : outputStream(_outputStream), init(this) {}

void AirTrafficController::RequestLanding(Airplane* airplane) {
}