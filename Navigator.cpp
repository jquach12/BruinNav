#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

// 9:16 pm 3-15-17, if you ever opened this file, i hope you are feeling a lot better than how you're feeling at this very moment
//and well, i hope you forgive yourself. It's just an assignment. Life moves on lol.
class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    AttractionMapper m_am;     //MAPS  MyMap<string, GeoCoord> m_lowerCaseNameOfAttraction2coord;
    SegmentMapper m_sm;        //MAPS MyMap<GeoCoord, vector <StreetSegment> > m_geoCoord2StreetSegments;
    MapLoader m_ml;
    
    
    
    //MyMap<Attraction,StreetSegment> m_attraction2StreetSegment;
   // MyMap<string,StreetSegment> m_nameOfAttraction2StreetSegment;
    
    
    struct Node{
        
        GeoCoord geoCoord;
        double distanceAwayFromDestination = 9999;
        vector<StreetSegment> neighboringStreetSegments;
        
        double cost;
        
        
    };
    
    //hmm > or < ...
    
    /*
     struct compareNodes{
     bool operator() ( Node* a,  Node* b){
     return a->distanceAwayFromDestination > b->distanceAwayFromDestination;
     }
     };
     */
    
    
    
    
    struct NodeStreet{
        StreetSegment streetSeg;
        double distanceAwayFromDestination = 99999;
        
        double cost;
        
        NodeStreet* parent = nullptr;
        
        bool operator< (NodeStreet* other){
            return this->distanceAwayFromDestination > other->distanceAwayFromDestination;
        }
        
    };
    
    struct compareNodeStreets{
        bool operator() ( NodeStreet* a, NodeStreet* b){
            return (a->cost ) > (b->cost);
        }
    };
    
    struct comparePair{
        bool operator() (const pair<GeoCoord, double>& a,const pair<GeoCoord, double>&b ){
            return  a.second > b.second;
        }
    };
    
    
    
    struct geoVisit{
        GeoCoord g;
        bool visit = false;
    };
    
    
    //some reason [] doesn't like objects
    
    vector<Node*> listOfNodes;
    
    vector<NodeStreet*> listOfNodeStreets;
    
   
    vector<StreetSegment> listOfStreetSegments;
    vector<GeoCoord> listOfStartingCoords;
    
    
   
    
    // vector<Node*> yee;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
    for (int i = 0; i < listOfNodes.size();i++)
        delete listOfNodes[i];
    
    for(int i =0; i < listOfNodeStreets.size();i++)
        delete listOfNodeStreets[i];
}


