//
//  Player.h
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#ifndef PLAYER
#define PLAYER

#include "ofMain.h"

class Player : public ofPoint {
public:
    
    Player( int _id );
    
    void    set(ofPoint pos);
    bool    inside(ofPoint p);
    
    bool    arrive(){ 
        return  timer < 0.1;
    };
    
    void    update();
    void    draw();
    
    ofEvent<int> arriveToPlace;
    
    ofImage img;
    ofColor color;
    
    int     nCursorID;
    int     nPlaceID;
    bool    bLeave;
    
private:
    float   radio, timer;
    int     nID;
};

#endif
