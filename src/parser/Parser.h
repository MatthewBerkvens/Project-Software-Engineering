#ifndef INC_PARSER_H
#define INC_PARSER_H

#include "../lib/tinyxml/tinyxml.h"
#include "../lib/DesignByContract.h"

#include "../utils/Utils.h"

#include "../classes/Airport.h"
#include "../classes/Runway.h"
#include "../classes/Airplane.h"

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <set>

namespace ParseEnum {
    enum EResult {
        kInvalidResult,
        kSuccess,
        kPartial,
        kAborted,
    };
}

class Parser {
private:
    std::ostream& errorStream;
public:
    Parser(std::ostream& _errorStream);

    std::pair<ParseEnum::EResult, std::map<std::string, Airport*> > parseFile(const char* fileName);

    std::map<std::string, std::string> convertXmlNodeToMap(TiXmlElement* object);

    std::vector<std::pair<bool, std::string> > extractTaxiRoute(TiXmlElement* object);
};

#endif
