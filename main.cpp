

//
//  main.cpp
//  jesusChrist
//
//  Created by Jonathan Quach on 3/15/17.
//  Copyright © 2017 QuachLabs. All rights reserved.
//
#include <vector>
#include <iostream>
#include "support.h"
#include "MyMap.h"
#include "provided.h"
#include <ctime>
using namespace std;
int main(int argc, const char * argv[]) {
    
    std::clock_t start;
    double duration;
    
    start = std::clock();
    
    /* Your algorithm here */
    
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    Navigator ns;
    
    ns.loadMapData("testmap.txt");
    
   // SegmentMapper s;
   // MapLoader ms;
   // ms.load("mapdata.txt");
    vector<NavSegment> lol;
    
    
    ns.navigate("dykstra hall","greystone park", lol);
    
    
    double distanceTraveled = 0;
    for(int i = 0; i < lol.size();i++){
        
        
        if(lol[i].m_command==NavSegment::TURN){
            cerr<<"type: "<<lol[i].m_command<<endl;
            cerr<<"direction: "<<lol[i].m_direction<<endl;
            cerr<<"street: "<<lol[i].m_streetName<<endl;
            
            
            cerr<<"start: "<<lol[i].m_geoSegment.start.latitude<<","<<lol[i].m_geoSegment.start.longitude<<endl;
            cerr<<"end: "<<lol[i].m_geoSegment.end.latitude<<","<<lol[i].m_geoSegment.end.longitude<<endl;
        }
        
        else{
            
            cerr<<"type: "<<lol[i].m_command<<endl;
            cerr<<"start: "<<lol[i].m_geoSegment.start.latitude<<","<<lol[i].m_geoSegment.start.longitude<<endl;
            
            cerr<<"start: "<<lol[i].m_geoSegment.start.latitudeText<<","<<lol[i].m_geoSegment.start.longitudeText<<" str"<<endl;
            
            cerr<<"end: "<<lol[i].m_geoSegment.end.latitude<<","<<lol[i].m_geoSegment.end.longitude<<endl;
            cerr<<"end: "<<lol[i].m_geoSegment.end.latitude<<","<<lol[i].m_geoSegment.end.longitude<<" str"<<endl;

            cerr<<"direction: "<<lol[i].m_direction<<endl;
            cerr<<"street: "<<lol[i].m_streetName<<endl;
            
            //cerr<<"Distance from this point to previous point is "<<distanceEarthMiles(lol[i].m_geoSegment.start, lol[i-1].m_geoSegment.start)<<endl;
            distanceTraveled+=lol[i].m_distance;
            cerr<<"Adding this distance: "<<lol[i].m_distance<<endl;
            
        }
        
        cerr<<endl;
        
        
    }
    cerr<<"Distance traveled: "<<distanceTraveled<<endl;
    
    clock_t end = clock();
    
    cerr<<(end-start)/(CLOCKS_PER_SEC/1000);
    
   // std::cout<<"printf: "<< duration <<'\n';
}




/*

#include "provided.h"
#include "MyMap.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <cassert>
using namespace std;

int main()
{
    cout << "About to test MyMap" << endl;
    {
        MyMap<int, string> mm;
        mm.associate(20, "Ethel");
        mm.associate(10, "Fred");
        const string* p = mm.find(10);
        assert(p != nullptr  &&  *p == "Fred");
        assert(mm.find(30) == nullptr);
        assert(mm.size() == 2);
    }
    cout << "MyMap PASSED" << endl;
    
    cout << "About to test MapLoader" << endl;
    {
        MapLoader ml;
        assert(ml.load("testmap.txt"));
        size_t numSegments = ml.getNumSegments();
        assert(numSegments == 7);
        bool foundAttraction = false;
        for (size_t i = 0; i < numSegments; i++)
        {
            StreetSegment seg;
            assert(ml.getSegment(i, seg));
            if (seg.streetName == "Picadilly")
            {
                assert(seg.attractions.size() == 1);
                assert(seg.attractions[0].name == "Eros Statue");
                foundAttraction = true;
                break;
            }
        }
        assert(foundAttraction);
    }
    cout << "MapLoader PASSED" << endl;
    
    cout << "About to test AttractionMapper" << endl;
    {
        MapLoader ml;
        assert(ml.load("testmap.txt"));
        AttractionMapper am;
        am.init(ml);
        GeoCoord gc;
        assert(am.getGeoCoord("Hamleys Toy Store", gc));
        assert(gc.latitudeText == "51.512812");
        assert(gc.longitudeText == "-0.140114");
    }
    cout << "AttractionMapper PASSED" << endl;
    
    cout << "About to test SegmentMapper" << endl;
    {
        MapLoader ml;
        assert(ml.load("testmap.txt"));
        SegmentMapper sm;
        sm.init(ml);
        GeoCoord gc("51.510087", "-0.134563");
        vector<StreetSegment> vss = sm.getSegments(gc);
        assert(vss.size() == 4);
        string names[4];
        for (size_t i = 0; i < 4; i++)
            names[i] = vss[i].streetName;
        sort(names, names+4);
        const string expected[4] = {
            "Coventry Street", "Picadilly", "Regent Street", "Shaftesbury Avenue"
        };
        assert(equal(names, names+4, expected));
    }
    cout << "SegmentMapper PASSED" << endl;
    
    cout << "About to test Navigator" << endl;
    {
        Navigator nav;
        assert(nav.loadMapData("testmap.txt"));
        vector<NavSegment> directions;
        assert(nav.navigate("Eros Statue", "Hamleys Toy Store", directions) == NAV_SUCCESS);
        
        
        for(int i = 0; i < directions.size();i++){
            cerr<<directions[i].m_geoSegment.start.latitudeText<<", "<<directions[i].m_geoSegment.start.longitudeText<<endl;
            cerr<<directions[i].m_geoSegment.end.latitudeText<<", "<<directions[i].m_geoSegment.end.longitudeText<<endl;
        }
        
        
        /*
        assert(directions.size() == 6);
        struct ExpectedItem
        {
            NavSegment::NavCommand command;
            string direction;
            double distance;
            string streetName;
        };
        const ExpectedItem expected[6] = {
            { NavSegment::PROCEED, "northwest", 0.0138, "Picadilly" },
            { NavSegment::TURN, "left", 0, "" },
            { NavSegment::PROCEED, "west", 0.0119, "Regent Street" },
            { NavSegment::PROCEED, "west", 0.0845, "Regent Street" },
            { NavSegment::PROCEED, "west", 0.0696, "Regent Street" },
            { NavSegment::PROCEED, "northwest", 0.1871, "Regent Street" },
        };
        
        
        for (size_t i = 0; i < 6; i++)
        {
            const NavSegment& ns = directions[i];
            const ExpectedItem& exp = expected[i];
            assert(ns.m_command == exp.command);
            assert(ns.m_direction == exp.direction);
            if (ns.m_command == NavSegment::PROCEED)
            {
               // assert(abs(ns.m_distance - exp.distance) < 0.00051);
                assert(ns.m_streetName == exp.streetName);
            }
        }
    }
    }
    cout << "Navigator PASSED" << endl;
}*/
