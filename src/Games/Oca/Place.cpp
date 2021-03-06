//
//  Place.cpp
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#include "Place.h"

Place::Place( int _nId ){
    angle       =   0.0;
    nState      =   1.0;
    turnToState =   1;
    bLoop       =   false;
    bAnimated   =   false;
    bColored    =   false;
    color.set(0,0,0,0);
    lockUntil   =   -1;
    
    nId = _nId;
}

void Place::turnToMax(){ 
    if(bAnimated)
        turnTo(2.0);
    else
        turnTo(1.0);
}

void Place::setImage(string _imgFile){
    ofFile file;
    file.open(_imgFile);
    image.loadImage( file );
    x = getBoundingBox().getCenter().x - image.getWidth()*0.5*scale; 
    y = getBoundingBox().getCenter().y - image.getHeight()*0.5*scale;
    width = image.getWidth()*scale;
    height = image.getHeight()*scale;
    
    ofDirectory dir;
    string animationFolder = "Oca/"+ file.getBaseName();
    
    if (dir.listDir(animationFolder) > 0){
        sequence.loadSequence(animationFolder);
        sequence.preloadAllFrames();
        sequence.setFrameRate(3);
        
        bAnimated   =   true;
    } else {
        bAnimated   =   false;
    }
    
}

void Place::draw(){
    nState = ofLerp(nState, turnToState, 0.01);
    
    if (bColored){
        ofPushStyle();
        ofSetColor(color);
        ofFill();
        ofBeginShape();
        for(int i = 0; i < getVertices().size() ; i++ ){
            ofVertex( getVertices()[i] );
        }
        ofEndShape();
        ofPopStyle();
    }
    
    if ( nState <= 1.1){
        ofSetColor(255, nState*255);
        image.draw(x,y,width,height);
        
        if (bAnimated)
            sequence.setFrame(0);
        
    } else if ( nState <= 2){
        
        ofSetColor(255);
        if (bAnimated){
            if (bLoop){
                sequence.getFrameForTime(ofGetElapsedTimef())->draw(x,y,width,height);
            } else
                sequence.getFrameAtPercent(nState-1.0)->draw(x,y,width,height);
        } else {
            image.draw(x,y,width,height);
        }
    } 

    //  Debug nState
    //
    //ofSetColor(0);
    //ofDrawBitmapString( ofToString(nId) , getCentroid2D() );
}