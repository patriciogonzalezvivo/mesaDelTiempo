//
//  Oca.h
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#ifndef OCAGAME
#define OCAGAME

#include "ofMain.h"

#include "Game.h"
#include "TextMessager.h"
#include "InkAnimation.h"

#include "Place.h"

#include "ofxFX.h"

class Oca : public Game {
public:
    Oca();
    ~Oca();
    
    void    init(ofRectangle _space);
    void    reset();
    
    void    update();
    void    render();
    
    void    objectAdded(ofxBlob &_blob);
    
private:
    bool    loadPlaces(string xmlConfigFile);
    void    updateBackground(int _placeNumber, ofxTint& _backgroundEffect);
    
    
    TextMessager    text;
    
    ofxTint         dragonBackground;
    ofxTint         forestBackground;
    ofImage         background, mask, maskBack, ficha;
    
    ofImage         obj17;
    vector<Place*>  places;
    
    ofPoint         fichaPos;
    int             selectedPlace;
    int             lockUntil;
    
    float           scaleFactor, textAngle;
    bool            bChange,bWaitToSendText;
};

#endif
