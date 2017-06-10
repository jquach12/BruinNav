#include "provided.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>


using namespace std;

class MapLoaderImpl
{
public:
    MapLoaderImpl();
    ~MapLoaderImpl();
    bool load(string mapFile);
    size_t getNumSegments() const;
    bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
    vector<StreetSegment> m_streetSegs;
    
};

MapLoaderImpl::MapLoaderImpl()
{
    //cerr<<"MapLoaderImpl [C]"<<endl;
}

MapLoaderImpl::~MapLoaderImpl()
{
    // cerr<<"MapLoaderImpl [D]"<<endl;
}



/* Your load() method must load all of the data from the specified map data file into a container of StreetSegments (you may use a dynamic array or the STL vector class to hold your data). You’ll need to ensure that you’ve loaded each and every street segment from the file, each into its own StreetSegment variable.
 */
bool MapLoaderImpl::load(string mapFile)
{
    
    ifstream someMapFile;
    
    someMapFile.open(mapFile);
    
    if(someMapFile.is_open() == false){
        cerr<<"Error Reading in file!"<<endl;
        return false;
    }
    
    int count = 0;
    
    cerr<<"File successfully read!"<<endl;
    
    
    vector<string> allLines;
    string someLine;
    
    while(getline(someMapFile,someLine)){
        
        allLines.push_back(someLine);
        count++;
        
    }
    //   cerr<<count<<endl;
    //  cerr<<m_streetSegs.size()<<endl;
    
    
    //O(N + N) -> O(N)
    
    //parse through all the lines
    for(int i = 0; i < allLines.size();){
        
        StreetSegment tempStreetSeg;
        
        
        //get the name of the street
        tempStreetSeg.streetName = allLines[i];
        
        //    cerr<<"This is a street name: "<<tempStreetSeg.streetName<<endl;
        ++i;
        
        //after every street name are the starting lats/longs and ending lats/longs
        string init_lat;
        int k = 0;
        for (; k < allLines[i].size(); k++){
            if(allLines[i][k] == ',')
                break;
            else
                init_lat+=allLines[i][k];
        }
        
        //  cerr<<"This is some initial latitude for "<<tempStreetSeg.streetName<<": "<<init_lat<<endl;
        
        //at this point, k is the position of some comma, and k+1 would be a space character
        k++;
        if(isspace(allLines[i][k]))
            k++;
        
        string init_long;
        for(;k < allLines[i].size();k++){
            if(allLines[i][k] == ' ')
                break;
            else
                init_long += allLines[i][k];
        }
        //  cerr<<"This is some initial longitude for "<<tempStreetSeg.streetName<<": "<<init_long<<endl;
        
        
        //at this point, k is the position of some space character
        ++k;
        
        string end_lat;
        for (; k < allLines[i].size(); k++){
            if(allLines[i][k] == ',')
                break;
            else
                end_lat+=allLines[i][k];
        }
        
        
        
        
        //A pair of geocoordinates will be separated by one space.
        
        //  cerr<<"This is the ending latitude for "<<tempStreetSeg.streetName<<": "<<end_lat<<endl;
        
        //3-16 LOL JK
        //NOTE: THE INITIAL GEO_COORD HAS AN EXTRA SPACE CHARACTER THAT THE ENDING GEO_COORD DOES NOT HAVE. OBSERVE THE DIFFERENCE
        //34.0709818, -118.4447589
        //34.0709897,-118.4449837
        
        //at this point, k is on another comma
        ++k;
        if(isspace(allLines[i][k]))
            ++k;
        
        string end_long;
        for(;k < allLines[i].size();k++)
            end_long+=allLines[i][k];
        
        
        //     cerr<<"This is some ending longitude for<<"<<tempStreetSeg.streetName<<": "<<end_long<<endl;
        
        GeoCoord startingPoint(init_lat,init_long);
        GeoCoord endingPoint(end_lat,end_long);
        
        GeoSegment someSegment(startingPoint, endingPoint);
        
        tempStreetSeg.segment = someSegment;
        
        ++i;
        
        //after every GeoSegment are the number of attractions
        int num_attractions = stoi(allLines[i]);
        
        
        ++i;
        
        //after number of attractions are specified, the next n lines are the lines of attractions
        
        
        
        
        /*Bruin Walk
         34.0709897, -118.4449837 34.0709835,-118.4460503
         4
         Bruin Plaza|34.0709897, -118.4449837
         John Wooden Center|34.0720093, -118.4458886
         West Alumni Center|34.0701358, -118.4457751
         Wooden West|34.0711037, -118.4458100
         */
        
        //    cerr<<"The number of attactions that "<<tempStreetSeg.streetName<<" has is "<<num_attractions<<endl;
        
        
        
        for (int n = 0; n < num_attractions ; n++,i++){
            
            int q = 0;
            string someAttractionName;
            
            while( allLines[i][q] != '|'){
                someAttractionName += allLines[i][q];
                ++q;
            }
            
            //  cerr<<"This is the name of an attraction that belongs to "<<tempStreetSeg.streetName<<": "<<someAttractionName<<endl;
            
            //at this point, q is on a |
            ++q;
            
            string someLatForAttraction;
            
            while( allLines[i][q] != ','){
                someLatForAttraction += allLines[i][q];
                ++q;
            }
            
            //  cerr<<"This is the latitude for "<<someAttractionName<<": "<<someLatForAttraction<<endl;
            //at this point, q is on a comma, and q + 1 is actually a space character
            
            //!!!!
            //q += 2;
            
            q++;
            if(isspace(allLines[i][q]))
                q++;
            
            string someLongForAttraction;
            while( q != allLines[i].size() ){
                someLongForAttraction += allLines[i][q];
                ++q;
            }
            
            //  cerr<<"This is the longitude for "<<someAttractionName<<": "<<someLongForAttraction<<endl;
            
            
            GeoCoord coordForAttraction(someLatForAttraction,someLongForAttraction);
            
            //at this point, you should have parsed through the name and coordinate of the attraction
            Attraction someAttraction;
            someAttraction.name = someAttractionName;
            someAttraction.geocoordinates = coordForAttraction;
            
            
            
            tempStreetSeg.attractions.push_back(someAttraction);
            
        }
        
        
        m_streetSegs.push_back(tempStreetSeg);
        
        
    }
    
    cerr<<"This is the amount of street segments you have: "<<m_streetSegs.size()<<endl;
    
    /*
     for(int i = 0; i < m_streetSegs.size();i++){
     cerr<<"Street Segment ["<<i<<"] is "<<m_streetSegs[i].streetName<<endl;
     }
     */
    
    return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
    //even if the file never loaded, this would still be size zero :)
    return m_streetSegs.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    //cannot get a segNum for a negative position in the vector, nor we can get a segNum for a position >= vector size
    //Apparently, size_t is never negative, so we do not need to worry about segNum < 0
    if (segNum >= m_streetSegs.size())
        return false;
    
    //at this point, segNum is within the bounds of the vector
    seg = m_streetSegs[segNum];
    return true;
    
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
    m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
    delete m_impl;
}

bool MapLoader::load(string mapFile)
{
    return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
    return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
    return m_impl->getSegment(segNum, seg);
}
