//
//  Shadows.h
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#ifndef SHADOWSGAME
#define SHADOWSGAME

#include "ofMain.h"

#include "ofxFX.h"

#include "Game.h"


#define WITH_TEXT
#ifdef WITH_TEXT
#include "TextSequencer.h"
#include "InkAnimation.h"
#include "AnimatedShadow.h"
#endif

class Shadows: public Game {
public:
    
    Shadows();
    
    void    init(ofRectangle _space);
    void    reset();
    
    void    update();
    void    render();
    
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
    
    TextSequencer   text;
    InkAnimation    textAnimation;
    
    ofxBlur         blur;
     
    int     countDown;
    int     nLastShadows;
    bool    bNew;
};

#endif
