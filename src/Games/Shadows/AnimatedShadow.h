//
//  Shadow.h
//  mdt-Core
//
//  Created by Patricio González Vivo on 4/6/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef SHADOWANIMATION
#define SHADOWANIMATION

#include "ofMain.h"
#include "ofxBlob.h"

class AnimatedShadow {
public:
    AnimatedShadow(){
        currentFrame = 0;
        nId = 0;
        bHand = false;
        bActive = false;
    };
    
    void    addFrame( ofxBlob &_blob, float _widht, float _height ){
        
        if (nId == 0 ){
            nId = _blob.id;
        }
        
        if (nId == _blob.id){
            
            if ( _blob.nFingers >= 3 ){
                bHand = true;
            }
                
            ofPolyline contourLine;
        
            for (int i = 0; i < _blob.pts.size(); i++){
                contourLine.addVertex(_blob.pts[i].x * _widht, _blob.pts[i].y * _height);
            }
        
            if ( contourLine.size() > 0){
                contourLine.setClosed( true );
                contourLine.simplify(1);
        
                contours.push_back( contourLine.getSmoothed(1,1) );
            }
            
            bActive = false;
        }
    }
    
    int     getId() const { return nId; };
    bool    isHand()const { return bHand; };
    void    clear() { contours.clear();};
    int     size() const { return contours.size(); };
    ofPolyline& operator[](int _n){ if ( (_n >= 0) && (_n < contours.size()) ) return contours[_n]; };

    void    draw(){
        if ((contours.size() > 0) && bActive){
            int nPoints = contours[currentFrame].getVertices().size();
            ofPushStyle();
            
            ofSetColor(0,55+sin(currentFrame/nPoints)*200);
            ofBeginShape();
            for (int i = 0; i < nPoints; i++){
                ofVertex( contours[currentFrame][i] );
            }
            ofEndShape(true);
            
            currentFrame += 0.4;
            
            if ((int)(currentFrame) >= (contours.size() - 1)) 
                currentFrame = 0;
            
            ofPopStyle();
        }
    };
    
    bool    bActive;
    
private:
    vector<ofPolyline>  contours;
    float               currentFrame;
    int                 nId;
    bool                bHand;
};

#endif
