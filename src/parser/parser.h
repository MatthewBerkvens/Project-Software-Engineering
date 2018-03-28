#ifndef INC_PARSER_H
#define INC_PARSER_H

#include "../lib/tinyxml/tinyxml.h"
#include "../lib/DesignByContract.h"
#include "../utils/utils.h"

#include "../classes/Airport.h"
#include "../classes/Runway.h"
#include "../classes/Airplane.h"

#include <string>
#include <iostream>
#include <vector>
#include <map>

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

public:
    Parser();

    ParseEnum::EResult parseFile(const char* fileName, std::ostream& errorStream);
};


#endif
