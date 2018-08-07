#include "parser.h"

Parser::Parser(std::ostream& _errorStream) : errorStream(_errorStream) {};

std::pair<ParseEnum::EResult, std::map<std::string, Airport*> > Parser::parseFile(const char* fileName) {
    TiXmlDocument xmlFile;

    std::map<std::string, Airport*> allAirports;

    if (!xmlFile.LoadFile(fileName)) {
        errorStream << "Error loading file: " << xmlFile.ErrorDesc() << std::endl;
        xmlFile.Clear();
        return std::pair<ParseEnum::EResult, std::map<std::string, Airport*> >(ParseEnum::kAborted, allAirports);
    }

    if (xmlFile.FirstChildElement() == NULL) {
        errorStream << "Error loading file: root missing" << std::endl;
        xmlFile.Clear();
        return std::pair<ParseEnum::EResult, std::map<std::string, Airport*> >(ParseEnum::kPartial, allAirports);
    }

    ParseEnum::EResult parseResult = ParseEnum::kSuccess;

    Airport* lastAirport;

    for (TiXmlElement* object = xmlFile.FirstChildElement(); object != NULL; object = object->NextSiblingElement()) {
        std::string objectName = object->Value();
        if (objectName == "AIRPORT") {
            std::string airportName;
            std::string airportIATA;
            std::string airportCallsign;
            unsigned int airportGates = 0;

            bool invalidAirport = false;

            std::map<std::string, std::string> airportMap = convertXmlNodeToMap(object);

            if (!airportMap["name"].empty()) {
                airportName = airportMap["name"];
            } else {
                invalidAirport = true;
                errorStream << objectName << ": 'name' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (!airportMap["iata"].empty()) {
                airportIATA = airportMap["iata"];
            } else {
                invalidAirport = true;
                errorStream << objectName << ": 'iata' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (!airportMap["callsign"].empty()) {
                airportCallsign = airportMap["callsign"];
            } else {
                invalidAirport = true;
                errorStream << objectName << ": 'callsign' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (!airportMap["gates"].empty()) {
                unsigned int gatesAsInt;
                if (tryCastStringToUnsignedInt(airportMap["gates"], &gatesAsInt)) {
                    airportGates = gatesAsInt;
                } else {
                    invalidAirport = true;
                    errorStream << objectName << ": 'gates' attribute is not an unsigned integer. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidAirport = true;
                errorStream << objectName << ": 'gates' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (allAirports[airportIATA] != NULL) {
                invalidAirport = true;
                errorStream << objectName << ": duplicate airport IATA. " << getRowAndColumnStr(object) << std::endl;
            }

            if (invalidAirport) {
                parseResult = ParseEnum::kPartial;
            } else {
                Airport* newAirport = new Airport(airportName, airportIATA, airportCallsign, airportGates);
                allAirports[airportIATA] = newAirport;
                lastAirport = newAirport;
            }
        } else if (objectName == "RUNWAY") {
            std::string runwayName = "";
            unsigned int runwayLength = 0;
            RunwayEnums::EType runwayType = RunwayEnums::kInvalidType;
            Airport* runwayAirport = NULL;

            bool invalidRunway = false;

            std::map<std::string, std::string> runwayMap = convertXmlNodeToMap(object);

            std::vector<std::pair<bool, std::string> > taxirouteMap = extractTaxiRoute(object->FirstChildElement("TAXIROUTE"));

            if (!runwayMap["name"].empty()) {
                runwayName = runwayMap["name"];
            } else {
                invalidRunway = true;
                errorStream << objectName << ": 'name' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (!runwayMap["length"].empty()) {
                unsigned int lengthAsInt;
                if (tryCastStringToUnsignedInt(runwayMap["length"], &lengthAsInt)) {
                    runwayLength = lengthAsInt;
                } else {
                    invalidRunway = true;
                    errorStream << objectName << ": 'length' attribute is not an unsigned integer. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidRunway = true;
                errorStream << objectName << ": 'length' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (!runwayMap["airport"].empty()) {
                runwayAirport = allAirports[runwayMap["airport"]];
                if (runwayAirport == NULL) {
                    invalidRunway = true;
                    errorStream << objectName << ": referenced airport does not exist, make sure to declare runways after the referenced airport. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidRunway = true;
                errorStream << objectName << ": 'airport' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (!runwayMap["type"].empty()) {
                RunwayEnums::EType tempType = RunwayEnums::StringToTypeEnum(runwayMap["type"].c_str());
                if (tempType != RunwayEnums::kInvalidType) {
                    runwayType = tempType;
                } else {
                    invalidRunway = true;
                    errorStream << objectName << ": 'type' attribute does not contain a valid option. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidRunway = true;
                errorStream << objectName << ": 'type' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (invalidRunway) {
                parseResult = ParseEnum::kPartial;
            } else {
                Runway* newRunway = new Runway(runwayName, runwayLength, runwayType);
                newRunway->setAirport(runwayAirport);

                runwayAirport->addRunway(newRunway);
                runwayAirport->addLocation(newRunway);

                //adding all locations first
                for (unsigned int i = 0; i < taxirouteMap.size(); i++) {
                    std::pair<bool, std::string> currentNode = taxirouteMap[i];
                    std::string nodeName = currentNode.second;

                    if (runwayAirport->getLocationByName(nodeName) == NULL) {
                        //if currentNode.first is true, node is a crossing of a runway
                        if (!currentNode.first) {
                            Location* newLocation = new Location(nodeName);
                            newLocation->setAirport(runwayAirport);
                            runwayAirport->addLocation(newLocation);
                        } else {
                            invalidRunway = true;
                            errorStream << objectName << ": Runways added in wrong order, this one has defined runways before they were added. " << getRowAndColumnStr(object) << std::endl;
                        }
                    }
                }

                for (unsigned int i = 0; i < taxirouteMap.size(); i++) {
                    std::pair<bool, std::string> currentNode = taxirouteMap[i];
                    std::string nodeName = currentNode.second;

                    bool start_index = (i == 0);
                    bool end_index = ((i + 1) == taxirouteMap.size());


                    Location* previousLocation = NULL;
                    Location* currentLocation = runwayAirport->getLocationByName(nodeName);
                    Location* nextLocation = NULL;

                    if (!start_index) {
                        previousLocation = runwayAirport->getLocationByName(taxirouteMap[i-1].second);
                        previousLocation->setNextLocation(currentLocation);
                    }

                    if (!end_index) {
                        nextLocation = runwayAirport->getLocationByName(taxirouteMap[i+1].second);
                    } else {
                        nextLocation = newRunway;
                    }

                    nextLocation->setPreviousLocation(currentLocation);
                    currentLocation->setPreviousLocation(previousLocation);
                    currentLocation->setNextLocation(nextLocation);
                }
            }
        } else if (objectName == "AIRPLANE") {
            std::string airplaneNumber = "";
            std::string airplaneCallsign = "";
            std::string airplaneModel = "";

            unsigned int airplaneFuelCapacity = 0;
            unsigned int airplanePassengerCapacity = 0;

            AirplaneEnums::EStatus airplaneStatus = AirplaneEnums::kInvalidStatus;
            AirplaneEnums::EType airplaneType = AirplaneEnums::kInvalidType;
            AirplaneEnums::ESize airplaneSize = AirplaneEnums::kInvalidSize;
            AirplaneEnums::EEngine airplaneEngine = AirplaneEnums::kInvalidEngine;

            bool invalidAirplane = false;

            std::map<std::string, std::string> airplaneMap = convertXmlNodeToMap(object);

            if (!airplaneMap["number"].empty()) {
                airplaneNumber = airplaneMap["number"];
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'number' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (!airplaneMap["callsign"].empty()) {
                airplaneCallsign = airplaneMap["callsign"];
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'callsign' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (!airplaneMap["model"].empty()) {
                airplaneModel = airplaneMap["model"];
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'model' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (!airplaneMap["fuel"].empty()) {
                unsigned int fuelCapacityAsInt;
                if (tryCastStringToUnsignedInt(airplaneMap["fuel"], &fuelCapacityAsInt)) {
                    airplaneFuelCapacity = fuelCapacityAsInt;
                } else {
                    invalidAirplane = true;
                    errorStream << objectName << ": 'fuel' attribute is not an unsigned integer. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'fuel' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (!airplaneMap["passengers"].empty()) {
                unsigned int passengerCapacityAsInt;
                if (tryCastStringToUnsignedInt(airplaneMap["passengers"], &passengerCapacityAsInt)) {
                    airplanePassengerCapacity = passengerCapacityAsInt;
                } else {
                    invalidAirplane = true;
                    errorStream << objectName << ": 'passengers' attribute is not an unsigned integer. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'passengers' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (!airplaneMap["status"].empty()) {
                AirplaneEnums::EStatus tempStatus = AirplaneEnums::StringToStatusEnum(airplaneMap["status"].c_str());
                if (tempStatus != AirplaneEnums::kInvalidStatus) {
                    airplaneStatus = tempStatus;
                } else {
                    invalidAirplane = true;
                    errorStream << objectName << ": 'status' attribute does not contain a valid option. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'status' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (!airplaneMap["type"].empty()) {
                AirplaneEnums::EType tempType = AirplaneEnums::StringToTypeEnum(airplaneMap["type"].c_str());
                if (tempType != AirplaneEnums::kInvalidType) {
                    airplaneType = tempType;
                } else {
                    invalidAirplane = true;
                    errorStream << objectName << ": 'type' attribute does not contain a valid option. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'type' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (!airplaneMap["size"].empty()) {
                AirplaneEnums::ESize tempSize = AirplaneEnums::StringToSizeEnum(airplaneMap["size"].c_str());
                if (tempSize != AirplaneEnums::kInvalidSize) {
                    airplaneSize = tempSize;
                } else {
                    invalidAirplane = true;
                    errorStream << objectName << ": 'size' attribute does not contain a valid option. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'size' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (!airplaneMap["engine"].empty()) {
                AirplaneEnums::EEngine tempType = AirplaneEnums::StringToEngineEnum(airplaneMap["engine"].c_str());
                if (tempType != AirplaneEnums::kInvalidEngine) {
                    airplaneEngine = tempType;
                } else {
                    invalidAirplane = true;
                    errorStream << objectName << ": 'engine' attribute does not contain a valid option. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'engine' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }


            if (invalidAirplane) {
                parseResult = ParseEnum::kPartial;
            } else {
                unsigned int altitude = (airplaneStatus == AirplaneEnums::kApproaching ? 10000 : 0);
                unsigned int fuel = airplaneFuelCapacity;
                unsigned int passengers = (airplaneStatus == AirplaneEnums::kApproaching ? airplanePassengerCapacity : 0);

                Airplane* newAirplane = new Airplane(airplaneNumber, airplaneCallsign, airplaneModel, 0, altitude, fuel, airplaneFuelCapacity, passengers, airplanePassengerCapacity, airplaneStatus, airplaneType, airplaneSize, airplaneEngine);

                newAirplane->setAirport(lastAirport);
                lastAirport->addAirplane(newAirplane);
            }
        } else {
            errorStream << "Invalid object '" << objectName << "'. " << getRowAndColumnStr(object) << std::endl;
            parseResult = ParseEnum::kPartial;
        }
    }

    for (AirportMap::const_iterator it_airport = allAirports.begin(); it_airport != allAirports.end(); it_airport++) {
        Airport* airport = (*it_airport).second;

        for (AirplaneMap::iterator it_airplane = airport->getAirplanes().begin(); it_airplane != airport->getAirplanes().end(); ) {
            Airplane* airplane = (*it_airplane).second;

            AirplaneEnums::EType airplaneType = airplane->getType();
            AirplaneEnums::ESize airplaneSize = airplane->getSize();
            AirplaneEnums::EEngine airplaneEngine = airplane->getEngine();

            unsigned int offset = 00;

            if (airplaneType == AirplaneEnums::kPrivate) {
                if (airplaneSize == AirplaneEnums::kSmall) {
                    offset = 01;
                } else if (airplaneSize == AirplaneEnums::kMedium) {
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
                errorStream << "Invalid airplane combination: " << airplane->getCallsign() << " (" << airplane->getNumber() << ") " << std::endl;
                errorStream << "  Type: " << AirplaneEnums::EnumToString(airplaneType) << "  Size: " << AirplaneEnums::EnumToString(airplaneSize) << "  Engine: "
                            << AirplaneEnums::EnumToString(airplaneEngine) << std::endl;
                parseResult = ParseEnum::kPartial;
                delete (*it_airplane).second;
                airport->getAirplanes().erase(it_airplane++);
            } else {
                airplane->setSquawk(offset + std::distance(airport->getAirplanes().begin(), it_airplane));
                it_airplane++;
            }
        }

        if (!isAirportStartConsistent(airport)) {
            errorStream << "Airport " << airport->getIata() << " is not consistent." << std::endl;
        }
    }

    xmlFile.Clear();

    return std::pair<ParseEnum::EResult, std::map<std::string, Airport*> >(parseResult, allAirports);
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

std::vector<std::pair<bool, std::string> > Parser::extractTaxiRoute(TiXmlElement* object) {
    std::vector<std::pair<bool, std::string> > taxiroute;
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
                        taxiroute.push_back(std::pair<bool, std::string>(false, locationName));
                    } else if (elemName == "crossing") {
                        taxiroute.push_back(std::pair<bool, std::string>(true, locationName));
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