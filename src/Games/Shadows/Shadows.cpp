//
//  Shadows.cpp
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#define STRINGIFY(A) #A
#include "Shadows.h"

Shadows::Shadows(){
    //  Screen resolution
    //
    //width = 1024;
    //height = 768;
    
    width = 1024 * 1.5;
    height = 768 * 1.5;
    trackedSurfaceID = TRACK_JUST_HANDS;
}

void Shadows::init(ofRectangle _space){
    
    background.loadImage("shadows/fondo.jpg");
    
    //  Screen resolution
    //
    //width = background.getWidth();
    //height = background.getHeight();
    
    //  Asign a space (it´s the surface area)
    //
    ofPoint center = ofPoint(_space.getCenter().x * width,_space.getCenter().y * height);
    space.setFromCenter(center, 
                        _space.width*width, 
                        _space.height*height);
    
    //  Allocate the fbo and clean it
    //
    fbo.allocate(width, height);
    fbo.begin();
    ofClear(0,0);
    fbo.end();
    
    blur.allocate(width,height);
    
    text.loadSequence("shadows/texto.xml");
    text.set(0,0,space.width*0.8, space.height*0.8);
    ofRectangle area;
    area.setFromCenter(space.x+space.width*0.5, 
                       space.y+space.height*0.5,
                       text.width,
                       text.height);
    
    textAnimation.set(area);
    
    //  Clean the variables and the start
    //
    reset();
}

void Shadows::reset(){
    
    for( map<int,AnimatedShadow*>::reverse_iterator rit = hands.rbegin(); rit != hands.rend(); rit++ ){
        delete rit->second;
    }
    
    hands.clear();
    
    currentShadow = NULL;
    bNew = false;
    
    countDown = 0;
    nLastShadows = 0;
}

void Shadows::update(){
    
    //  shadow bluring
    //
    blur.begin();
    ofPushStyle();
    ofClear(255,255);
    if ( countDown == 0){
        
        //  Play the shadows animation one by one from the last up to the first one
        //  Ever time a new shadow it´s made, the cicle start´s from the beginning.
        //
        ofFill();
        if (currentShadow != NULL){
            if (currentShadow->bActive){
                if ( currentShadow->draw() ){
                    if (bNew){
                        playLast();
                        bNew = false;
                    } else {
                        playNext();
                    }
                }
            }
        }
    } else {
        int clamp = nLastShadows;
        for( map<int,AnimatedShadow*>::reverse_iterator rit = hands.rbegin(); rit != hands.rend(); rit++ ){
            if ((clamp > 0) || 
                rit->second->getCurrentFrame() != 0){
                if ( rit->second->bActive ) {
                    rit->second->draw();
                    clamp--;
                }
            }
        }
        countDown--;
        if ((countDown == 0) && 
            (nLastShadows > 0) ){
            countDown = 50;
            nLastShadows--;
        }
    }
    ofPopStyle();
    blur.end();
    blur.update();
    
    //  Animate the text
    //
    text.update();
    textAnimation.update(text);
}

void Shadows::render(){
    fbo.begin();
    ofPushStyle();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(255, 255);
    
    //  Background Gradient Mesh
    //
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    // this could be optimized by building a single mesh once, then copying
    // it and just adding the colors whenever the function is called.
    ofVec2f center(width / 2, height / 2);
    mesh.addVertex(center);
    mesh.addColor(ofColor(252,239,233,255));
    int n = 32; // circular gradient resolution
    float angleBisector = TWO_PI / (n * 2);
    float smallRadius = ofDist(0, 0, height / 2, height / 2);
    float bigRadius = smallRadius / cos(angleBisector);
    for(int i = 0; i <= n; i++) {
        float theta = i * TWO_PI / n;
        mesh.addVertex(center + ofVec2f(sin(theta), cos(theta)) * bigRadius);
        mesh.addColor(ofColor(58,30,0,255));
    }
	glDepthMask(false);
	mesh.draw();
	glDepthMask(true);
    
    //  Background texture
    //
    ofSetColor(255, 50);
    background.draw(0,0,width,height);
    
    //  Draw Blured shadows fbo
    //
    ofSetColor(255, 255);
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    blur.getTextureReference().draw(0,0);
    
    //  Draw Text fbo
    //
    ofSetColor(255, 255);
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    textAnimation.draw();
    ofDisableBlendMode();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    ofPopStyle();
    fbo.end();
}

//---------------------------------------------------------------- EVENTS
//

//  Hands are initialized with a nId and flaged as non-active.
//  In every move a new frame it´s added to each other acording the nId
//  When they disapier the are flagged as Active... that´s means they are not longer to be edited...
//  ... now they are going to be played.
//

