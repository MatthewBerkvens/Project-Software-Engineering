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

std::string getRowAndColumnStr(const TiXmlNode* node)
{
    REQUIRE(node != NULL, "Referenced TiXmlElement is a NULLPTR.");
    std::ostringstream outputStream;
    outputStream << "(row " << node->Row() << ", column " << node->Column() << ")";
    return outputStream.str();
}

std::string getRowAndColumnStr(const TiXmlText* text)
{
    REQUIRE(text != NULL, "Referenced TiXmlElement is a NULLPTR.");
    std::ostringstream outputStream;
    outputStream << "(row " << text->Row() << ", column " << text->Column() << ")";
    return outputStream.str();
}


std::string convertToNATOAlphabet(std::string& string) {
    std::string result;

    for (std::string::const_iterator it_char = string.begin(); it_char != string.end(); it_char++) {
        if (!result.empty()) result += " ";
        switch(*it_char) {
            case 'A': case 'a': result += "Alpha"; break;
            case 'B': case 'b': result += "Bravo"; break;
            case 'C': case 'c': result += "Charlie"; break;
            case 'D': case 'd': result += "Delta"; break;
            case 'E': case 'e': result += "Echo"; break;
            case 'F': case 'f': result += "Foxtrot"; break;
            case 'G': case 'g': result += "Golf"; break;
            case 'H': case 'h': result += "Hotel"; break;
            case 'I': case 'i': result += "India"; break;
            case 'J': case 'j': result += "Juliet"; break;
            case 'K': case 'k': result += "Kilo"; break;
            case 'L': case 'l': result += "Lima"; break;
            case 'M': case 'm': result += "Mike"; break;
            case 'N': case 'n': result += "November"; break;
            case 'O': case 'o': result += "Oscar"; break;
            case 'P': case 'p': result += "Papa"; break;
            case 'Q': case 'q': result += "Quebec"; break;
            case 'R': case 'r': result += "Romeo"; break;
            case 'S': case 's': result += "Sierra"; break;
            case 'T': case 't': result += "Tango"; break;
            case 'U': case 'u': result += "Uniform"; break;
            case 'V': case 'v': result += "Victor"; break;
            case 'W': case 'w': result += "Whiskey"; break;
            case 'X': case 'x': result += "Xray"; break;
            case 'Y': case 'y': result += "Yankee"; break;
            case 'Z': case 'z': result += "Zulu"; break;
            case '0': result += "Zero"; break;
            case '1': result += "One"; break;
            case '2': result += "Two"; break;
            case '3': result += "Three"; break;
            case '4': result += "Four"; break;
            case '5': result += "Five"; break;
            case '6': result += "Six"; break;
            case '7': result += "Seven"; break;
            case '8': result += "Eight"; break;
            case '9': result += "Nine"; break;
            default: break;
        }
    }

    return result;
}