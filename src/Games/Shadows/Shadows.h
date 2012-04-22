//
//  Shadows.h
//  mdt-Core
//
//  Created by Patricio González Vivo on 4/6/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef SHADOWSGAME
#define SHADOWSGAME

#include "ofMain.h"

#include "ofxFX.h"
#include "ofxTextSuite.h"

#include "Game.h"
#include "AnimatedShadow.h"


class Shadows: public Game {
public:
    
    Shadows();
    
    void    init(ofRectangle _space);
    void    reset();
    
    void    update();
    
    void    handAdded(ofxBlob &_blob);
    void    handMoved(ofxBlob &_blob);
    void    handDeleted(ofxBlob &_blob);

private:
    void    playNext();
    void    playLast();
    int     checkIfHole(ofPolyline& _polyline);
    ofPolyline  getContour(ofxBlob& _blob);
    
    map<int,AnimatedShadow*>    hands;

    AnimatedShadow* currentShadow;
    
    ofImage         background;
    
    ofxTextSequencer text;
    ofxBlur         blur;
    ofxBlur         blurText;
    ofxTint         tint;
     
    int     countDown;
    int     nLastShadows;
    bool    bNew;
};

#endif
