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
    scaleFactor = _space.height;
    
    space.setFromCenter(center, 
                        scaleFactor * height, 
                        scaleFactor * height);
    
    fbo.allocate(width,height);
    fbo.begin();
    ofClear(0,255);
    fbo.end();
    
    mask.loadImage("Oca/mask.png");
    background.loadImage("Oca/background.jpg");
    
    reset();
}

void Oca::reset(){
    places.clear();
    loadPlaces("Oca/config.xml");
    
    dragonBackground.set( places[25]->getBoundingBox() );
    dragonBackground.setZoom(60);
    dragonBackground.clear();
    
    
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
            newPlace->setScale( scaleFactor );
            
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
                    ficha.placeN = i;
                }
                passed = true;
                over = true;
            }
        }
        
        if (passed){
            places[i]->turnTo( 1.0 );
        } else {
            places[i]->turnTo( 0.0 );
        }
    }

    //  Dragon Background 
    //
    dragonBackground.setFade( 0.2 + (1.0- places[25]->getState() ) *0.8  );
    if (places[25]->getState() < 0.01)
        dragonBackground.clear();
    dragonBackground.begin();
    ofTranslate(-places[25]->getBoundingBox().x,-places[25]->getBoundingBox().y);
    ofPushMatrix();
    ofClear(0,255);
    ofSetColor(places[25]->getState()*200,255);
    ofBeginShape();
    for(int i = 0; i < places[25]->size(); i++)
        ofVertex( places[25]->getVertices()[i] );
    ofEndShape();
    ofPopMatrix();
    dragonBackground.end();
    dragonBackground.update();
}

void Oca::render(){
    fbo.begin();
    ofClear(0,255);
    ofPushStyle();
    ofSetColor(255,255);
    
    //  Draw Background
    //
    background.draw(space);
    
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    ofSetColor(255,255);
    dragonBackground.draw();
    ofDisableBlendMode();
    
    ofSetColor(255, 255);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    //  Draw Places
    //
    for(int i = 0; i < places.size(); i++){
        places[i]->draw();
    }
    
    //  Draw the deck mask
    //
    ofSetColor(255,255);
    mask.draw(space);
    
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