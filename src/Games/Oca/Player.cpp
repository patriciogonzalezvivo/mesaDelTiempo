//
//  Player.cpp
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#include "Player.h"

Player::Player( int _id ){
    width = 70;
    height = 70;
    
    //  Numero de ficha
    //
    nID = _id;
    
    //  Registros de cursor que lo presiono y del casillero donde esta
    //
    nPlaceID = -1;
    nCursorID = -1;
    
    color.set(255,255);
}

void Player::set(int _x, int _y){
    x = _x;
    y = _y;
    
    pos.set(x, y);
}

void Player::update(){
    if ( !arrive() ){
        pos += ( getCenter() - pos)*0.01;
    } else if ( bLeave ){
        ofNotifyEvent(arriveToPlace, nID , this);
        bLeave = false;
    }
}

void Player::draw(){
    ofPushStyle();
    
    if ( !arrive() ){
        ofSetColor(255,200);
        img.draw(pos.x-34,pos.y-35,70,70);
        
        ofSetColor(255,50);
        img.draw(*this);
    } else {
        ofSetColor(255,255);
        img.draw(*this);
    }

    //  Debug nState
    //
    //ofSetColor(0,255);
    //ofDrawBitmapString( ofToString(nPlaceID), x+10, y+15);
    
    ofPopStyle();
}