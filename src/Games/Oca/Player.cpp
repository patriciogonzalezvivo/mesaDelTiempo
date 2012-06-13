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
    
    color.set(255,255);
}

void Player::update(){
    
}

void Player::draw(){
    ofPushStyle();
    ofSetColor(color);
    ofCircle(getCenter(), (width+height)*0.5 );
    ofPopStyle();
}