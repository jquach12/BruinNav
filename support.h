//
//  support.h
//  BruinNav
//
//  Created by Jonathan Quach on 3/11/17.
//  Copyright © 2017 QuachLabs. All rights reserved.
//


#ifndef support_h
#define support_h

#include <string>
#include <vector>
class GeoCoord;
class Attraction;
class StreetSegment;
class GeoSegment;

class simpleStreet;
#include "provided.h"

bool operator<(const GeoCoord& a, const GeoCoord& b);
bool operator==(const GeoCoord& a, const GeoCoord& b);
bool operator>(const GeoCoord& a, const GeoCoord& b);
bool operator!=(const GeoCoord& a, const GeoCoord& b);



bool operator<(const Attraction& a, const Attraction& b);
bool operator==(const Attraction& a, const Attraction& b);
bool operator>(const Attraction& a, const Attraction& b);


bool operator==(const StreetSegment& a, const StreetSegment& b);
bool operator!=(const StreetSegment& a, const StreetSegment& b);
bool operator<(const StreetSegment& a, const StreetSegment& b);

bool operator>(const StreetSegment& a, const StreetSegment& b);


bool operator==(const GeoSegment& a, const GeoSegment& b);
bool operator!=(const GeoSegment& a, const GeoSegment& b);
bool operator>(const GeoSegment& a, const GeoSegment& b);
bool operator<(const GeoSegment& a, const GeoSegment& b);







bool isInVec(const std::vector<StreetSegment> &vec, StreetSegment s);



std::string printGeoCoord(const GeoCoord& a);

std::string direction(double d);

const int NUM_SEGS = 20000;





#endif /* support_h */
