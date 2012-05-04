//
//  AnimatedShadow.cpp
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#include "AnimatedShadow.h"

AnimatedShadow::AnimatedShadow(){
    currentFrame = 0;
    
    bHand = false;
    bActive = false;
    nId = 0;
};

AnimatedShadow::AnimatedShadow(int _nId){
    currentFrame = 0;
    bHand = false;
    bActive = false;
    nId = _nId;
};

void AnimatedShadow::addFrame( ofxBlob &_blob, float _widht, float _height ){
    
    if (nId == 0 ){
        nId = _blob.id;
    }
    
    if (nId == _blob.id){
        ofPolyline contourLine;
        
        for (int i = 0; i < _blob.pts.size(); i++){
            contourLine.addVertex(_blob.pts[i].x * _widht, _blob.pts[i].y * _height);
        }
        
        addFrame(contourLine, _blob.nFingers);
    }
}

void AnimatedShadow::addFrame( ofPolyline &_contourLine, int _nFingers){
    
    if ( _nFingers > 0 ){
        bHand = true;
    }
    
    if ( _contourLine.size() > 0){
        _contourLine.setClosed(true);  
        _contourLine.simplify(1);
        
        ShadowShape newShape;
        newShape.contour = _contourLine.getSmoothed(1,1);
        newShape.haveHole = false;
        //newShape.interv = -1;
        
        shapes.push_back( newShape );
    }
    
    bActive = false;
}

void AnimatedShadow::insertHole( ofPolyline &holeContourLine ){
    if (shapes.size() > 0){
        holeContourLine.setClosed(true);    
        holeContourLine.simplify(1);
        
        int lastFrame = shapes.size()-1;
        shapes[lastFrame].hole = holeContourLine.getSmoothed(1,1);
        shapes[lastFrame].haveHole = true;
    }
}

bool AnimatedShadow::draw(){
    bool finish = true;
    
    //  Just draw it´s active
    //
    if ( bActive ){
        ofPushStyle();
        
        //  Draw shape
        //
        ofSetColor(0,sin( (currentFrame/shapes.size() ) * PI ) * 255);
        int nPoints = shapes[currentFrame].contour.getVertices().size();
        ofBeginShape();
        for (int i = 0; i < nPoints; i++){
            ofVertex( shapes[currentFrame].contour[i] );
        }
        ofEndShape(true);
        
        //  If have holes draw them
        //
        if ( shapes[currentFrame].haveHole ){
            
            ofSetColor(255,sin( (currentFrame/shapes.size() ) * PI ) * 255);
            nPoints = shapes[currentFrame].hole.getVertices().size();
            ofBeginShape();
            for (int i = 0; i < nPoints; i++){
                ofVertex( shapes[currentFrame].hole[i] );
            }
            ofEndShape(true);
        }
        
        ofPopStyle();
        
        //  Update drawing frame counter
        //
        currentFrame += 0.4;

        if ((int)(currentFrame) >= (shapes.size() - 1))
            currentFrame = 0;
        else 
            finish = false;
    }
    
    return finish;
};