//THIS IS NOT SUBJECT TO A TIME COMPLEXITY WOOOOOOO
bool NavigatorImpl::loadMapData(string mapFile)
{
    
    if(! m_ml.load(mapFile) ){
        cerr<<"Error loading file! Return false :("<<endl;
        return false;
    }
    cerr.precision(12);
    
    //at this point, m_ml has a vector full of street segments
    
    m_am.init(m_ml); //so this guy maps strings to geoCoord  "dykStra hAll" -> (34.0701, -118.45)
    
    m_sm.init(m_ml); //and this guy maps a geoCoord to a vector of street segments it is associated with
    
    //so this gets all the coordinates
    for(int i = 0; i < m_ml.getNumSegments(); i++){
        StreetSegment ss;
        if (m_ml.getSegment(i, ss) ){
            listOfStartingCoords.push_back(ss.segment.start);
            listOfStreetSegments.push_back(ss);
            
        }
        
        
        
        
    }
        cerr<<"HELLO"<<endl;
    
    /* prints all geocoords
     for (int i = 0; i < listOfStreetSegments.size();i++){
     cerr<<printGeoCoord(listOfStreetSegments[i].segment.start)<<" "<<printGeoCoord(listOfStreetSegments[i].segment.end)<<endl;
     }*/
    
    cerr<<"Size of streetseck"<<listOfStreetSegments.size()<<endl;
    
    //this gets all Nodes
    for (int k = 0; k <  listOfStartingCoords.size(); k++){
        
        Node* someCoord = new Node;
        someCoord->geoCoord = listOfStartingCoords[k];
        
        someCoord->neighboringStreetSegments = m_sm.getSegments(listOfStartingCoords[k]);
        
        listOfNodes.push_back(someCoord);
        
        
    }
    
    
    //this gets all NodeStreets
    for (int k = 0; k <  listOfStreetSegments.size(); k++){
        
        NodeStreet* someNS = new NodeStreet;
        someNS->streetSeg = listOfStreetSegments[k];
        
        someNS->cost = distanceEarthMiles(someNS->streetSeg.segment.start, someNS->streetSeg.segment.end);
        listOfNodeStreets.push_back(someNS);
    }
    
    
    /*
     // seems to work!
     for(int i = 0; i < listOfNodeStreets.size();i++){
     
     cerr<<printGeoCoord(listOfNodeStreets[i]->streetSeg.segment.start)<<", "<<printGeoCoord(listOfNodeStreets[i]->streetSeg.segment.end)<<endl;
     }*/
     
    
    for(auto itr : listOfNodes){
        // cerr<<"This is a coordinate: "<<itr->geoCoord.latitude<<","<<itr->geoCoord.longitude<<endl<<endl;
        // cerr<<"For this coordinate it has "<<itr->neighboringStreetSegments.size()<<" associations:"<<endl;
        
        for(int i = 0; i < itr->neighboringStreetSegments.size();i++){
            //     cerr<<itr->neighboringStreetSegments[i].streetName<<" is at "<<itr->neighboringStreetSegments[i].segment.start.latitude<<","<<itr->neighboringStreetSegments[i].segment.start.longitude<<endl<<endl;
        }
    }
    
    /*
     for(int j = 0; j < listOfStartingCoords.size(); j++){
     cerr<<"Lat: "<<listOfStartingCoords[j].latitude<<" Long: "<<listOfStartingCoords[j].longitude<<endl;
     }
     cerr<<listOfStartingCoords.size()<<endl;
     */
    return true;  // This compiles, but may not be correct
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    
    
    // start and end are names of places, case sensitive it looks
    
    GeoCoord startingLocation;
    if (! m_am.getGeoCoord(start, startingLocation) ){
        cerr<<"Bad starting location!"<<endl;
        
        return NAV_BAD_SOURCE;
    }
    
    GeoCoord destination;
    if ( ! m_am.getGeoCoord(end, destination) ){
        cerr<<"Bad destination!"<<endl;
        
        return NAV_BAD_DESTINATION;
    }
    
    
    if (startingLocation==destination){
        vector<NavSegment> empty;
        directions = empty;
        return NAV_SUCCESS;
    }
    
  

    
    
    cerr<<"Size of listOfStreeSegs: "<<listOfStreetSegments.size()<<endl;
  
    //FIRST CHECK IF START AND END ARE ON SAME STREET SEGMENT
    
    
    bool inCommon = false;
    vector<StreetSegment> singleStart = m_sm.getSegments(startingLocation);
    vector<StreetSegment> singleDestination = m_sm.getSegments(destination);
    
    for(int i = 0; i < singleStart.size();i++){
        
        for(int j = 0; j < singleDestination.size(); j++){
            if (singleDestination[j] == singleStart[i])
                inCommon = true;
        }
    }
    
   
    if(inCommon){
        vector<NavSegment> singleton;
        
        NavSegment single;
        single.m_geoSegment.start = startingLocation;
        single.m_geoSegment.end = destination;
        single.m_streetName = singleStart[0].streetName;
        single.m_direction = direction(angleOfLine(single.m_geoSegment));
        single.m_command= NavSegment::PROCEED;
    
        singleton.push_back(single);
        
        directions = singleton;
        return NAV_SUCCESS;
    }
    
    
    
    

    
    vector<StreetSegment> d = m_sm.getSegments(destination);
    
    
    
    vector<StreetSegment> s = m_sm.getSegments(startingLocation);
    
    if(s.empty()){
        cerr<<"How tho"<<endl;
        return NAV_NO_ROUTE;
    }
    
    
    vector<StreetSegment> destination_streetsegs = m_sm.getSegments(destination);
    
    
  

    
    
    typedef pair<GeoCoord, double> gc2weight;
    
    
    
    priority_queue<int> ipq;
    
    priority_queue<gc2weight, vector <gc2weight> , comparePair > pq;
    
    
    GeoCoord TARGET1 = destination_streetsegs[0].segment.start;
    GeoCoord TARGET2 = destination_streetsegs[0].segment.end;
    
    
    
    vector<pair<GeoCoord, double>> listOfGs2Ws;
    
    for(int i = 0; i < listOfStartingCoords.size();i++)
        listOfGs2Ws.push_back(make_pair(listOfStartingCoords[i],99999));
    
    
    
    //starting point is 0
    for(int i = 0; i < listOfGs2Ws.size();i++){
        if(listOfGs2Ws[i].first == s[0].segment.start)
            listOfGs2Ws[i].second = 0;
    }
    
    
    

    
    
    
    
    pq.push(make_pair(s[0].segment.start,distanceEarthMiles(s[0].segment.start, startingLocation)));
    pq.push(make_pair(s[0].segment.end, distanceEarthMiles(s[0].segment.end, startingLocation)));
    //pq.push(make_pair(s[0].segment.end, 123));
    
    
    //cerr<<pq.top().second<<endl;
    
    
    vector<pair<GeoCoord,double>> adjacentPoints;
    
    
    
    
    MyMap<GeoCoord, double> g2weight;
    MyMap<GeoCoord, bool> g2visit;
    
    for(int i = 0; i < listOfGs2Ws.size();i++)
        g2visit.associate(listOfGs2Ws[i].first, false);
    
    
    
    vector<pair<GeoCoord, double>> visited;
    
    double distanceThusFar = 0;
    while(pq.empty() == false){
        
        
     
        GeoCoord testPoint = pq.top().first;
        double testWeight = pq.top().second;
    
        
        
        
        double currDistanceAwayFromGoal = distanceEarthMiles(testPoint, TARGET1);

        
        cerr<<"Examining: "<<testPoint.latitude<<", "<<testPoint.longitude<<endl;
                cerr<<"Distance away from destination is: "<<currDistanceAwayFromGoal<<endl;
        
    
        
        distanceThusFar+=testWeight;
        
        //cerr<<"Distance traveled so far is "<<distanceThusFar<<endl;
        
         visited.push_back(pq.top());
        
        cerr<<"Pushed something in visited: "<<visited.size()<<endl;
        pq.pop();
        g2visit.associate(testPoint, true);
       
        double somePrevPathDistance = testWeight - distanceEarthMiles(testPoint, destination);
        
        
        if(testPoint == TARGET1 || testPoint == TARGET2){
            cerr<<"success!"<<endl;
            
            
            for(int i = 0; i < visited.size();i++){
                cerr<<visited[i].first.latitude<<", "<<visited[i].first.longitude<<endl;
            }
            
            
            vector<GeoSegment> connectors;
            for(int i = 1; i < visited.size();i++){
               
                GeoSegment gs;
                gs.start = visited[i-1].first;
                gs.end = visited[i].first;
                
                connectors.push_back(gs);
            }
            cerr<<"visit"<<visited.size()<<endl;
            
            cerr<<connectors.size()<<endl;
            
         
            
            vector<NavSegment> route;
            
            
        
            
            //making the first NavSegment
            NavSegment ns1;
            ns1.m_command= NavSegment::PROCEED;
            ns1.m_geoSegment.start = startingLocation;
            ns1.m_geoSegment.end = connectors[1].start;
            ns1.m_streetName = s[0].streetName;
            ns1.m_direction=direction(angleOfLine(ns1.m_geoSegment));
            ns1.m_distance = distanceEarthMiles(ns1.m_geoSegment.start, ns1.m_geoSegment.end);
            route.push_back(ns1);
            
            
            for(int i = 1; i < connectors.size();i++){
                
                NavSegment nst;
                nst.m_command=NavSegment::PROCEED;
                nst.m_geoSegment = connectors[i];
                nst.m_streetName = "";
                
                vector<StreetSegment> cand = m_sm.getSegments(nst.m_geoSegment.start);
                for(int i = 0; i < cand.size();i++){
                    if(cand[i].segment.start==nst.m_geoSegment.start && cand[i].segment.end == cand[i].segment.end)
                        nst.m_streetName=cand[i].streetName;
                }
                
                //yo turn foo
                if(route.back().m_streetName != nst.m_streetName){
                    
                    NavSegment turn;
                    turn.m_distance = 0;
                    turn.m_command=NavSegment::TURN;
                    turn.m_streetName = nst.m_streetName;
                    
                    double angleturn = angleBetween2Lines(route.back().m_geoSegment,nst.m_geoSegment);
                    if(angleturn < 180)
                        turn.m_direction = "left";
                    else
                        turn.m_direction = "right";
                    
                    route.push_back(turn);
                }
                
                
                nst.m_direction=direction(angleOfLine(nst.m_geoSegment));
                nst.m_distance = distanceEarthMiles(nst.m_geoSegment.start, nst.m_geoSegment.end);
                route.push_back(nst);
            }
            
            //last segment
            NavSegment ns2;
             ns2.m_command= NavSegment::PROCEED;
            ns2.m_geoSegment.start = route.back().m_geoSegment.end;
            ns2.m_geoSegment.end = destination;
            ns2.m_streetName = d[0].streetName;
            ns2.m_direction= direction(angleOfLine(ns2.m_geoSegment));
            ns2.m_distance = distanceEarthMiles(ns2.m_geoSegment.start, ns2.m_geoSegment.end);
            
            route.push_back(ns2);
            
            directions = route;
            
            return NAV_SUCCESS;
        }
        
        
        //the search continues...
        
        cerr<<endl;
        cerr<<"Testing weight: "<<testWeight<<endl;
        
        
        /*
      
        //so as to re-queue stuff
        while(pq.empty()==false)
            pq.pop();
        
         */
        vector<StreetSegment> someConnectingStreets = m_sm.getSegments(testPoint);
        
        if(someConnectingStreets.empty()){
            return NAV_NO_ROUTE;
        }
        
        
        //this connects all points and weighs them against current test point
        for(int i = 0 ; i < someConnectingStreets.size();i++){
            
           
    //distanceEarthMiles(someConnectingStreets[i].segment.end,TARGET1)
           
            cerr<<"looking at: "<<someConnectingStreets[i].streetName<<endl;
         
            if(testPoint==someConnectingStreets[i].segment.start )
                adjacentPoints.push_back(make_pair(someConnectingStreets[i].segment.end, testWeight + distanceEarthMiles(someConnectingStreets[i].segment.end,TARGET1)+distanceEarthMiles(someConnectingStreets[i].segment.end, testPoint)));
                                
            
        
            
            
             else if(testPoint==someConnectingStreets[i].segment.end )
                 adjacentPoints.push_back(make_pair(someConnectingStreets[i].segment.start,testWeight + distanceEarthMiles(someConnectingStreets[i].segment.start,TARGET1)+distanceEarthMiles(someConnectingStreets[i].segment.start, testPoint)));

        }
        
        
    
        for(int i = 0; i < adjacentPoints.size();i++){
            
            bool* hasVisit = g2visit.find(adjacentPoints[i].first);
            
            //!hasVisit || *hasVisit == false
            // && adjacentPoints[i].second < currDistanceAwayFromGoal
            if(!hasVisit || *hasVisit == false){
                
                cerr<<"Pushing: "<<adjacentPoints[i].first.latitude<<", "<<adjacentPoints[i].first.longitude<<endl;
                pq.push(adjacentPoints[i]);
                
                
           }
        }
        
        adjacentPoints.clear();
     
        
       
    }
    
    cerr<<"Bu hao"<<endl;
    
    return NAV_NO_ROUTE;  // This compiles, but may not be correct
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}

//cerr drake to john works 4:59 am
