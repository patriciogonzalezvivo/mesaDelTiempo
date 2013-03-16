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
#include "Player.h"

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
    void    objectMoved(ofxBlob &_blob);
    void    objectDeleted(ofxBlob &_blob);
    
private:
    bool    loadXml(string xmlConfigFile);
    
    void    updatePlacesStatus();
    void    updatePlacesBackground(int _placeNumber, ofxTint& _backgroundEffect);
    
    void    playerArriveToPlace( int &_player );
    
    TextMessager    text;
    
    ofxTint         dragonBackground;
    ofxTint         forestBackground;
    ofImage         background, mask;
    
    ofImage         obj17;
    bool            bFriend, b;
    
    vector<Place*>  places;
    vector<Player>  players;
    
    int             higherPlace;
    float           scaleFactor;
};

#endif
