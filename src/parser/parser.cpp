#include "parser.h"

Parser::Parser() {};

ParseEnum::EResult Parser::parseFile(const char* fileName, std::ostream& errorStream) {
    TiXmlDocument xmlFile;

    if (!xmlFile.LoadFile(fileName)) {
        errorStream << "Error loading file: " << xmlFile.ErrorDesc() << std::endl;
        xmlFile.Clear();
        return ParseEnum::kAborted;
    }

    if (xmlFile.FirstChildElement() == NULL) {
        errorStream << "Error loading file: root missing" << std::endl;
        xmlFile.Clear();
        return ParseEnum::kPartial;
    }

    ParseEnum::EResult result = ParseEnum::kSuccess;
    std::vector<Airport*> airports;
    std::vector<Runway*> runways;
    std::vector<Airplane*> airplanes;

    for (TiXmlElement* object = xmlFile.FirstChildElement(); object != NULL; object = object->NextSiblingElement()) {
        std::string objectName = object->Value();
        if (objectName == "AIRPORT") {
            std::string airportName;
            std::string iata;
            std::string callsign;
            unsigned int gates = 0;

            for (TiXmlElement* elem = object->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
                std::string elemName = elem->Value();
                std::string elemValue = "";

                TiXmlNode* node = elem->FirstChild();
                if (node != NULL) {
                    TiXmlText* textNode = node->ToText();
                    if (textNode != NULL) {
                        std::string textNodeValue = textNode->Value();
                        if (isString(textNodeValue)) {
                            elemValue = textNodeValue;
                        } else {
                            errorStream << object->Value() << ": '" << elemName << "' attribute is not a valid string (only the alphabet, digits and spaces are allowed). " << getRowAndColumnStr(elem)
                                        << std::endl;
                        }
                    } else {
                        errorStream << object->Value() << ": '" << elemName << "' attribute does not contain text. " << getRowAndColumnStr(elem) << std::endl;
                    }
                } else {
                    errorStream << object->Value() << ": '" << elemName << "' attribute does not contain anything. " << getRowAndColumnStr(elem) << std::endl;
                }

                if (elemName == "name") {
                    airportName = elemValue;
                } else if (elemName == "iata") {
                    iata = elemValue;
                } else if (elemName == "callsign") {
                    callsign = elemValue;
                } else if (elemName == "gates") {
                    unsigned int nodeTextAsInt;
                    if (tryCastStringToUnsignedInt(elemValue, &nodeTextAsInt)) {
                        if (nodeTextAsInt > 0) {
                            gates = nodeTextAsInt;
                        } else {
                            errorStream << objectName << ": '" << elemName << "' attribute can't be 0. " << getRowAndColumnStr(elem) << std::endl;
                        }
                    } else {
                        errorStream << objectName << ": '" << elemName << "' attribute is not an unsigned integer. " << getRowAndColumnStr(elem) << std::endl;
                        result = ParseEnum::kPartial;
                    }
                } else {
                    errorStream << objectName << ": '" << elemName << "' invalid attribute. " << getRowAndColumnStr(elem) << std::endl;
                    result = ParseEnum::kPartial;
                }
            }


            if (airportName == "") {
                errorStream << objectName << ": 'airportName' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
                result = ParseEnum::kPartial;
            }
            if (iata == "") {
                errorStream << objectName << ": 'iata' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
                result = ParseEnum::kPartial;
            }
            if (callsign == "") {
                errorStream << objectName << ": 'callsign' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
                result = ParseEnum::kPartial;
            }
            if (gates == 0) {
                errorStream << objectName << ": 'gates' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
                result = ParseEnum::kPartial;
            }

            if (airportName == "" || iata == "" || callsign == "" || gates == 0) {
                continue;
            }

            airports.push_back(new Airport(airportName, iata, callsign, gates));
        } else if (objectName == "RUNWAY") {
            std::string runwayName;
            Airport* linkedAirport = NULL;

            for (TiXmlElement* elem = object->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
                std::string elemName = elem->Value();
                std::string elemValue = "";

                TiXmlNode* node = elem->FirstChild();
                if (node != NULL) {
                    TiXmlText* textNode = node->ToText();
                    if (textNode != NULL) {
                        std::string textNodeValue = textNode->Value();
                        if (isString(textNodeValue)) {
                            elemValue = textNodeValue;
                        } else {
                            errorStream << object->Value() << ": '" << elemName << "' attribute is not a valid string (only the alphabet, digits and spaces are allowed). " << getRowAndColumnStr(elem)
                                        << std::endl;
                        }
                    } else {
                        errorStream << object->Value() << ": '" << elemName << "' attribute does not contain text. " << getRowAndColumnStr(elem) << std::endl;
                    }
                } else {
                    errorStream << object->Value() << ": '" << elemName << "' attribute does not contain anything. " << getRowAndColumnStr(elem) << std::endl;
                }

                if (elemName == "name") {
                    runwayName = elemValue;
                } else if (elemName == "airport") {
                    AirportVectorIterator it_airport = airports.end();

                    for (AirportVectorIterator it = airports.begin(); it != airports.end(); it++) {
                        if ((*it)->getIata() == elemValue) {
                            it_airport = it;
                            break;
                        }
                    }

                    if (it_airport != airports.end()) {
                        linkedAirport = *it_airport;
                    } else {
                        errorStream << objectName << ": referenced airport does not exist, make sure to declare runways after the referenced airport. " << getRowAndColumnStr(elem) << std::endl;
                    }
                } else {
                    errorStream << objectName << ": '" << elemName << "' invalid attribute. " << getRowAndColumnStr(elem) << std::endl;
                    result = ParseEnum::kPartial;
                }
            }


            if (runwayName == "") {
                errorStream << objectName << ": 'runwayName' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
                result = ParseEnum::kPartial;
            }
            if (linkedAirport == NULL) {
                errorStream << objectName << ": 'airport' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
                result = ParseEnum::kPartial;
            }

            if (runwayName == "" || linkedAirport == NULL) {
                continue;
            }

            Runway* newRunway = new Runway(runwayName);
            newRunway->setAirport(linkedAirport);
            linkedAirport->addRunway(newRunway);
            runways.push_back(newRunway);
        } else if (objectName == "AIRPLANE") {
            std::string number;
            std::string callsign;
            std::string model;
            AirplaneEnums::EStatus status = AirplaneEnums::kInvalidStatus;

            Airport* linkedAirport = NULL;
            Runway* linkedRunway = NULL;

            for (TiXmlElement* elem = object->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
                std::string elemName = elem->Value();
                std::string elemValue = "";

                TiXmlNode* node = elem->FirstChild();
                if (node != NULL) {
                    TiXmlText* textNode = node->ToText();
                    if (textNode != NULL) {
                        std::string textNodeValue = textNode->Value();
                        if (isString(textNodeValue)) {
                            elemValue = textNodeValue;
                        } else {
                            errorStream << object->Value() << ": '" << elemName << "' attribute is not a valid string (only the alphabet, digits and spaces are allowed). " << getRowAndColumnStr(elem)
                                        << std::endl;
                        }
                    } else {
                        errorStream << object->Value() << ": '" << elemName << "' attribute does not contain text. " << getRowAndColumnStr(elem) << std::endl;
                    }
                } else {
                    errorStream << object->Value() << ": '" << elemName << "' attribute does not contain anything. " << getRowAndColumnStr(elem) << std::endl;
                }

                if (elemName == "number") {
                    number = elemValue;
                } else if (elemName == "callsign") {
                    callsign = elemValue;
                } else if (elemName == "model") {
                    model = elemValue;
                } else if (elemName == "status") {
                    if (AirplaneEnums::gStringToAirplaneStatus.find(elemValue) != AirplaneEnums::gStringToAirplaneStatus.end()) {
                        status = AirplaneEnums::gStringToAirplaneStatus.at(elemValue);
                    } else {
                        errorStream << objectName << ": '" << elemName << "' attribute is not a valid AirplaneStatus. " << getRowAndColumnStr(object->FirstChildElement(elemName.c_str())) << std::endl;
                    }
                /*} else if (elemName == "airport") {
                    AirportVectorIterator it_airport = airports.end();

                    for (AirportVectorIterator it = airports.begin(); it != airports.end(); it++) {
                        if ((*it)->getIata() == elemValue) {
                            it_airport = it;
                            break;
                        }
                    }

                    if (it_airport != airports.end()) {
                        linkedAirport = *it_airport;
                    } else {
                        errorStream << objectName << ": '" << elemValue << "' referenced airport does not exist, make sure to declare airplanes after the referenced airport and runway. "
                                    << getRowAndColumnStr(object) << std::endl;
                    }
                } else if (elemName == "runway") {
                    RunwayVectorIterator it_runway = runways.end();

                    for (RunwayVectorIterator it = runways.begin(); it != runways.end(); it++) {
                        if ((*it)->getRunwayName() == elemValue) {
                            it_runway = it;
                            break;
                        }
                    }

                    if (it_runway != runways.end()) {
                        linkedRunway = *it_runway;
                    } else {
                        errorStream << objectName << ": '" << elemValue << "'referenced runway does not exist, make sure to declare airplanes after the referenced airport and runway. "
                                    << getRowAndColumnStr(object) << std::endl;
                    }*/
                } else {
                    errorStream << objectName << ": '" << elemName << "' invalid attribute. " << getRowAndColumnStr(elem) << std::endl;
                    result = ParseEnum::kPartial;
                }
            }


            if (number.empty()) {
                errorStream << objectName << ": 'number' required attribute missing. " << getRowAndColumnStr(object) << std::endl;
                result = ParseEnum::kPartial;
            }
            if (callsign.empty()) {
                errorStream << objectName << ": 'callsign' required attribute missing. " << getRowAndColumnStr(object) << std::endl;
                result = ParseEnum::kPartial;
            }
            if (model.empty()) {
                errorStream << objectName << ": 'model' required attribute missing. " << getRowAndColumnStr(object) << std::endl;
                result = ParseEnum::kPartial;
            }
            if (status == AirplaneEnums::kInvalidStatus) {
                errorStream << objectName << ": 'status' required attribute missing. " << getRowAndColumnStr(object) << std::endl;
                result = ParseEnum::kPartial;
            }

            if (number.empty() || callsign.empty() || model.empty() || status == AirplaneEnums::kInvalidStatus) {
                continue;
            }

            Airplane* newAirplane = new Airplane(number, callsign, model, status);

            if (linkedAirport != NULL) {
                newAirplane->setAirport(linkedAirport);

                //TODO: check for correct status if airport is OR is not available
            }

            if (linkedRunway != NULL) {
                newAirplane->setRunway(linkedRunway);

                //TODO: check for correct status if linkedRunway is OR is not available
            }

            airplanes.push_back(newAirplane);
        } else {
            errorStream << "Invalid object '" << objectName << "'. " << getRowAndColumnStr(object) << std::endl;
            result = ParseEnum::kPartial;
        }
    }
    xmlFile.Clear();

    return result;
}