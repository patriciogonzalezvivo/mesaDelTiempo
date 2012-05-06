//
//  Oca.cpp
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#include "Oca.h"

Oca::Oca(){
    width = 800*1.771;
    height = 600*1.771;
}

Oca::~Oca(){
    int total = places.size();
    for(int i = 0; i < total; i++){
        delete places[i];
    }
    places.clear();
}

void Oca::init(ofRectangle _space){
    ofPoint center = ofPoint(_space.getCenter().x * width,_space.getCenter().y * height);
    float scaleFactor = _space.height;
    
    space.setFromCenter(center, 
                        scaleFactor * height, 
                        scaleFactor * height);
    
    fbo.allocate(width,height);
    fbo.begin();
    ofClear(0,255);
    fbo.end();

    mask.loadImage("Oca/mask.png");
    background.loadImage("Oca/background.jpg");
    
    text.set(space);
    /*
    text.setFromCenter(space.x+space.width*0.5, 
                       space.y+space.height*0.5,
                       space.width*0.8, 
                       space.height*0.8);
    */
    text.loadSequence("Oca/style.xml");
    reset();
}

void Oca::reset(){
    
    //  Load each place of the deck
    //
    places.clear();
    loadPlaces("Oca/config.xml");
    
    //  Setup spacial backgrounds
    //
    forestBackground.set( places[10]->getBoundingBox() );
    forestBackground.setZoom(60);
    forestBackground.clear();
    dragonBackground.set( places[25]->getBoundingBox() );
    dragonBackground.clear();
    
    //  Load neutral images of interactive places
    //
    obj09.loadImage("Oca/09alt/09-00.png");
    obj17.loadImage("Oca/17alt/17-00.png");
    
    ficha.pos = places[0]->getBoundingBox().getCenter();
    ficha.placeN = 0;
}

bool Oca::loadPlaces(string _xmlConfigFile){
    bool loaded = false;
    
    ofxXmlSettings XML;
    
    if (XML.loadFile(_xmlConfigFile)){
        
        int totalPatchs = XML.getNumTags("place");
        
        for(int i = 0; i < totalPatchs ; i++){
            XML.pushTag("place", i);
            
            Place *newPlace = new Place( XML.getValue("id", -1) );
            newPlace->setMessage (XML.getValue("message", "NO MESSAGE") );
            
            // Load the mask path
            if ( XML.pushTag("mask") ){
                int totalMaskCorners = XML.getNumTags("point");
                if (totalMaskCorners > 0){
                    newPlace->clear();
                }
                
                for(int i = 0; i < totalMaskCorners; i++){
                    XML.pushTag("point",i);
                    newPlace->addVertex( space.x + XML.getValue("x", 0.0) * space.width, space.y + XML.getValue("y", 0.0)*space.height);
                    XML.popTag();
                }
                XML.popTag(); // Pop "mask"
            }
            
            newPlace->setImage( XML.getValue("baseImage", "00.png") );
            
            places.push_back(newPlace);
            
            XML.popTag();   // Pop "place"
        }
    } else
        ofLog(OF_LOG_ERROR,"Oca: loading file " + _xmlConfigFile );
    
    return loaded; 
}

void Oca::update(){
    //  Places update
    //
    bool passed = false;
    for(int i = places.size()-1; i >= 0; i--){
        bool over = false;
        //  Activar (nLevel) todos los casilleros anteriores
        //  al lugar donde este la última ficha
        //
        if ( !passed ){
            if (places[i]->inside( ficha.pos )){
                if ( ficha.placeN != i){
                    
                    if ( i < 4 ){
                        obj09.loadImage("Oca/09alt/09-00.png");
                    } else if (i <= 7){
                        obj09.loadImage("Oca/09alt/09-0"+ofToString(i-3)+".png");
                    } 
                    
                    if ( i < 13){
                        obj17.loadImage("Oca/17alt/17-00.png");
                    } else if (i == 13){
                        obj17.loadImage("Oca/17alt/17-01.png");
                    }
                    
                    text.addMessage( places[i]->getMessage() );
                    ficha.placeN = i;
                }
                
                if ( places[i]->getState() >= 1.8 ){
                    places[i]->setState(0.99);
                }
                
                passed = true;
                over = true;
            }
        }
        
        if (passed){
            //places[i]->turnTo( 1.0 );
            places[i]->turnToMax();
        } else {
            places[i]->turnTo( 0.0 );
        }
    }

    //  Animate the text
    //
    text.update();
    
    //  Forest and Dragon Background update
    //
    updateBackground(10, forestBackground);
    updateBackground(25, dragonBackground);
}

