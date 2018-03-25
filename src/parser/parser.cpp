#include "parser.h"

Parser::Parser() {};

EParseEnum::EResult Parser::parseFile(const char* fileName, std::ostream& errorStream) {
    TiXmlDocument xmlFile;

    if (!xmlFile.LoadFile(fileName)) {
        errorStream << "Error loading file: " << xmlFile.ErrorDesc() << std::endl;
        xmlFile.Clear();
        return EParseEnum::kAborted;
    }

    if (xmlFile.FirstChildElement() == NULL) {
        errorStream << "Error loading file: root missing" << std::endl;
        xmlFile.Clear();
        return EParseEnum::kPartial;
    }

    for (TiXmlElement* elem = xmlFile.FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
        std::string elemName = elem->Value();
        if (elemName == "AIRPORT") {
            //Airport* newAirport = parseAirport(elem->FirstChildElement());
        }
    }

    return EParseEnum::kSuccess;
};