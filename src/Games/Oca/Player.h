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

class Player : public ofRectangle {
public:
    
    Player( int _id );
    
    void    set(int _x, int _y);
    void    setImage(string _file);
    
    bool    arrive(){ return pos.distance(getCenter()) < 3.0; };
    
    void    update();
    void    draw();
    
    ofEvent<int> arriveToPlace;
    
    ofColor color;
    
    
    int     nCursorID;
    int     nPlaceID;
    bool    bLeave;
    
private:
    int     nID;
    ofImage img;
    ofPoint pos;
};

#endif
