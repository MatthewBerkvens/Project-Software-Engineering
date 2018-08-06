#include "parser.h"

Parser::Parser(std::ostream& _errorStream) : errorStream(_errorStream) {};

std::pair<ParseEnum::EResult, Airport*> Parser::parseFile(const char* fileName) {
    TiXmlDocument xmlFile;

    if (!xmlFile.LoadFile(fileName)) {
        errorStream << "Error loading file: " << xmlFile.ErrorDesc() << std::endl;
        xmlFile.Clear();
        return std::pair<ParseEnum::EResult, Airport*>(ParseEnum::kAborted, NULL);
    }

    if (xmlFile.FirstChildElement() == NULL) {
        errorStream << "Error loading file: root missing" << std::endl;
        xmlFile.Clear();
        return std::pair<ParseEnum::EResult, Airport*>(ParseEnum::kPartial, NULL);
    }

    ParseEnum::EResult parseResult = ParseEnum::kSuccess;
    Airport* airport;
    std::vector<Runway*> runways;
    std::vector<Airplane*> airplanes;

    for (TiXmlElement* object = xmlFile.FirstChildElement(); object != NULL; object = object->NextSiblingElement()) {
        std::string objectName = object->Value();
        if (objectName == "AIRPORT") {
            std::string airportName;
            std::string iata;
            std::string callsign;
            unsigned int gates = 0;

            bool invalidAirport = false;

            std::map<std::string, std::string> xmltomap = convertXmlNodeToMap(object);

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
                            invalidAirport = true;
                            continue;
                        }
                    } else {
                        errorStream << object->Value() << ": '" << elemName << "' attribute does not contain text. " << getRowAndColumnStr(elem) << std::endl;
                        invalidAirport = true;
                        continue;
                    }
                } else {
                    errorStream << object->Value() << ": '" << elemName << "' attribute does not contain anything. " << getRowAndColumnStr(elem) << std::endl;
                    invalidAirport = true;
                    continue;
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
                            errorStream << objectName << ": '" << elemName << "' attribute must be a strictly positive integer. " << getRowAndColumnStr(elem) << std::endl;
                            invalidAirport = true;
                        }
                    } else {
                        errorStream << objectName << ": '" << elemName << "' attribute is not an unsigned integer. " << getRowAndColumnStr(elem) << std::endl;
                        invalidAirport = true;
                    }
                } else {
                    errorStream << objectName << ": '" << elemName << "' invalid attribute. " << getRowAndColumnStr(elem) << std::endl;
                    invalidAirport = true;
                }
            }


            if (airportName.empty()) {
                errorStream << objectName << ": 'airportName' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
                invalidAirport = true;
            }
            if (iata.empty()) {
                errorStream << objectName << ": 'iata' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
                invalidAirport = true;
            }
            if (callsign.empty()) {
                errorStream << objectName << ": 'callsign' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
                invalidAirport = true;
            }
            if (gates == 0) {
                errorStream << objectName << ": 'gates' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
                invalidAirport = true;
            }

            if (invalidAirport) {
                parseResult = ParseEnum::kPartial;
            } else {
                airport = new Airport(airportName, iata, callsign, gates);
            }
        } else if (objectName == "RUNWAY") {
            std::string runwayName = "";
            unsigned int runwayLength = 0;
            RunwayEnums::EType runwayType = RunwayEnums::kInvalidType;
            Airport* linkedAirport = NULL;

            bool invalidRunway = false;

            std::map<std::string, std::string> xmltomap = convertXmlNodeToMap(object);

            std::map<int, std::pair<bool, std::string> > taxiRouteToMap = extractTaxiRoute(object->FirstChildElement("TAXIROUTE"));

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
                            invalidRunway = true;
                            continue;
                        }
                    } else {
                        errorStream << object->Value() << ": '" << elemName << "' attribute does not contain text. " << getRowAndColumnStr(elem) << std::endl;
                        invalidRunway = true;
                        continue;
                    }
                } else {
                    errorStream << object->Value() << ": '" << elemName << "' attribute does not contain anything. " << getRowAndColumnStr(elem) << std::endl;
                    invalidRunway = true;
                    continue;
                }

                if (elemName == "name") {
                    runwayName = elemValue;
                } else if (elemName == "airport") {
                    if (airport != NULL && airport->getIata() == elemValue) {
                        linkedAirport = airport;
                    } else {
                        errorStream << objectName << ": referenced airport does not exist, make sure to declare runways after the referenced airport. " << getRowAndColumnStr(elem) << std::endl;
                        invalidRunway = true;
                    }
                } else if (elemName == "length") {
                    unsigned int nodeTextAsInt;
                    if (tryCastStringToUnsignedInt(elemValue, &nodeTextAsInt)) {
                        if (nodeTextAsInt > 0) {
                            runwayLength = nodeTextAsInt;
                        } else {
                            errorStream << objectName << ": '" << elemName << "' attribute must be a strictly positive integer. " << getRowAndColumnStr(elem) << std::endl;
                            invalidRunway = true;
                        }
                    }
                } else if (elemName == "type") {
                    RunwayEnums::EType temp = RunwayEnums::StringToTypeEnum(elemValue.c_str());
                    if (temp != RunwayEnums::kInvalidType) {
                        runwayType = temp;
                    } else {
                        errorStream << objectName << ": '" << elemName << "' attribute does not contain a valid option. " << getRowAndColumnStr(elem) << std::endl;
                        invalidRunway = true;
                    }
                } else {
                    errorStream << objectName << ": '" << elemName << "' invalid attribute. " << getRowAndColumnStr(elem) << std::endl;
                    invalidRunway = true;
                }
            }


            if (runwayName.empty()) {
                errorStream << objectName << ": 'name' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
                invalidRunway = true;
            }
            if (runwayLength == 0) {
                errorStream << objectName << ": 'length' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
                invalidRunway = true;
            }
            if (runwayType == RunwayEnums::kInvalidType) {
                errorStream << objectName << ": 'type' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
                invalidRunway = true;
            }
            if (linkedAirport == NULL) {
                errorStream << objectName << ": 'airport' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
                invalidRunway = true;
            }

            if (invalidRunway) {
                parseResult = ParseEnum::kPartial;
            }
            else {
                Runway* newRunway = new Runway(runwayName, runwayLength, runwayType);
                newRunway->setAirport(linkedAirport);
                linkedAirport->addRunway(newRunway);
                runways.push_back(newRunway);
            }
        } else if (objectName == "AIRPLANE") {
            std::string number = "";
            std::string callsign = "";
            std::string model = "";

            unsigned int fuelCapacity = 0;
            unsigned int passengerCapacity = 0;

            AirplaneEnums::EStatus status = AirplaneEnums::kInvalidStatus;
            AirplaneEnums::EType type = AirplaneEnums::kInvalidType;
            AirplaneEnums::ESize size = AirplaneEnums::kInvalidSize;
            AirplaneEnums::EEngine engine = AirplaneEnums::kInvalidEngine;

            bool invalidAirplane = false;

            std::map<std::string, std::string> xmltomap = convertXmlNodeToMap(object);

            for (TiXmlElement* elem = object->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
                std::string elemName = elem->Value();
                std::string elemValue;

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
                            invalidAirplane = true;
                            continue;
                        }
                    } else {
                        errorStream << object->Value() << ": '" << elemName << "' attribute does not contain text. " << getRowAndColumnStr(elem) << std::endl;
                        invalidAirplane = true;
                        continue;
                    }
                } else {
                    errorStream << object->Value() << ": '" << elemName << "' attribute does not contain anything. " << getRowAndColumnStr(elem) << std::endl;
                    invalidAirplane = true;
                    continue;
                }

                if (elemName == "number") {
                    number = elemValue;
                } else if (elemName == "callsign") {
                    callsign = elemValue;
                } else if (elemName == "model") {
                    model = elemValue;
                } else if (elemName == "fuel") {
                    unsigned int nodeTextAsInt;
                    if (tryCastStringToUnsignedInt(elemValue, &nodeTextAsInt)) {
                        if (nodeTextAsInt > 0) {
                            fuelCapacity = nodeTextAsInt;
                        } else {
                            errorStream << objectName << ": '" << elemName << "' attribute must be a strictly positive integer. " << getRowAndColumnStr(elem) << std::endl;
                            invalidAirplane = true;
                        }
                    }
                } else if (elemName == "passengers") {
                    unsigned int nodeTextAsInt;
                    if (tryCastStringToUnsignedInt(elemValue, &nodeTextAsInt)) {
                        if (nodeTextAsInt > 0) {
                            passengerCapacity = nodeTextAsInt;
                        } else {
                            errorStream << objectName << ": '" << elemName << "' attribute must be a strictly positive integer. " << getRowAndColumnStr(elem) << std::endl;
                            invalidAirplane = true;
                        }
                    }
                } else if (elemName == "status") {
                    AirplaneEnums::EStatus tempStatus = AirplaneEnums::StringToStatusEnum(elemValue.c_str());
                    if (tempStatus != AirplaneEnums::kInvalidStatus) {
                        if (tempStatus == AirplaneEnums::kStandingAtGate || tempStatus == AirplaneEnums::kApproaching) {
                            status = tempStatus;
                        } else {
                            errorStream << objectName << ": '" << elemName << "' initial status of airplane must be \"Standing at gate\" or \"Approaching\". " << getRowAndColumnStr(object->FirstChildElement(elemName.c_str())) << std::endl;
                            invalidAirplane = true;
                        }
                    } else {
                        errorStream << objectName << ": '" << elemName << "' attribute is not a valid status. " << getRowAndColumnStr(object->FirstChildElement(elemName.c_str())) << std::endl;
                        invalidAirplane = true;
                    }
                } else if (elemName == "type") {
                    AirplaneEnums::EType temp = AirplaneEnums::StringToTypeEnum(elemValue.c_str());
                    if (temp != AirplaneEnums::kInvalidType) {
                        type = temp;
                    } else {
                        errorStream << objectName << ": '" << elemName << "' attribute does not contain a valid option. " << getRowAndColumnStr(elem) << std::endl;
                        invalidAirplane = true;
                    }
                } else if (elemName == "size") {
                    AirplaneEnums::ESize temp = AirplaneEnums::StringToSizeEnum(elemValue.c_str());
                    if (temp != AirplaneEnums::kInvalidSize) {
                        size = temp;
                    } else {
                        errorStream << objectName << ": '" << elemName << "' attribute does not contain a valid option. " << getRowAndColumnStr(elem) << std::endl;
                        invalidAirplane = true;
                    }
                } else if (elemName == "engine") {
                    AirplaneEnums::EEngine temp = AirplaneEnums::StringToEngineEnum(elemValue.c_str());
                    if (temp != AirplaneEnums::kInvalidEngine) {
                        engine = temp;
                    } else {
                        errorStream << objectName << ": '" << elemName << "' attribute does not contain a valid option. " << getRowAndColumnStr(elem) << std::endl;
                        invalidAirplane = true;
                    }
                } else {
                    errorStream << objectName << ": '" << elemName << "' invalid attribute. " << getRowAndColumnStr(elem) << std::endl;
                    invalidAirplane = true;
                }
            }


            if (number.empty()) {
                errorStream << objectName << ": 'number' required attribute missing. " << getRowAndColumnStr(object) << std::endl;
                invalidAirplane = true;
            }
            if (callsign.empty()) {
                errorStream << objectName << ": 'callsign' required attribute missing. " << getRowAndColumnStr(object) << std::endl;
                invalidAirplane = true;
            }
            if (model.empty()) {
                errorStream << objectName << ": 'model' required attribute missing. " << getRowAndColumnStr(object) << std::endl;
                invalidAirplane = true;
            }
            if (fuelCapacity == 0) {
                errorStream << objectName << ": 'fuel' required attribute missing. " << getRowAndColumnStr(object) << std::endl;
                invalidAirplane = true;
            }
            if (passengerCapacity == 0) {
                errorStream << objectName << ": 'passengers' required attribute missing. " << getRowAndColumnStr(object) << std::endl;
                invalidAirplane = true;
            }
            if (status == AirplaneEnums::kInvalidStatus) {
                errorStream << objectName << ": 'status' required attribute missing. " << getRowAndColumnStr(object) << std::endl;
                invalidAirplane = true;
            }
            if (type == AirplaneEnums::kInvalidType) {
                errorStream << objectName << ": 'type' required attribute missing. " << getRowAndColumnStr(object) << std::endl;
                invalidAirplane = true;
            }
            if (size == AirplaneEnums::kInvalidSize) {
                errorStream << objectName << ": 'size' required attribute missing. " << getRowAndColumnStr(object) << std::endl;
                invalidAirplane = true;
            }
            if (engine == AirplaneEnums::kInvalidEngine) {
                errorStream << objectName << ": 'engine' required attribute missing. " << getRowAndColumnStr(object) << std::endl;
                invalidAirplane = true;
            }

            if (invalidAirplane) {
                parseResult = ParseEnum::kPartial;
            } else {
                unsigned int altitude = status == AirplaneEnums::kApproaching ? 10000 : 0;
                unsigned int fuel = fuelCapacity;
                unsigned int passengers = status == AirplaneEnums::kApproaching ? passengerCapacity : 0;

                Airplane* newAirplane = new Airplane(number, callsign, model, 0, altitude, fuel, fuelCapacity, passengers, passengerCapacity, status, type, size, engine);

                newAirplane->setAirport(airport);
                airport->addAirplane(newAirplane);
                airplanes.push_back(newAirplane);
            }
        } else {
            errorStream << "Invalid object '" << objectName << "'. " << getRowAndColumnStr(object) << std::endl;
            parseResult = ParseEnum::kPartial;
        }
    }

    AirplaneVectorIterator it_airplane = airport->getModifiableAirplanes().begin();
    while (it_airplane != airport->getModifiableAirplanes() .end()) {
        AirplaneEnums::EType airplaneType = (*it_airplane)->getType();
        AirplaneEnums::ESize airplaneSize = (*it_airplane)->getSize();
        AirplaneEnums::EEngine airplaneEngine = (*it_airplane)->getEngine();

        unsigned int offset = 00;

        if (airplaneType == AirplaneEnums::kPrivate) {
            if (airplaneSize == AirplaneEnums::kSmall) {
                offset = 01;
            }  else if (airplaneSize == AirplaneEnums::kMedium) {
                if (airplaneEngine == AirplaneEnums::kJet) { offset = 01000; }
            }
        } else if (airplaneType == AirplaneEnums::kAirline) {
            if (airplaneSize == AirplaneEnums::kMedium) {
                if (airplaneEngine == AirplaneEnums::kPropeller) { offset = 02000; }
                else if (airplaneEngine == AirplaneEnums::kJet) { offset = 03000; }
            } else if (airplaneSize == AirplaneEnums::kLarge) {
                if (airplaneEngine == AirplaneEnums::kJet) { offset = 04000; }
            }
        } else if (airplaneType == AirplaneEnums::kMilitary) {
            if (airplaneSize == AirplaneEnums::kSmall) {
                if (airplaneEngine == AirplaneEnums::kJet) { offset = 05000; }
            } else if (airplaneSize == AirplaneEnums::kLarge) {
                if (airplaneEngine == AirplaneEnums::kPropeller) { offset = 05000; }
            }
        } else if (airplaneType == AirplaneEnums::kEmergency) {
            if (airplaneSize == AirplaneEnums::kSmall) {
                if (airplaneEngine == AirplaneEnums::kPropeller) { offset = 06000; }
            }
        }

        if (offset == 0) {
            errorStream << "Invalid airplane combination: " << (*it_airplane)->getCallsign() << " (" << (*it_airplane)->getNumber() << ") " << std::endl;
            errorStream << "  Type: " << AirplaneEnums::EnumToString(airplaneType) << "  Size: " << AirplaneEnums::EnumToString(airplaneSize) << "  Engine: " << AirplaneEnums::EnumToString(airplaneEngine) << std::endl;
            parseResult = ParseEnum::kPartial;
            it_airplane = airport->getModifiableAirplanes().erase(it_airplane);
        } else {
            (*it_airplane)->setSquawk(offset + std::distance(airport->getModifiableAirplanes().begin(), it_airplane));
            it_airplane++;
        }
    }

    if (!isAirportStartConsistent(airport)) {
        errorStream << "Airport is not consistent." << std::endl;
    }

    xmlFile.Clear();

    return std::pair<ParseEnum::EResult, Airport*>(parseResult, airport);
}


