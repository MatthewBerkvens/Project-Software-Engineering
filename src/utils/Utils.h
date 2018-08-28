#ifndef INC_UTILS_H
#define INC_UTILS_H

#include "../lib/tinyxml/tinyxml.h"
#include "../lib/DesignByContract.h"

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <map>

class Airport;
typedef std::vector<Airport*> AirportVector;
typedef std::map<std::string, Airport*> AirportMap;

class Airplane;
typedef std::vector<Airplane*> AirplaneVector;
typedef std::map<std::string, Airplane*> AirplaneMap;

class Runway;
typedef std::vector<Runway*> RunwayVector;
typedef std::map<std::string, Runway*> RunwayMap;

class Location;
typedef std::vector<Location*> LocationVector;
typedef std::map<std::string, Location*> LocationMap;

bool DirectoryExists(const std::string dirname);

bool FileExists(const std::string dirname);

bool FileIsEmpty(const std::string filename);

bool FileCompare(const std::string leftFileName, const std::string rightFileName);

/**
 * Returns true if string only contains alphabetical characters (lower & uppercase), digits, or spaces
 * PRE:
 * REQUIRE(!str.empty(), "Referenced string is empty.");
 */
bool isString(const std::string& str);

bool tryCastStringToInt(const std::string& str, int* i);

/**
 * Returns a string in the format of (row {ROW}, column {COLUMN}) of the referenced TiXmlElement \n
 * PRE:
 * REQUIRE(elem != NULL, "Referenced TiXmlElement was a NULLPTR");
 */
std::string getRowAndColumnStr(const TiXmlElement* elem);

std::string getRowAndColumnStr(const TiXmlNode* node);

std::string getRowAndColumnStr(const TiXmlText* text);

std::string convertPartialNumbersToNATOAlphabet(const std::string& string);

std::string convertToNATOAlphabet(const std::string& string);

bool isRunwayCompatible(const Airplane* airplane, const Runway* runway);

template<typename AType, typename BType>
bool mapContains(std::map<AType, BType> map, AType key, BType element)
{
    if (map.find(key) != map.end()) {
        return map.at(key) == element;
    } else {
        return false;
    }
}

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