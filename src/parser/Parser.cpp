#include "Parser.h"

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
        return std::pair<ParseEnum::EResult, std::map<std::string, Airport*> >(ParseEnum::kAborted, allAirports);
    }

    ParseEnum::EResult parseResult = ParseEnum::kSuccess;

    Airport* lastAirport = NULL;

    for (TiXmlElement* object = xmlFile.FirstChildElement(); object != NULL; object = object->NextSiblingElement()) {
        std::string objectName = object->Value();
        if (objectName == "AIRPORT") {
            std::string airportName;
            std::string airportIATA;
            std::string airportCallsign;
            unsigned int airportGates = 0;

            bool invalidAirport = false;

            std::map<std::string, std::string> airportMap = convertXmlNodeToMap(object);

            if (airportMap.find("name") != airportMap.end()) {
                airportName = airportMap.at("name");
            } else {
                invalidAirport = true;
                errorStream << objectName << ": 'name' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (airportMap.find("iata") != airportMap.end()) {
                airportIATA = airportMap.at("iata");
            } else {
                invalidAirport = true;
                errorStream << objectName << ": 'iata' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (airportMap.find("callsign") != airportMap.end()) {
                airportCallsign = airportMap.at("callsign");
            } else {
                invalidAirport = true;
                errorStream << objectName << ": 'callsign' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (airportMap.find("gates") != airportMap.end()) {
                int gatesAsInt = 0;
                int* ptr = &gatesAsInt;
                REQUIRE(ptr != NULL, "wtf");
                if (tryCastStringToInt(airportMap.at("gates"), ptr) && gatesAsInt > 0) {
                    airportGates = static_cast<unsigned int>(gatesAsInt);
                } else {
                    invalidAirport = true;
                    errorStream << objectName << ": 'gates' attribute is not an unsigned positive integer. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidAirport = true;
                errorStream << objectName << ": 'gates' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (allAirports.find(airportIATA) != allAirports.end()) {
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

            TiXmlElement* taxiRouteNode = object->FirstChildElement("TAXIROUTE");
            std::vector<std::pair<bool, std::string> > taxirouteMap;

            if (taxiRouteNode != NULL) {
                taxirouteMap = extractTaxiRoute(taxiRouteNode);
            } else {
                invalidRunway = true;
                errorStream << objectName << ": 'taxiroute' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (runwayMap.find("name") != runwayMap.end()) {
                runwayName = runwayMap.at("name");
            } else {
                invalidRunway = true;
                errorStream << objectName << ": 'name' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (runwayMap.find("length") != runwayMap.end()) {
                int lengthAsInt = 0;
                if (tryCastStringToInt(runwayMap.at("length"), &lengthAsInt) && lengthAsInt > 0) {
                    runwayLength = static_cast<unsigned int>(lengthAsInt);
                } else {
                    invalidRunway = true;
                    errorStream << objectName << ": 'length' attribute is not an unsigned positive integer. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidRunway = true;
                errorStream << objectName << ": 'length' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (runwayMap.find("airport") != runwayMap.end()) {
                if (allAirports.find(runwayMap.at("airport")) != allAirports.end()) {
                    runwayAirport = allAirports.at(runwayMap.at("airport"));
                } else {
                    invalidRunway = true;
                    errorStream << objectName << ": referenced airport does not exist, make sure to declare runways after the referenced airport. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidRunway = true;
                errorStream << objectName << ": 'airport' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (runwayMap.find("type") != runwayMap.end()) {
                RunwayEnums::EType tempType = RunwayEnums::StringToTypeEnum(runwayMap.at("type").c_str());
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

            AirplaneEnums::EStatus airplaneStatus = AirplaneEnums::kStatus_InvalidStatus;
            AirplaneEnums::EType airplaneType = AirplaneEnums::kType_InvalidType;
            AirplaneEnums::ESize airplaneSize = AirplaneEnums::kSize_InvalidSize;
            AirplaneEnums::EEngine airplaneEngine = AirplaneEnums::kEngine_InvalidEngine;

            bool invalidAirplane = false;

            std::map<std::string, std::string> airplaneMap = convertXmlNodeToMap(object);

            if (airplaneMap.find("number") != airplaneMap.end()) {
                airplaneNumber = airplaneMap.at("number");
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'number' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (airplaneMap.find("callsign") != airplaneMap.end()) {
                airplaneCallsign = airplaneMap.at("callsign");
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'callsign' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (airplaneMap.find("model") != airplaneMap.end()) {
                airplaneModel = airplaneMap.at("model");
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'model' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (airplaneMap.find("fuel") != airplaneMap.end()) {
                int fuelCapacityAsInt = 0;
                if (tryCastStringToInt(airplaneMap.at("fuel"), &fuelCapacityAsInt) && fuelCapacityAsInt > 0) {
                    airplaneFuelCapacity = static_cast<unsigned int>(fuelCapacityAsInt);
                } else {
                    invalidAirplane = true;
                    errorStream << objectName << ": 'fuel' attribute is not an unsigned positive integer. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'fuel' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (airplaneMap.find("passengers") != airplaneMap.end()) {
                int passengerCapacityAsInt = 0;
                if (tryCastStringToInt(airplaneMap.at("passengers"), &passengerCapacityAsInt) && passengerCapacityAsInt > 0) {
                    airplanePassengerCapacity = static_cast<unsigned int>(passengerCapacityAsInt);
                } else {
                    invalidAirplane = true;
                    errorStream << objectName << ": 'passengers' attribute is not an unsigned positive integer. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'passengers' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (airplaneMap.find("status") != airplaneMap.end()) {
                AirplaneEnums::EStatus tempStatus = AirplaneEnums::StringToStatusEnum(airplaneMap.at("status").c_str());
                if (tempStatus == AirplaneEnums::kStatus_Approaching || tempStatus == AirplaneEnums::kStatus_StandingAtGate) {
                    airplaneStatus = tempStatus;
                } else {
                    invalidAirplane = true;
                    errorStream << objectName << ": 'status' attribute does not contain a valid option. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'status' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (airplaneMap.find("type") != airplaneMap.end()) {
                AirplaneEnums::EType tempType = AirplaneEnums::StringToTypeEnum(airplaneMap.at("type").c_str());
                if (tempType != AirplaneEnums::kType_InvalidType) {
                    airplaneType = tempType;
                } else {
                    invalidAirplane = true;
                    errorStream << objectName << ": 'type' attribute does not contain a valid option. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'type' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (airplaneMap.find("size") != airplaneMap.end()) {
                AirplaneEnums::ESize tempSize = AirplaneEnums::StringToSizeEnum(airplaneMap.at("size").c_str());
                if (tempSize != AirplaneEnums::kSize_InvalidSize) {
                    airplaneSize = tempSize;
                } else {
                    invalidAirplane = true;
                    errorStream << objectName << ": 'size' attribute does not contain a valid option. " << getRowAndColumnStr(object) << std::endl;
                }
            } else {
                invalidAirplane = true;
                errorStream << objectName << ": 'size' required attribute is missing. " << getRowAndColumnStr(object) << std::endl;
            }

            if (airplaneMap.find("engine") != airplaneMap.end()) {
                AirplaneEnums::EEngine tempType = AirplaneEnums::StringToEngineEnum(airplaneMap.at("engine").c_str());
                if (tempType != AirplaneEnums::kEngine_InvalidEngine) {
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
            } else if (lastAirport != NULL) {
                unsigned int altitude = (airplaneStatus == AirplaneEnums::kStatus_Approaching ? 10000 : 0);

                Airplane* newAirplane = new Airplane(airplaneNumber, airplaneCallsign, airplaneModel, 0, altitude, airplaneFuelCapacity, airplaneFuelCapacity, airplanePassengerCapacity,
                                                     airplanePassengerCapacity, airplaneStatus, airplaneType, airplaneSize, airplaneEngine);

                newAirplane->setAirport(lastAirport);
                lastAirport->addAirplane(newAirplane);

                if (airplaneStatus == AirplaneEnums::kStatus_StandingAtGate) {
                    AirplaneVector::iterator it_freeGate = std::find(lastAirport->getGates().begin(), lastAirport->getGates().end(), static_cast<Airplane*>(NULL));
                    if (it_freeGate != lastAirport->getGates().end()) {
                        *it_freeGate = newAirplane;
                        newAirplane->setGate(std::distance(lastAirport->getGates().begin(), it_freeGate));
                    } else {
                        errorStream << objectName << ": no free gate for new airplane. " << getRowAndColumnStr(object) << std::endl;
                        delete newAirplane;
                    }
                }
            } else {
                errorStream << objectName << ": No airport defined before this airplane. " << getRowAndColumnStr(object) << std::endl;
            }
        } else {
            errorStream << "Invalid object '" << objectName << "'. " << getRowAndColumnStr(object) << std::endl;
            parseResult = ParseEnum::kPartial;
        }
    }

    for (AirportMap::const_iterator it_airport = allAirports.begin(); it_airport != allAirports.end(); it_airport++) {
        Airport* airport = it_airport->second;

        for (AirplaneMap::iterator it_airplane = airport->getAirplanes().begin(); it_airplane != airport->getAirplanes().end(); ) {
            Airplane* airplane = it_airplane->second;

            AirplaneEnums::EType airplaneType = airplane->getType();
            AirplaneEnums::ESize airplaneSize = airplane->getSize();
            AirplaneEnums::EEngine airplaneEngine = airplane->getEngine();

            unsigned int offset = 00;

            if (airplaneType == AirplaneEnums::kType_Private) {
                if (airplaneSize == AirplaneEnums::kSize_Small) {
                    offset = 01;
                } else if (airplaneSize == AirplaneEnums::kSize_Medium) {
                    if (airplaneEngine == AirplaneEnums::kEngine_Jet) { offset = 01000; }
                }
            } else if (airplaneType == AirplaneEnums::kType_Airline) {
                if (airplaneSize == AirplaneEnums::kSize_Medium) {
                    if (airplaneEngine == AirplaneEnums::kEngine_Propeller) { offset = 02000; }
                    else if (airplaneEngine == AirplaneEnums::kEngine_Jet) { offset = 03000; }
                } else if (airplaneSize == AirplaneEnums::kSize_Large) {
                    if (airplaneEngine == AirplaneEnums::kEngine_Jet) { offset = 04000; }
                }
            } else if (airplaneType == AirplaneEnums::kType_Military) {
                if (airplaneSize == AirplaneEnums::kSize_Small) {
                    if (airplaneEngine == AirplaneEnums::kEngine_Jet) { offset = 05000; }
                } else if (airplaneSize == AirplaneEnums::kSize_Large) {
                    if (airplaneEngine == AirplaneEnums::kEngine_Propeller) { offset = 05000; }
                }
            } else if (airplaneType == AirplaneEnums::kType_Emergency) {
                if (airplaneSize == AirplaneEnums::kSize_Small) {
                    if (airplaneEngine == AirplaneEnums::kEngine_Propeller) { offset = 06000; }
                }
            }

            if (offset == 00) {
                errorStream << "Invalid airplane combination: " << airplane->getCallsign() << " (" << airplane->getNumber() << ") " << std::endl;
                errorStream << "  Type: " << AirplaneEnums::EnumToString(airplaneType) << "  Size: " << AirplaneEnums::EnumToString(airplaneSize) << "  Engine: "
                            << AirplaneEnums::EnumToString(airplaneEngine) << std::endl;
                parseResult = ParseEnum::kPartial;
                delete it_airplane->second;
                airport->getAirplanes().erase(it_airplane++);
            } else {
                airplane->setSquawk(offset + std::distance(airport->getAirplanes().begin(), it_airplane));
                it_airplane++;
            }
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
                if (xmltomap.find(elemName) == xmltomap.end()) {
                    xmltomap[elemName] = elemChildNode->Value();
                } else {
                    errorStream << object->Value() << ": Duplicate node found. " << getRowAndColumnStr(elem) << std::endl;
                }
            } else {
                xmltomap[elemName] = "";
                errorStream << object->Value() << ": Empty node found. " << getRowAndColumnStr(elem) << std::endl;
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