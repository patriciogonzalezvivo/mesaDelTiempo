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
    
    Player();
    
    void    update();
    void    draw();
    
    ofColor color;
    
    int     nId;
    int     nPlace;
    
private:
    ofImage *img;
    
};

#endif
