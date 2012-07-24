//
//  Kaleido.h
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 7/9/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "Game.h"

#include "ofxFX.h"
#include "Shape.h"

class Kaleido : public Game {
public:
    
    Kaleido();
    ~Kaleido();
    
    void    init(ofRectangle _space);
    void    reset();
    
    void    objectAdded(ofxBlob &_blob);
    void    objectMoved(ofxBlob &_blob);
    void    objectDeleted(ofxBlob &_blob);
    
    void    update();
    void    render(); 
    
private:
    ofPolyline      getContour(ofxBlob& _blob);
    
    ofFbo           objects;
    ofImage         background;
    ofImage         *image;
    ofxKaleidoscope kaleidoEffect;
    map<int,Shape*> shapes;
    
    ofPolyline      triangle;
    ofPoint         offset;
    
    int             nImages;
    float           countDown;
};
