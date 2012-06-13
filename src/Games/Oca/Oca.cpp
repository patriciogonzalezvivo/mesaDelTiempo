//
//  Oca.cpp
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#include "Oca.h"

Oca::Oca(){
    width = 800*1.771;
    height = 600*1.771;
    bWaitToSendText = false;
    trackMode = TRACK_JUST_OBJECT;
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
    scaleFactor = _space.height;// * 0.85;
    
    space.setFromCenter(center, 
                        scaleFactor * height, 
                        scaleFactor * height);
    
    fbo.allocate(width,height);
    fbo.begin();
    ofClear(0,255);
    fbo.end();
    
    mask.loadImage("Oca/mask.png");
    background.loadImage("Oca/background.jpg");
    
    text.setFromCenter(space.x+space.width*0.5, 
                       space.y+space.height*0.5,
                       space.width*0.6, 
                       space.height*0.6);
    
    text.loadSequence("Oca/style.xml");
    reset();
}

void Oca::reset(){
    
    //  Load each place of the deck
    //
    places.clear();
    loadXml("Oca/config.xml");
    
    cout << places[0]->getCentroid2D() << endl;
    //  Setup spacial backgrounds
    //
    forestBackground.set( places[10]->getBoundingBox() );
    forestBackground.setZoom(30);
    forestBackground.clear();
    dragonBackground.set( places[25]->getBoundingBox() );
    dragonBackground.clear();
    
    //  Friend Image
    //
    obj17.loadImage("Oca/17alt/17-00.png");
    
    bWaitToSendText = false;
}

bool Oca::loadXml(string _xmlConfigFile){
    bool loaded = false;
    
    ofxXmlSettings XML;
    
    if (XML.loadFile(_xmlConfigFile)){
        
        //  Places ( casilleros )
        //
        int totalPatchs = XML.getNumTags("place");
        
        for(int i = 0; i < totalPatchs ; i++){
            XML.pushTag("place", i);
            
            Place *newPlace = new Place( XML.getValue("id", -1) );
            
            newPlace->angle = XML.getValue("angle", 0.0);
            newPlace->message = XML.getValue("message", "NULL");
            
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
            
            newPlace->scale = scaleFactor;
            newPlace->bLoop = XML.getValue("loop", false);
            newPlace->lockUntil = XML.getValue("lock", -1);
            
            if ( XML.tagExists("link")){
                if ( XML.pushTag("link") ){
                    int totalLinks = XML.getNumTags("id");
                    
                    for(int i = 0; i < totalLinks; i++){
                        int linkId = XML.getValue("id", i);
                        newPlace->linked.push_back(linkId);
                        
                    }
                    XML.popTag(); // Pop "link"
                }
            }
            
            newPlace->setImage( XML.getValue("baseImage", "00.png") );
            newPlace->bColored = XML.getValue("colored", false);
            
            places.push_back(newPlace);
            
            XML.popTag();   // Pop "place"
            
            
            //  Players ( fichas )
            //
            int totalPlayer = XML.getNumTags("player");
            for(int i = 0; i < totalPlayer ; i++){
                XML.pushTag("player", i);
                
                Player newPlayer;
                
                newPlayer.x = XML.getValue("pos:x", 0.0);
                newPlayer.y = XML.getValue("pos:y", 0.0);

                newPlayer.color.set(XML.getValue("color:r", 255),
                                    XML.getValue("color:g", 255),
                                    XML.getValue("color:b", 255));
                
                players.push_back(newPlayer);
                
                XML.popTag();   // Pop "player"
            }
        }
    } else
        ofLog(OF_LOG_ERROR,"Oca: loading file " + _xmlConfigFile );
    
    return loaded; 
}

void Oca::update(){
    //  Wait to lunch text
    //
    /*
    if (bWaitToSendText){
        if ( places[selectedPlace]->getTransition() < 0.1 ){
            text.addMessage( places[selectedPlace]->message );
            textAngle = places[selectedPlace]->angle;
            bWaitToSendText = false;
        }
    }*/
    
    //  Animate the text
    //
    //text.update();
    
    //  Forest and Dragon Background update
    //
    updateBackground(10, forestBackground);
    updateBackground(25, dragonBackground);
}

void Oca::updateBackground(int _placeNumber, ofxTint& _backgroundEffect){
    _backgroundEffect.setFade( 0.2 + (1.0- ofClamp(places[_placeNumber]->nState, 0.0, 1.0) ) *0.8  ); 
    
    if (places[_placeNumber]->nState < 0.01)
        _backgroundEffect.clear();
    
    _backgroundEffect.begin();
        ofTranslate(-places[_placeNumber]->getBoundingBox().x,
                    -places[_placeNumber]->getBoundingBox().y);
    
        ofPushMatrix();
            ofClear(0,255);
            ofSetColor( ofClamp(places[_placeNumber]->nState, 0.0, 1.0) *200,255);
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
    
    //  Draw Friend
    //
    ofSetColor( 255, ofClamp( places[13]->nState, 0.0, 1.0)*255);
    obj17.draw(places[17]->getBoundingBox());
    
    //  Draw the deck mask
    //
    ofSetColor(255, 255);
    mask.draw(space);
    
    //  Draw text
    //
    /*
    ofPushMatrix();
    ofTranslate(space.getCenter());
    ofRotateZ( textAngle );
    ofTranslate(-space.getCenter().x,-space.getCenter().y);
    ofSetColor(0,ofMap( text.getNormTransitionValue(), 0.0, 0.5, 0.0, 1.0, true) * 255);
    text.draw();
    ofPopMatrix();
    */
    
    //  Draw players
    //
    for(int i = 0; i < players.size(); i++){
        players[i].draw();
    }
    
    ofPopStyle();
    fbo.end();
}

void Oca::objectAdded(ofxBlob &_blob){
    
}

void Oca::objectMoved(ofxBlob &_blob){
    ofPoint blobPos = ofPoint(_blob.centroid.x * space.width, 
                              _blob.centroid.y * space.height);
    
    for(int i = 0; i < players.size(); i++){
        if (players[i].inside( blobPos )){
            players[i].setFromCenter(blobPos, width, height);
        }
    }
}

void Oca::objectDeleted(ofxBlob &_blob){
    
}

