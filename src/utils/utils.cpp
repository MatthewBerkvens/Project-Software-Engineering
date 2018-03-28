#include "utils.h"

bool isString(const std::string& str) {
    REQUIRE(!str.empty(), "References string is empty.");
    for (std::string::const_iterator it_char = str.begin(); it_char != str.end(); it_char++) {
        if (!(
                (*it_char >= 'a' && *it_char <= 'z') ||
                (*it_char >= 'A' && *it_char <= 'Z') ||
                (*it_char >= '0' && *it_char <= '9') ||
                (*it_char == ' ')
        )) return false;
    }
    return true;
};

bool tryCastStringToUnsignedInt(const std::string& str, unsigned int* i)
{
    REQUIRE(i != NULL, "Referenced unsigned int is a NULLPTR.");
    REQUIRE(!str.empty(), "Referenced unsigned int is a NULLPTR.");
    int result = std::sscanf(str.c_str(), "%u", i);
    return (result && result != EOF);
}

std::string getRowAndColumnStr(const TiXmlElement* elem)
{
    REQUIRE(elem != NULL, "Referenced TiXmlElement is a NULLPTR.");
    std::ostringstream outputStream;
    outputStream << "(row " << elem->Row() << ", column " << elem->Column() << ")";
    return outputStream.str();
}