//
//  Shadows.cpp
//  mdt-Core
//
//  Created by Patricio González Vivo on 4/6/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#define STRINGIFY(A) #A
#include "Shadows.h"

Shadows::Shadows(){
    
    //  Screen resolution
    //
    width = 1024;
    height = 768;
}

void Shadows::init(ofRectangle _space){
    
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
    
    //  Allocate the fbo for the blur
    //
    for(int i = 0; i < 2; i++){
        preBlurFbo[i].allocate(width, height);
        preBlurFbo[i].begin();
        ofClear(0,0);
        preBlurFbo[i].end();
    }
    
    //  Load both blur shaders
    //
    string fragmentHorizontalBlurShader = STRINGIFY(
                                                    uniform sampler2DRect backbuffer;
                                                    uniform float radius;
                                                    
                                                    const float total = (1. + 8. + 28. + 56.) * 2. + 70.;
                                                    
                                                    void main(void) {
                                                        vec2 st = gl_TexCoord[0].st;
                                                        
                                                        vec4 color = vec4(0.0,0.0,0.0,0.0);
                                                        color += (1. / total) * texture2DRect(backbuffer, st - radius * vec2(4. / 4., 0.));
                                                        color += (8. / total)  * texture2DRect(backbuffer, st - radius * vec2(3. / 4., 0.));
                                                        color += (28. / total)  * texture2DRect(backbuffer, st - radius * vec2(2. / 4., 0.));
                                                        color += (56. / total)  * texture2DRect(backbuffer, st - radius * vec2(1. / 4., 0.));
                                                        
                                                        color +=  (70. / total) * texture2DRect(backbuffer, st);
                                                        
                                                        color += (1. / total) * texture2DRect(backbuffer, st + radius * vec2(4. / 4., 0.));
                                                        color += (8. / total)  * texture2DRect(backbuffer, st + radius * vec2(3. / 4., 0.));
                                                        color += (28. / total)  * texture2DRect(backbuffer, st + radius * vec2(2. / 4., 0.));
                                                        color += (56. / total)  * texture2DRect(backbuffer, st + radius * vec2(1. / 4., 0.));
                                                        
                                                        gl_FragColor = color;
                                                    } 
                                                    );
    blurShader[0].unload();
    blurShader[0].setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentHorizontalBlurShader);
    blurShader[0].linkProgram();
    
    string fragmentVerticalBlurShader = STRINGIFY(
                                                  uniform sampler2DRect backbuffer;
                                                  uniform float radius;
                                                  
                                                  const float total = (1. + 8. + 28. + 56.) * 2. + 70.;
                                                  
                                                  void main(void) {
                                                      vec2 st = gl_TexCoord[0].st;
                                                      
                                                      vec4 color = vec4(0.0,0.0,0.0,0.0);
                                                      color += (1. / total) * texture2DRect(backbuffer, st - radius * vec2(0., 4. / 4.));
                                                      color += (8. / total)  * texture2DRect(backbuffer, st - radius * vec2(0., 3. / 4.));
                                                      color += (28. / total)  * texture2DRect(backbuffer, st - radius * vec2(0., 2. / 4.));
                                                      color += (56. / total)  * texture2DRect(backbuffer, st - radius * vec2(0., 1. / 4.));
                                                      
                                                      color +=  (70. / total) * texture2DRect(backbuffer, st);
                                                      
                                                      color += (1. / total) * texture2DRect(backbuffer, st + radius * vec2(0., 4. / 4.));
                                                      color += (8. / total)  * texture2DRect(backbuffer, st + radius * vec2(0., 3. / 4.));
                                                      color += (28. / total)  * texture2DRect(backbuffer, st + radius * vec2(0., 2. / 4.));
                                                      color += (56. / total)  * texture2DRect(backbuffer, st + radius * vec2(0., 1. / 4.));
                                                      
                                                      gl_FragColor = color;
                                                  } 
                                                  );
    blurShader[1].unload();
    blurShader[1].setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentVerticalBlurShader);
    blurShader[1].linkProgram();
    
    //  Clean the variables and the start
    //
    reset();
}

void Shadows::reset(){
    
    for( map<int,AnimatedShadow*>::reverse_iterator rit = hands.rbegin(); rit != hands.rend(); rit++ ){
        delete rit->second;
    }
    
    hands.clear();
    intervShadows.clear();
    
    currentShadow = NULL;
    bNew = false;
}

void Shadows::update(){
    
    //  FBO pre-blur effect
    //
    preBlurFbo[0].begin();
    ofPushStyle();
    
    ofClear(255,255);

    //  Play the shadows animation allways from the last up to the first one
    //  Ever time a new shadow it´s made, the cicle start´s from the beginning.
    //
    ofFill();
    if (currentShadow != NULL){
        if (currentShadow->bActive){
            
            
            if ( currentShadow->isInterv() != -1 ){
                map<int,AnimatedShadow*>::iterator it;
                
                it = hands.find( currentShadow->isInterv() );
                
                if (it != hands.end() ){
                    if (it->second->bActive){
                        intervShadows.push_back(currentShadow->isInterv());
                    }
                }
                
            }
            
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
    
    //ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    //  Play the interventions to the past shadows
    //
    for(int i = intervShadows.size() - 1; i >= 0; i--){
        map<int,AnimatedShadow*>::iterator it;
        
        it = hands.find( intervShadows[i] );
        
        if (it != hands.end() ){
    
            if ( hands[ intervShadows[i] ]->bActive ){
                if ( hands[ intervShadows[i] ]->draw() ){
                    intervShadows.erase( intervShadows.begin() + i);
                }
            }
        }
    }
    //ofDisableBlendMode();
    //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    ofPopStyle();
    preBlurFbo[0].end();
        
    //  PingPong cicles for blur post-process
    //
    for(int j = 0; j < 6; j++) {    
        preBlurFbo[(j+1)%2].begin();
        blurShader[j%2].begin();
        blurShader[j%2].setUniform1f("radius", 5);
        preBlurFbo[j%2].draw(0,0);
        blurShader[j%2].end();
        preBlurFbo[(j+1)%2].end();
    }
    
    //  Final Rendering width letters and debug information
    //
    fbo.begin();
    ofPushStyle();
    
    //  Background gradient
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
    
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    //  Draw Blured shadows
    //
    ofSetColor(255, 255);
    preBlurFbo[1].draw(0,0);
    
    //  Write debug information
    //
    ofSetColor(0,255);
    ofDrawBitmapString( "Total shadows: " + ofToString(hands.size()), 200, 200);
    if (currentShadow != NULL)
        ofDrawBitmapString( "Playing idº: " + ofToString(currentShadow->getId()), 200, 215);
    
    ofDrawBitmapString( "Total interventions: " + ofToString(intervShadows.size()), 200, 230);
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
        
        if (currentShadow != NULL){
            //  If it's playing a shadow insert the id in order to record witch one makes an intervention
            //  this will trigger the playing of this shadow
            currentShadow->addIntervention( newShadow->getId() );
            //ofLog(OF_LOG_NOTICE,"Adding intervention to shadow idº " + ofToString( currentShadow->getId() ));
        }
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
            !hands[ _blob.id ]->size() < 5 ){
            
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