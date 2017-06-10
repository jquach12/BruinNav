//
//  support.cpp
//  BruinNav
//
//  Created by Jonathan Quach on 3/11/17.
//  Copyright © 2017 QuachLabs. All rights reserved.
//

#include "provided.h"
#include "support.h"
#include <cmath>
//operator < for GeoCoords
bool operator<(const GeoCoord& a, const GeoCoord& b){
    
    if (a.latitude == b.latitude)
        return a.longitude < b.longitude;
    
    return a.latitude < b.latitude;
}

bool operator==(const GeoCoord& a, const GeoCoord& b){
    return a.latitude == b.latitude && a.longitude == b.longitude;
}


//operator > for GeoCoords
bool operator>(const GeoCoord& a, const GeoCoord& b){
    
    if (a.latitude == b.latitude)
        return a.longitude > b.longitude;
    
    return a.latitude > b.latitude;
}

bool operator !=(const GeoCoord& a, const GeoCoord& b){
    
    return a.latitude != b.latitude || a.longitude != b.longitude;
}




bool operator<(const Attraction& a, const Attraction& b){
    
    return a.name < b.name;
}

bool operator==(const Attraction& a, const Attraction& b){
    return a.name == b.name;
}


//operator > for GeoCoords
bool operator>(const Attraction& a, const Attraction& b){
    return a.name > b.name;
}




bool operator==(const StreetSegment& a, const StreetSegment& b){
    
    if (a.segment.start == b.segment.start && a.segment.end == b.segment.end && a.streetName == b.streetName)
        return true;
    
    return false;
}

bool operator!=(const StreetSegment& a, const StreetSegment& b){
    
    if (a.segment.start != b.segment.start || a.segment.end != b.segment.end || a.streetName != b.streetName)
        return true;
    
    return false;
}

bool operator>(const GeoSegment& a, const GeoSegment& b){
    return a.start.latitude > b.start.latitude;
}

bool operator<(const GeoSegment& a, const GeoSegment& b){
    return a.start.latitude < b.start.latitude;
}


bool operator<(const StreetSegment& a, const StreetSegment& b){
    if (a.segment.start.latitude == b.segment.start.latitude)
        return a.segment.start.longitude < b.segment.start.longitude;
    
    return a.segment.start.latitude < b.segment.start.longitude;
}
bool operator>(const StreetSegment& a, const StreetSegment& b){
    if (a.segment.start.latitude == b.segment.start.latitude)
        return a.segment.start.longitude > b.segment.start.longitude;
    
    return a.segment.start.latitude > b.segment.start.longitude;

}




bool operator==(const GeoSegment& a, const GeoSegment& b){
    return a.start == b.start && a.end == b.end;
}

bool operator!=(const GeoSegment& a, const GeoSegment& b){
    return a.start != b.start || a.end != b.end;
}




std::string printGeoCoord(const GeoCoord& a){
    
    return a.latitudeText + ',' + a.longitudeText;
}



bool isInVec(const std::vector<StreetSegment> &vec, StreetSegment s){
    for (int i = 0; i < vec.size(); i++)
        if(vec[i] == s)
            return true;
    
    return false;
}



std::string direction(double d){
    
    if (d >= 0 && d <= 22.5)
        return "east";
    
    else if (d > 22.5 && d <= 67.5)
        return "northeast";
    
    else if (d > 67.5 && d <= 112.5)
        return "north";
    
    else if (d > 112.5 && d <= 157.5)
        return "northwest";
    
    else if (d > 157.5 && d <= 202.5)
        return "west";
    
    else if (d > 202.5 && d <= 247.5)
        return "southwest";
    
    else if (d > 247.5 && d <= 292.5)
        return "south";
    
    else if (d > 292.5 && d < 337.5)
        return "southeast";
    
    else
        return "east";
    
}