void Oca::updateBackground(int _placeNumber, ofxTint& _backgroundEffect){
    _backgroundEffect.setFade( 0.2 + (1.0- ofClamp(places[10]->getState(), 0.0, 1.0) ) *0.8  ); 
    
    if (places[_placeNumber]->getState() < 0.01)
        forestBackground.clear();
    
    _backgroundEffect.begin();
        ofTranslate(-places[_placeNumber]->getBoundingBox().x,
                    -places[_placeNumber]->getBoundingBox().y);
    
        ofPushMatrix();
            ofClear(0,255);
            ofSetColor( ofClamp(places[_placeNumber]->getState(), 0.0, 1.0) *200,255);
            ofBeginShape();
                for(int i = 0; i < places[_placeNumber]->size(); i++)
                    ofVertex( places[_placeNumber]->getVertices()[i] );
            ofEndShape();
        ofPopMatrix();
    
    _backgroundEffect.end();
    
    _backgroundEffect.update();
}

void Oca::render(){
    fbo.begin();
    ofClear(0,255);
    ofPushStyle();
    ofSetColor(255,255);
    
    //  Draw Background
    //
    background.draw(space);
    
    //  Draw forest and dragon background
    //
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    ofSetColor(255,255);
    forestBackground.draw();
    dragonBackground.draw();
    ofDisableBlendMode();
    
    ofSetColor(255, 255);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    //  Draw Places
    //
    for(int i = 0; i < places.size(); i++){
        places[i]->draw();
    }
    
    //  Draw Object Picked
    //
    ofSetColor( ofClamp( places[9]->getState(), 0.0, 1.0)*200, 255);
    obj09.draw(places[9]->getBoundingBox());
    
    //  Draw Friend
    //
    ofSetColor( 255, ofClamp( places[13]->getState(), 0.0, 1.0)*255);
    obj17.draw(places[17]->getBoundingBox());
    
    //  Draw the deck mask
    //
    ofSetColor(255,255);
    mask.draw(space);
    
    //  Draw text
    //
    text.draw();
    
    //  Draw the ficha
    //
    ofNoFill();
    ofSetColor(0, 255, 0, 255);
    ofCircle(ficha.pos , 15);
    ofFill();
    ofCircle(ficha.pos , 10);
    
    ofPopStyle();
    fbo.end();
}

void Oca::objectAdded(ofxBlob &_blob){
    ofPoint _pos;
    
    _pos.x = _blob.centroid.x;
    _pos.y = _blob.centroid.y;
    
    ficha.pos.x = _pos.x*width;
    ficha.pos.y = _pos.y*height;
}

void Oca::objectMoved(ofxBlob &_blob){
    ofPoint _pos;
    
    _pos.x = _blob.centroid.x;
    _pos.y = _blob.centroid.y;
    
    ficha.pos.x = _pos.x*width;
    ficha.pos.y = _pos.y*height;
}

void Oca::objectDeleted(ofxBlob &_blob){
    ofPoint _pos;
    
    _pos.x = _blob.centroid.x;
    _pos.y = _blob.centroid.y;
    
    ficha.pos.x = _pos.x*width;
    ficha.pos.y = _pos.y*height;
}