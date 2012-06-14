//
//  Player.cpp
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#include "Player.h"

Player::Player( int _id ){
    width = 30;
    height = 30;
    
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
    
    pos.set(_x, _y);
}

void Player::setImage(string _file){

}

void Player::update(){
    if ( !arrive() ){
        pos += (getCenter() - pos)*0.01;
    } else if ( bLeave ){
        ofNotifyEvent(arriveToPlace, nID , this);
        bLeave = false;
    }
}

void Player::draw(){
    ofPushStyle();
    
    if ( !arrive() ){
        ofSetColor(color,200);
        ofCircle( pos, (width+height)*0.5 );
        
        ofSetColor(color,50);
        ofCircle( getCenter(), (width+height)*0.5 );
    } else {
        ofSetColor(color,255);
        ofCircle( pos, (width+height)*0.5 );
    }

    //  Debug nState
    //
    ofSetColor(0,255);
    ofDrawBitmapString( ofToString(nPlaceID), x+10, y+15);
    
    ofPopStyle();
}