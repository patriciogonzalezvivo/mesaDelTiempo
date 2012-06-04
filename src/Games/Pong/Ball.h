//
//  Ball.h
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#ifndef BALL
#define BALL

#include "ofMain.h"
#include "ofxBlob.h"

class Ball : public ofRectangle {
public:
    
    Ball(){
        p1 = NULL;
        p2 = NULL;
        space = NULL;
    };
    
    Ball( ofRectangle* _space, ofRectangle* _player1, ofRectangle* _player2){
        space = _space;
        p1 = _player1;
        p2 = _player2;
        
        width = 15;
        height = 15;
        
        bounce = -1.0;
        
        reset();
    };
    
    void    reset(){
        x = space->x + space->width*0.5;
        y = space->y + space->height*0.5;
        
        vel.set( ofRandom(-0.9,0.9), ofRandom(-0.1,0.1));
        vel.normalize();
        vel *= 4;
    }
     
    void    update(){
        
        // Check bounds
        //
        if (( (x + width) >= (space->x+space->width)) ||
            ( x <= space->x) ){
            vel.x *= bounce;
            
            int pointToPlayer = 0;
            
            if (x < (space->x+space->width*0.5) )
                pointToPlayer = 2;
            else 
                pointToPlayer = 1;
            
            ofNotifyEvent(goal, pointToPlayer);
        } 
        
        if (( (y + height) >= (space->y+space->height) ) ||
            ( y <= space->y) ){
            vel.y *= bounce;
        }
        
        // Check Paddles
        // 
        checkPaddle(p1);
        checkPaddle(p2);
        
        x += vel.x;
        y += vel.y;
    }
    
    void checkPaddle(ofRectangle * paddle){
        if (paddle->inside( getCenter() + vel)){
            vel.x *= bounce;
        }
    }   
    
    vector<ofxBlob> obstacles;
    
    ofEvent<int>    goal;
    
    ofRectangle     *space;
    ofRectangle     *p1;
    ofRectangle     *p2;
    
    ofVec2f         vel;
    float           bounce;
};

#endif
