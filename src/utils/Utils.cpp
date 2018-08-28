#include "Utils.h"

bool DirectoryExists(const std::string dirname) {
    struct stat st;
    return stat(dirname.c_str(), &st) == 0;
}

bool FileExists(const std::string filename) {
    struct stat st;
    if (stat(filename.c_str(), &st) != 0) return false;
    std::ifstream f(filename.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }
}

bool FileIsEmpty(const std::string filename) {
    struct stat st;
    if (stat(filename.c_str(), &st) != 0) return true; // File does not exist; thus it is empty
    return st.st_size == 0;
}

bool FileCompare(const std::string leftFileName, const std::string rightFileName) {
    std::ifstream leftFile, rightFile;
    char leftRead, rightRead;
    bool result;

    // Open the two files.
    leftFile.open(leftFileName.c_str());
    if (!leftFile.is_open()) {
        return false;
    };
    rightFile.open(rightFileName.c_str());
    if (!rightFile.is_open()) {
        leftFile.close();
        return false;
    };

    result = true; // files exist and are open; assume equality unless a counterexamples shows up.
    while (result && leftFile.good() && rightFile.good()) {
        leftFile.get(leftRead);
        rightFile.get(rightRead);
        result = (leftRead == rightRead);
    };
    if (result) {
        // last read was still equal; are we at the end of both files ?
        result = (!leftFile.good()) && (!rightFile.good());
    };

    leftFile.close();
    rightFile.close();
    return result;
}

bool isString(const std::string& str) {
    REQUIRE(!str.empty(), "Referenced string is empty.");
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

bool tryCastStringToInt(const std::string& str, int* i)
{
    REQUIRE(i != NULL, "Referenced int is a nullpointer.");
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

std::string convertPartialNumbersToNATOAlphabet(const std::string& string) {
    std::string result;

    for (std::string::const_iterator it_char = string.begin(); it_char != string.end(); it_char++) {
        if (*it_char >= '0' && *it_char <= '9') {
            if (!result.empty() && result[result.size() - 1] != ' ') result += " ";
            switch(*it_char) {
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
        } else {
            result += *it_char;
        }
    }

    return result;
}

std::string convertToNATOAlphabet(const std::string& string) {
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