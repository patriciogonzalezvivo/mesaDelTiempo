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
    background.loadImage("shadows/fondo.jpg");
    
    //  Asign a space (it´s the surface area)
    //
    ofPoint center = ofPoint(_space.getCenter().x * width,_space.getCenter().y * height);
    space.setFromCenter(center, _space.width*width, _space.height*height);
    
    //  Allocate the fbo and clean it
    //
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
    
    countDown = 100;
}

void Kaleido::update(){
    
}

void Kaleido::render(){
    
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
        
        countDown = 100;
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
        shapes[ _blob.id ]->changeHue( lastPos.length() );
        
        countDown = 100;
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
        
        countDown = 100;
    }
}