std::map<std::string, std::string> Parser::convertXmlNodeToMap(TiXmlElement* object) {
    std::map<std::string, std::string> xmltomap;

    for (TiXmlElement* elem = object->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
        std::string elemName = elem->Value();

        TiXmlNode* elemChildElement = elem->FirstChildElement();
        TiXmlNode* elemChildNode = elem->FirstChild();

        if (elemChildElement == NULL) {
            if (elemChildNode != NULL) {
                if (xmltomap[elemName].empty()) {
                    xmltomap[elemName] = elemChildNode->Value();
                } else {
                    errorStream << object->Value() << ": Duplicate node found. " << getRowAndColumnStr(elem) << std::endl;
                }
            }
        } else {
            if (elemName != "TAXIROUTE") {
                errorStream << object->Value() << ": Unrecognized child element. " << getRowAndColumnStr(elem) << std::endl;
            }
        }

    }

    return xmltomap;
}

std::map<int, std::pair<bool, std::string> > Parser::extractTaxiRoute(TiXmlElement* object) {
    std::map<int, std::pair<bool, std::string> > taxiroute;
    std::set<std::string> duplicateChecker;

    for (TiXmlElement* elem = object->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
        std::string elemName = elem->Value();

        TiXmlNode* elemChildNode = elem->FirstChild();

        if (elemChildNode != NULL) {
            TiXmlText* elemChildNodeAsText = elemChildNode->ToText();

            if (elemChildNodeAsText != NULL) {
                std::string locationName = elemChildNodeAsText->Value();
                if (duplicateChecker.insert(locationName).second) {
                    if (elemName == "taxipoint") {
                        taxiroute[taxiroute.size()] = std::pair<bool, std::string>(true, locationName);
                    } else if (elemName == "crossing") {
                        taxiroute[taxiroute.size()] = std::pair<bool, std::string>(false, locationName);
                    } else {
                        errorStream << object->Value() << ": Unrecognized node name. " << getRowAndColumnStr(elemChildNodeAsText) << std::endl;
                    }
                } else {
                    errorStream << object->Value() << ": Duplicate crossing/taxipoint found. " << getRowAndColumnStr(elemChildNodeAsText) << std::endl;
                }
            } else {
                errorStream << object->Value() << ": Node does not contain text. " << getRowAndColumnStr(elemChildNode) << std::endl;
            }
        } else {
            errorStream << object->Value() << ": Node does not contain anything. " << getRowAndColumnStr(elem) << std::endl;
        }
    }

    return taxiroute;
}