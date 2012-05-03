//
//  Oca.h
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#ifndef OCAGAME
#define OCAGAME

#include "ofMain.h"

#include "Game.h"
#include "TextSequencer.h"

#include "Place.h"

#include "ofxFX.h"

typedef struct {
    ofPoint pos;
    int     placeN;
} Ficha;


class Oca : public Game {
public:
    Oca();
    ~Oca();
    
    void    init(ofRectangle _space);
    void    reset();
    
    void    update();
    void    render();
    
    void    objectAdded(ofxBlob &_blob);
    void    objectMoved(ofxBlob &_blob);
    void    objectDeleted(ofxBlob &_blob);
    
private:
    bool    loadPlaces(string xmlConfigFile);
    
    vector<Place*>   places;
    ofImage         mask;
    ofImage         background;
    
    ofRectangle     centerSpace;
    ofxBlur         blur;
    ofxTint         tint;
    ofxTextSequencer text;
    
    Ficha         ficha;
    
    float           scaleFactor;
    bool            bChange;
};

#endif
