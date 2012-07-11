//
//  Kaleido.cpp
//  mdt-Core
//
//  Created by Patricio González Vivo on 7/10/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#include "Kaleido.h"

Kaleido::Kaleido(){
    width = 800*1.771;
    height = 600*1.771;
    trackMode = TRACK_JUST_OBJECT;
}

void Kaleido::init(ofRectangle _space){
    //background.loadImage("shadows/back.jpg");
    
    //  Asign a space (it´s the surface area)
    //
    ofPoint center = ofPoint(_space.getCenter().x * width,_space.getCenter().y * height);
    space.setFromCenter(center, _space.width*width, _space.height*height);
    
    //  Allocate the fbo and clean it
    //
    fbo.allocate(width, height);
    fbo.begin();
    ofClear(0,0);
    fbo.end();
    kaleidoEffect.allocate(width, height);
    kaleidoEffect.clear(0);
    
    //  Clean the variables and the start
    //
    reset();
}

void Kaleido::reset(){
    for( map<int,Shape*>::reverse_iterator rit = shapes.rbegin(); rit != shapes.rend(); rit++ ){
        delete rit->second;
    }
    shapes.clear();
    
    countDown = 255.0;
}

void Kaleido::update(){
    kaleidoEffect.begin();
    ofClear(0,0);
    
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
    mesh.addColor(ofColor(abs(sin(ofGetElapsedTimef()*0.01)*255),
                          abs(sin(ofGetElapsedTimef()*0.03)*255),
                          abs(sin(ofGetElapsedTimef()*0.09)*255),255));
    int n = 32; // circular gradient resolution
    float angleBisector = TWO_PI / (n * 2);
    float smallRadius = ofDist(0, 0, height / 2, height / 2);
    float bigRadius = smallRadius / cos(angleBisector);
    for(int i = 0; i <= n; i++) {
        float theta = i * TWO_PI / n;
        mesh.addVertex(center + ofVec2f(sin(theta), cos(theta)) * bigRadius);
        mesh.addColor(ofColor(0,0));
    }
	glDepthMask(false);
	mesh.draw();
	glDepthMask(true);
    
    //  Background texture
    //
    //ofSetColor(255, 50);
    //background.draw(0,0,width,height);
    
    //  Draw all shapes
    //
    for( map<int,Shape*>::reverse_iterator rit = shapes.rbegin(); rit != shapes.rend(); rit++ ){
        rit->second->draw();
    }
    
    kaleidoEffect.end();
    kaleidoEffect.setRotation(ofGetElapsedTimef()*0.1);
    kaleidoEffect.update();
    
    if ( countDown > 0.0 ){
        countDown = ofLerp(countDown,0,0.1);
    }
}

void Kaleido::render(){
    fbo.begin();
    ofPushStyle();
    
    ofSetColor(255, countDown);
    kaleidoEffect[0].draw(0,0);
    
    ofSetColor(255, 255-countDown);
    kaleidoEffect.getTextureReference().draw(0,0);
    
    ofSetColor(0, 255);
    ofDrawBitmapString(ofToString(countDown), width*0.5,height*0.5);
    
    ofPopStyle();
    fbo.end();
}

ofPolyline Kaleido::getContour(ofxBlob& _blob){
    ofPolyline contourLine;
    for (int i = 0; i < _blob.pts.size(); i++){
        contourLine.addVertex(_blob.pts[i].x * width, _blob.pts[i].y * height);
    }
    
    return contourLine;
}

void Kaleido::objectAdded(ofxBlob &_blob){
    //  Make a ofPolygone from the _blob
    //
    ofPoint     pos = ofPoint(_blob.centroid.x * width,
                              _blob.centroid.y * height);
    ofPolyline  contour = getContour(_blob);
    
    if ( space.inside(pos)){
        Shape *newShape = new Shape( _blob.id, contour.getVertices() );
        shapes[ newShape->getId() ] = newShape;
        
        countDown = ofLerp(countDown,255,0.05);
    }
}

void Kaleido::objectMoved(ofxBlob &_blob){
    ofPoint     pos = ofPoint(_blob.centroid.x * width,
                              _blob.centroid.y * height);
    ofPolyline  contour = getContour(_blob);
    
    if ( shapes[ _blob.id ] == NULL ){
        shapes.erase( _blob.id );
    } else {
        shapes[ _blob.id ]->clear();
        shapes[ _blob.id ]->addVertexes( contour.getVertices() );
        
        ofPoint lastPos = ofPoint(_blob.lastCentroid.x * width,
                                  _blob.lastCentroid.y * height);
        lastPos -= pos;
        
        float vel = lastPos.length()*0.01;
        if (vel > 1.0 ){
            shapes[ _blob.id ]->changeHue( vel );
            countDown = ofLerp(countDown,255,0.05);
        }
    }
}

void Kaleido::objectDeleted(ofxBlob &_blob){
    if ( shapes[ _blob.id ] == NULL ){
        shapes.erase( _blob.id );
    } else {
        for( map<int,Shape*>::reverse_iterator rit = shapes.rbegin(); rit != shapes.rend(); rit++ ){
            if ( rit->second->getId() == _blob.id){
                delete rit->second;
                break;
            }
        }
        shapes.erase( _blob.id );
        
        countDown = ofLerp(countDown,255,0.05);
    }
}