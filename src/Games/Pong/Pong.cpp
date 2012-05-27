//
//  Pong.cpp
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#include "Pong.h"

Pong::Pong(){
    width = 800;
    height = 600;
    trackedSurfaceID = TRACK_JUST_HANDS;
}

void Pong::init(ofRectangle _space){    
    ofPoint center = ofPoint(_space.getCenter().x * width,_space.getCenter().y * height);
    space.setFromCenter(center, 
                        _space.width* width * 0.85, 
                        _space.height * height * 0.85);
    
    fbo.allocate(width, height);
    fbo.begin();
    ofClear(0);
    fbo.end();
    
    ball = Ball(&space, &player1, &player2);
    ofAddListener( ball.goal , this, &Pong::point);
    pixel = space.width/40;
    
    reset();
}

void Pong::reset(){
    player1Points = 0;
    player2Points = 0;
    
    player1Position = space.y + space.height * 0.5;
    player2Position = space.y + space.height * 0.5;
    
    player1.setFromCenter(space.x+60, player1Position, pixel, pixel*4);
    player2.setFromCenter(space.x+space.width-60, player2Position, pixel, pixel*4);
    
    ball.reset();
    ball.width = pixel;
    ball.height = pixel;
}

void Pong::handMoved(ofxBlob &_blob){
    if (_blob.gotFingers){
        if (_blob.palm.x < 0.5 ){
            player1Position = _blob.palm.y * height;
        } else {
            player2Position = _blob.palm.y * height;
        }
    }
}

void Pong::update(){
    ball.update();
    
    player1.setFromCenter(space.x+60, player1.getCenter().y + ( player1Position - player1.getCenter().y)*0.2, pixel, pixel*4);
    player2.setFromCenter(space.x+space.width-60, player2.getCenter().y + ( player2Position - player2.getCenter().y)*0.2, pixel, pixel*4);
    
    //  AutoPlayer 1
    //
    //player1.setFromCenter(space.x+60, ball.getCenter().y, pixel, pixel*4);
}

void Pong::render(){
    fbo.begin();
    ofClear(0);
    
    ofSetColor(255,255);
    
    drawCenter();
    drawScore();
    
    ofRect(ball);
    ofRect(player1);
    ofRect(player2);
    
    fbo.end();
}

void Pong::point(int &playerNumber){
    if (playerNumber == 1)
        player1Points++;
    else
        player2Points++;
}

void Pong::drawCenter() {
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(space.x+space.width*0.5, space.y);
    for (int i = 0 ; i <= space.height; i++) {
        if (i%pixel == 0) {
            ofSetColor(255);
            ofSetLineWidth(2);
            ofLine(0, i, 0, i+pixel/2);
        }
    }
    ofPopStyle();
    ofPopMatrix();
}

void Pong::drawScore() {
    drawNumber( (space.x+space.width*0.5 - pixel*4), player1Points);
    drawNumber( (space.x+space.width*0.5 + pixel*3), player2Points);
}

void Pong::drawNumber( float _x, int n ) {
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(_x, space.y+space.height*0.05+pixel);
    ofSetLineWidth(5);
    int pixel = 30;
    ofNoFill();
    switch( n ) {
        case 0 :
            ofRect(0, 0, pixel/2, pixel);
            break;
        case 1 :
            ofLine(pixel/2, 0, pixel/2, pixel); 
            break;
        case 2 :
            ofLine(0, 0, pixel/2, 0);
            ofLine(pixel/2, 0, pixel/2, pixel/2);
            ofLine(pixel/2, pixel/2, 0, pixel/2);
            ofLine(0, pixel/2, 0, pixel);
            ofLine(0, pixel, pixel/2, pixel);
            break;
        case 3:
            ofLine(0, 0, pixel/2, 0);
            ofLine(pixel/2, 0, pixel/2, pixel);
            ofLine(pixel/2, pixel, 0, pixel);
            ofLine(pixel/2, pixel/2, 0, pixel/2);
            break;
        case 4:
            ofLine(0, 0, 0, pixel/2);
            ofLine(0, pixel/2, pixel/2, pixel/2);
            ofLine(pixel/2, pixel/2, pixel/2, pixel);
            break;
        case 5:
            ofLine(0, 0, pixel/2, 0);
            ofLine(0, 0, 0, pixel/2);
            ofLine(0, pixel/2, pixel/2, pixel/2);
            ofLine(pixel/2, pixel/2, pixel/2, pixel);
            ofLine(0, pixel, pixel/2, pixel);
            break;
        case 6:
            ofLine(0, 0, 0, pixel);
            ofLine(0, pixel/2, pixel/2, pixel/2);
            ofLine(pixel/2, pixel/2, pixel/2, pixel);
            ofLine(0, pixel, pixel/2, pixel);
            break;
        case 7:
            ofLine(0, 0, pixel/2, 0);
            ofLine(pixel/2, 0, pixel/2, pixel);
            break;
        case 8:
            ofLine(0, 0, pixel/2, 0);
            ofLine(0, pixel/2, pixel/2, pixel/2);
            ofLine(0, pixel, pixel/2, pixel);
            ofLine(0, 0, 0, pixel);
            ofLine(pixel/2, 0, pixel/2, pixel);
            break;
        case 9:
            ofLine(0, 0, pixel/2, 0);
            ofLine(0, pixel/2, pixel/2, pixel/2);
            ofLine(0, 0, 0, pixel/2);
            ofLine(pixel/2, 0, pixel/2, pixel);
            break;
        case 10:
            reset();
            break;
    }
    ofPopStyle();
    ofPopMatrix();
}
