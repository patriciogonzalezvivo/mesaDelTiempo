//
//  Player.cpp
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#include "Player.h"

Player::Player(){
    width = 30;
    height = 30;
    
    nId = -1;
    nPlace = -1;
    
    color.set(255,255);
}

void Player::update(){
    
}

void Player::draw(){
    ofPushStyle();
    ofSetColor(color);
    ofCircle(getCenter(), (width+height)*0.5 );
    ofSetColor(0,255);
    ofDrawBitmapString(ofToString(nPlace), x+10, y+10);
    ofDrawBitmapString(ofToString(nId), x+10,y+25);
    ofPopStyle();
}