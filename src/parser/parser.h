#ifndef INC_PARSER_H
#define INC_PARSER_H

#include "../lib/tinyxml/tinyxml.h"
#include "../lib/DesignByContract.h"
#include "../utils/utils.h"

#include "../classes/Airport.h"

#include <string>
#include <iostream>

namespace EParseEnum {
    enum EResult {
        kInvalidResult,
        kSuccess,
        kPartial,
        kAborted,
    };
}

class Parser {
private:
    Airport* parseAirport(TiXmlElement* elem, std::ostream& errorStream);

public:
    Parser();

    ~Parser();

    EParseEnum::EResult parseFile(const char* fileName, std::ostream& errorStream);
};


#endif
