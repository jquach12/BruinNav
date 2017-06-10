#include "provided.h"
#include <string>
#include "MyMap.h"
#include <iostream>
using namespace std;


class AttractionMapperImpl
{
public:
    AttractionMapperImpl();
    ~AttractionMapperImpl();
    void init(const MapLoader& ml);
    bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    MyMap<string, GeoCoord> m_lowerCaseNameOfAttraction2coord;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    //O(?)??  carey said this is okay
    for (int i = 0; i < ml.getNumSegments(); i++){
        StreetSegment ss;
        if ( ml.getSegment(i, ss) )
            
            for (int j = 0; j < ss.attractions.size(); j++){
                
                string attract_name = ss.attractions[j].name;
                string lowerCase;
                for (int k = 0; k < attract_name.size();k++)
                    lowerCase+=tolower(attract_name[k]);
                
                
                m_lowerCaseNameOfAttraction2coord.associate(lowerCase, ss.attractions[j].geocoordinates);
            }
        else{
            cerr<<"Couldnt get segment at: "<<i<<endl; //was actually never ran
        }
        
    }
    
    cerr<<"Attraction mapper is of size :"<<m_lowerCaseNameOfAttraction2coord.size()<<endl;
    for(int i = 0; i < m_lowerCaseNameOfAttraction2coord.size();i++){
        
    }
    
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    //convert this to lower case this is timely acceptable as this is a constant coefficient
    for (int i = 0; i < attraction.size();i++)
        attraction[i] = tolower(attraction[i]);
    
    const GeoCoord* someGeoPtr = m_lowerCaseNameOfAttraction2coord.find(attraction);
    if ( someGeoPtr!= nullptr){
        cerr<<"Found a value!"<<endl;
        gc = *someGeoPtr;
        
        cerr<<"The coordinates for "<<attraction<<" are: "<<endl;
        cerr<<"Latitude: "<<gc.latitude<<" , "<<gc.longitude<<endl;
        
        
        
        
        return true;
    }
    
    
    cerr<<"Value not found :("<<endl;
    
    return false;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
    m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
    delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
    m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
    return m_impl->getGeoCoord(attraction, gc);
}
