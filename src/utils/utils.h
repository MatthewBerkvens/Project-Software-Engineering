#ifndef INC_UTILS_H
#define INC_UTILS_H
#include <string>
#include <vector>
#include <algorithm>

class Runway;
typedef std::vector<Runway*> RunwayVector;
typedef std::vector<Runway*>::iterator RunwayVectorIterator;
typedef std::vector<Runway*>::const_iterator RunwayVectorConstIterator;

class Airport;
typedef std::vector<Airport*> AirportVector;
typedef std::vector<Airport*>::iterator AirportVectorIterator;
typedef std::vector<Airport*>::const_iterator AirportVectorConstIterator;

bool isString(const std::string& str);

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