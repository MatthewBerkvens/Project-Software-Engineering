#ifndef INC_UTILS_H
#define INC_UTILS_H

#include "../lib/tinyxml/tinyxml.h"
#include "../lib/DesignByContract.h"

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

class Runway;
typedef std::vector<Runway*> RunwayVector;
typedef std::vector<Runway*>::iterator RunwayVectorIterator;
typedef std::vector<Runway*>::const_iterator RunwayVectorConstIterator;

class Airport;
typedef std::vector<Airport*> AirportVector;
typedef std::vector<Airport*>::iterator AirportVectorIterator;
typedef std::vector<Airport*>::const_iterator AirportVectorConstIterator;

/**
 * Returns true if string only contains alphabetical characters (lower & uppercase), digits, or spaces
 * PRE:
 * REQUIRE(!str.empty(), "Referenced string is empty.");
 */
bool isString(const std::string& str);

/**
 * Returns true if string was succesfully cast, false if not.
 * If true, the referenced unsigned integer is the cast value.
 * PRE:
 * REQUIRE(i != NULL, "Referenced unsigned int was a NULLPTR");
 * REQUIRE(!str.empty(), "Referenced unsigned int was a NULLPTR");
 */
bool tryCastStringToUnsignedInt(const std::string& str, unsigned int* i);

/**
 * Returns a string in the format of (row {ROW}, column {COLUMN}) of the referenced TiXmlElement \n
 * PRE:
 * REQUIRE(elem != NULL, "Referenced TiXmlElement was a NULLPTR");
 */
std::string getRowAndColumnStr(const TiXmlElement* elem);

template<typename AType>
bool vectorContains(std::vector<AType> vector, AType element)
{
    return std::find(vector.begin(), vector.end(), element) != vector.end();
}

template<typename AType>
bool removeFromVector(std::vector<AType> vector, AType element)
{
    typename std::vector<AType>::iterator it_runway = std::find(vector.begin(), vector.end(), element);
    if (it_runway != vector.end())
    {
        vector.erase(it_runway);
        return true;
    }
    else return false;
}

#endif