void Shadows::handAdded(ofxBlob &_blob){
    
    //  Make a ofPolygone from the _blob
    //
    ofPolyline contourLine = getContour(_blob);
    
    //  Check if it's a hole. If it´s not it will return a -1 if it´s a hole it will
    //  return the nId of the owner
    //
    int holeOfBlob  = checkIfHole(contourLine);  
        
    if (holeOfBlob >= 0){
        
        //  If it´s a hole it will insert it into the last blobFrame
        //
        hands[holeOfBlob]->insertHole(contourLine);
        //ofLog(OF_LOG_NOTICE,"Adding hole to shadow idº " + ofToString( holeOfBlob ));
    } else {
        
        //  If it's not a hole add a new shadow to the map with the first frame
        //
        AnimatedShadow *newShadow = new AnimatedShadow( _blob.id );
        newShadow->addFrame( contourLine, _blob.nFingers );
        hands[ newShadow->getId() ] = newShadow;
        //ofLog(OF_LOG_NOTICE,"Adding shadow idº " + ofToString( newShadow->getId() ));
    }
}

void Shadows::handMoved(ofxBlob &_blob){
    
    if ( hands[ _blob.id ] == NULL ){
        hands.erase( _blob.id );
        
        //  If don´t have record of this blob probably it´s because it´s a hole of a blob...
        //  ... so try to find it owner
        //
        ofPolyline contourLine = getContour(_blob);
        int holeOfBlob  = checkIfHole(contourLine);  
        
        if (holeOfBlob >= 0){
            //  If it´s a hole it will insert it into the last blobFrame
            //
            hands[holeOfBlob]->insertHole(contourLine);
            //ofLog(OF_LOG_NOTICE,"Adding new frame of hole shadow idº " + ofToString( holeOfBlob ));
        } else {
            ofLog(OF_LOG_NOTICE,"Something goes wrong... I don´t what to do with blob idº " + ofToString( _blob.id ));
        }
        
    } else {
        hands[ _blob.id ]->addFrame(_blob,width,height);
        //ofLog(OF_LOG_NOTICE,"Adding new frame of shadow idº " + ofToString( _blob.id ));
    }
}

void Shadows::handDeleted(ofxBlob &_blob){
    
    if ( hands[ _blob.id ] == NULL ){
        hands.erase( _blob.id );
        
        //  If don't have record of this blob probably it's because it's a hole of a blob...
        //
        
    } else {
        
        //  So it seams like a shadow
        //
        
        if (!hands[ _blob.id ]->isHand() && 
            !hands[ _blob.id ]->size() < 30 ){
            
            //  if the shadow never gave a single finger or it didn't get enought frames
            //  probably it's just junk...
            //  ... so let's delet it
            //
            delete hands[ _blob.id ];
            hands.erase( _blob.id );
            //ofLog(OF_LOG_NOTICE,"Deleting shadow idº " + ofToString( _blob.id ));
            
        } else {
            
            //  if had fingers and enought frames let's keep it and play it!!!
            //
            hands[ _blob.id ]->bActive = true;  // this means it´s goign to be played and no longer edited
            bNew = true;                        // and the the sistem that we have a new one
            
            countDown = 50;
            nLastShadows++;
            
            //  When the game starts currentShadow it's just a NULL and nothing it´s going to happend
            //  until new blob arrive and take the place of currentShadow.
            //  If currentShadow it´s not NULL that just means that it's one been played 
            //
            if (currentShadow == NULL){
                playLast();
            }
        }
    }
}

//-------------------------------------------------------------------- TOOLS
//

ofPolyline  Shadows::getContour(ofxBlob& _blob){
    ofPolyline contourLine;
    for (int i = 0; i < _blob.pts.size(); i++){
        contourLine.addVertex(_blob.pts[i].x * width, _blob.pts[i].y * height);
    }
    
    return contourLine;
}

int Shadows::checkIfHole(ofPolyline& _contourLine){
    int rta = -1;
    
    //  Calculate the centroid
    //
    ofPoint centroid = _contourLine.getCentroid2D();
    
    //  Check if it fits inside another another non-active blob.
    //
    for( map<int,AnimatedShadow*>::reverse_iterator rit = hands.rbegin(); rit != hands.rend(); rit++ ){
        if ( !(rit->second->bActive) ) {
            if ( rit->second->isInside( centroid ) ){
                rta = rit->second->getId();
            }
        }
    }
    
    return rta;
}

void Shadows::playNext(){
    bool foundNext = false;
    bool currentPassed = false;
    
    for( map<int,AnimatedShadow*>::reverse_iterator rit = hands.rbegin(); rit != hands.rend(); rit++ ){
        if (currentPassed &&
            rit->second->isHand() && 
            rit->second->bActive ){
            currentShadow = rit->second;
            foundNext = true;
            break;
        }
        
        if (rit->second->getId() == currentShadow->getId() ){
            currentPassed = true;
        }
    }
    
    if ( !foundNext )
        playLast();
}

void Shadows::playLast(){
    for( map<int,AnimatedShadow*>::reverse_iterator rit = hands.rbegin(); rit != hands.rend(); rit++ ){
        if ( rit->second->isHand() && rit->second->bActive ){
            currentShadow = rit->second;
            break;
        }
    }
}