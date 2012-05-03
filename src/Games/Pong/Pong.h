//
//  Pong.h
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#ifndef PONGGAME
#define PONGGAME

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
    void        render();
    
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
