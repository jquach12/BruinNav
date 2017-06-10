#include "provided.h"
#include "MyMap.h"
#include <vector>
using namespace std;

#include "support.h"
#include <iostream>
class SegmentMapperImpl
{
public:
    SegmentMapperImpl();
    ~SegmentMapperImpl();
    void init(const MapLoader& ml);
    vector<StreetSegment> getSegments(const GeoCoord& gc) const;
    
private:
    //  MyMap<GeoCoord,StreetSegment> m_geoCoord2StreetSegment;
    // MyMap<GeoCoord,Attraction> m_geoCoord2Attraction;
    
    
    MyMap<GeoCoord, vector <StreetSegment> > m_geoCoord2StreetSegments;
    
    //vector<GeoCoord> m_listOfGeocoords;
};

SegmentMapperImpl::SegmentMapperImpl()
{
    
    
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    //for every segment
    for (int i = 0; i < ml.getNumSegments(); i++){
        
        StreetSegment ss;
        
        if ( ml.getSegment(i, ss)){
            
            GeoCoord someStart = ss.segment.start;
            GeoCoord someEnd = ss.segment.end;
            
            vector<StreetSegment> vecForStart;
            vector<StreetSegment> vecForEnd;
            
            vecForStart.push_back(ss);
            vecForEnd.push_back(ss);
            
            
            vector<GeoCoord> vecForAttractionCoord;
            for (int j = 0; j < ss.attractions.size(); j++ ){
                vecForAttractionCoord.push_back(ss.attractions[j].geocoordinates);
                //cerr<<"Pushing "<<ss.attractions[j].geocoordinates.latitude<<","<<ss.attractions[j].geocoordinates.longitude<<endl;
                //alright this seems to work too
            }
            
            vector<StreetSegment>* finderStart = m_geoCoord2StreetSegments.find(someStart);
            
            //this means that this will be a unique (NEW!) key to the map
            if(finderStart == nullptr){
                m_geoCoord2StreetSegments.associate(someStart, vecForStart);
            }
            
            
            //this means this key is already in the tree, and already has a vector of associated things THAT WE DO NOT WANNA STRAIGHT UP OVERWRITE
            else{
                
                //add all the associated stuff to the present vector
                for (int u = 0; u < vecForStart.size(); u++){
                    finderStart->push_back(vecForStart[u]);
                }
            }
            
            
            //Do the same for the ending geocoord too
            
            vector<StreetSegment>* finderEnd = m_geoCoord2StreetSegments.find(someEnd);
            
            if(finderEnd == nullptr){
                m_geoCoord2StreetSegments.associate(someEnd, vecForEnd);
            }
            
            else{
                //add all the associated stuff to the present vector
                for (int u = 0; u < vecForEnd.size(); u++){
                    finderEnd->push_back(vecForEnd[u]);
                }
            }
            
            
            //now do this for every geocoord in the attractions
            
            for(int g = 0; g < vecForAttractionCoord.size(); g++){
                
                vector<StreetSegment>* finderG = m_geoCoord2StreetSegments.find(vecForAttractionCoord[g]);
                
                vector<StreetSegment> someVecForAttract;
                someVecForAttract.push_back(ss);
                
                
                
                if(finderG == nullptr){
                    m_geoCoord2StreetSegments.associate(vecForAttractionCoord[g], someVecForAttract);
                }
                
                else{
                    //add all the associated stuff to the present vector
                    for (int u = 0; u < someVecForAttract.size(); u++){
                        finderG->push_back(someVecForAttract[u]);
                    }
                }
            }
            
        }
        
    }
    
    //so this is done in N + A time
    
    cerr<<"Segment mapper is of size :"<<m_geoCoord2StreetSegments.size()<<endl;
    
    /*
     for(int i = 0; i < m_listOfGeocoords.size();i++){
     cerr<<"Lat: "<<m_listOfGeocoords[i].latitude<<" , Long: "<<m_listOfGeocoords[i].longitude<<endl;
     }*/
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    
    const vector<StreetSegment>* somePtr = m_geoCoord2StreetSegments.find(gc);
    
    
    vector<StreetSegment> segments;
    
    if(somePtr != nullptr){
        segments = *somePtr;
        
    }
    
    //cerr<<"Wats up"<<endl;
    
    return segments;  // This compiles, but may not be correct
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
    m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
    delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
    m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
    return m_impl->getSegments(gc);
}
