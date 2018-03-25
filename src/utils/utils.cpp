#include "utils.h"

bool isString(const std::string& str) {
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
