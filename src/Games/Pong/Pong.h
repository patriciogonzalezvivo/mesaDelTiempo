//
//  Pong.h
//  museoDelJuguete
//
//  Created by Patricio González Vivo on 3/27/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef PONG
#define PONG

#include "ofMain.h"

#include "Game.h"
#include "Ball.h"

class Pong : public Game {
public:
    Pong();
    
    void        init(ofRectangle _space);
    void        reset();
    
    void        handMoved(ofxBlob &_blob);
    
    void        update();
    
private:
    void        point(int &playerNumber);
    
    void        drawCenter();
    void        drawScore();
    void        drawNumber( float _x, int n );
    
    Ball        ball;
    ofRectangle player1, player2;
    
    float       player1Position, player2Position;
    int         player1Points, player2Points;
    int         pixel;
};

#endif
