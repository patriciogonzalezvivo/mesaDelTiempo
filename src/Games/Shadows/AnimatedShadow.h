//
//  AnimatedShadow.h
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#ifndef SHADOWANIMATION
#define SHADOWANIMATION

#include "ofMain.h"
#include "ofxBlob.h"

struct ShadowShape{
    ofPolyline  contour;    // Contour shape of the Shadow
    ofPolyline  hole;       // Contour of the hole
    float       time;
    bool        haveHole;   // if it have a hole or not
};

class AnimatedShadow {
public:
    
    AnimatedShadow();
    AnimatedShadow(int _nId);

    int     getId() const { return nId; };
    int     getCurrentFrame() const { return currentFrame; };
    float   getPosition() const {
        if ( size() >= 2){
            float total = shapes[size()-1].time;
            return currentTime/total;
        }
    };
    
    int     size() const { return shapes.size(); };
    ShadowShape& operator[](int _n){ if ( (_n >= 0) && (_n < shapes.size()) ) return shapes[_n]; };
    bool    isInside( ofPoint &_centroid ) { if (size() > 0) return shapes[size()-1].contour.inside(_centroid); };
    bool    isHand() const { return bHand; };
    
    void    addFrame( ofxBlob &_blob, float _widht, float _height );
    void    addFrame( ofPolyline &_contourLine, int _nFingers);
    void    insertHole( ofPolyline &holeContourLine);
    void    clear() { shapes.clear();};
    
    bool    draw();
    
    bool    bActive;
    
private:
    vector<ShadowShape> shapes;
    int     currentFrame;
    float   currentTime;
    float   startTime;
    int     nId;
    bool    bHand;
};

#endif
