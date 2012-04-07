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

struct ShadowShape{
    ofPolyline  contour;
    ofPolyline  hole;
    bool        haveHole;
};

class AnimatedShadow {
public:
    AnimatedShadow(){
        currentFrame = 0;
        
        bHand = false;
        bActive = false;
        nId = 0;
    };
    
    AnimatedShadow(int _nId){
        currentFrame = 0;
        bHand = false;
        bActive = false;
        nId = _nId;
    };
    
    void    addFrame( ofxBlob &_blob, float _widht, float _height ){
        
        if (nId == 0 ){
            nId = _blob.id;
        }
        
        if (nId == _blob.id){
            
            if ( _blob.nFingers > 0 ){
                bHand = true;
            }
                
            ofPolyline contourLine;
        
            for (int i = 0; i < _blob.pts.size(); i++){
                contourLine.addVertex(_blob.pts[i].x * _widht, _blob.pts[i].y * _height);
            }
        
            if ( contourLine.size() > 0){
                contourLine.setClosed(true);  
                contourLine.simplify(1);
        
                ShadowShape newShape;
                newShape.contour = contourLine.getSmoothed(1,1);
                newShape.haveHole = false;
                
                shapes.push_back( newShape );
            }
            
            bActive = false;
        }
    }
    
    void    insertHole( ofPolyline &holeContourLine){
        if ((holeContourLine.size() > 0) &&
            (shapes.size() > 0)){
            holeContourLine.setClosed(true);    
            holeContourLine.simplify(1);
            cout << " Adding hole!!" << endl;
            shapes[currentFrame].hole = holeContourLine.getSmoothed(1,1);
            shapes[currentFrame].haveHole = true;
        }
    }
    
    int     getId() const { return nId; };
    bool    isInside( ofPoint &_centroid ) { if (size() > 0) return shapes[size()-1].contour.inside(_centroid); };
    bool    isHand() const { return bHand; };
    
    void    clear() { shapes.clear();};
    int     size() const { return shapes.size(); };
    ShadowShape& operator[](int _n){ if ( (_n >= 0) && (_n < shapes.size()) ) return shapes[_n]; };

    bool    draw(){
        bool finish = true;
        if ( bActive ){
            int nPoints = shapes[currentFrame].contour.getVertices().size();
            
            ofPushStyle();
            ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
            ofSetColor(255-sin( (currentFrame/shapes.size() ) * PI ) * 255,255);
            ofBeginShape();
            for (int i = 0; i < nPoints; i++){
                ofVertex( shapes[currentFrame].contour[i] );
            }
            ofEndShape(true);
            
            if ( shapes[currentFrame].haveHole ){
                nPoints = shapes[currentFrame].hole.getVertices().size();
                ofSetColor(255,255);
                ofBeginShape();
                for (int i = 0; i < nPoints; i++){
                    ofVertex( shapes[currentFrame].hole[i] );
                }
                ofEndShape(true);
            }
            
            ofDisableBlendMode();
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            ofPopStyle();
            
            currentFrame += 0.4;
            
            if ((int)(currentFrame) >= (shapes.size() - 1))
                currentFrame = 0;
            else 
                finish = false;
        }
        return finish;
    };
    
    bool    bActive;
    
private:
    vector<ShadowShape> shapes;
    float               currentFrame;
    int                 nId;
    bool                bHand;
};

#endif
