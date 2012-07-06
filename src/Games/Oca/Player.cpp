//
//  Player.cpp
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#include "Player.h"

Player::Player( int _id ){
    radio = 100;
    
    //  Numero de ficha
    //
    nID = _id;
    
    //  Registros de cursor que lo presiono y del casillero donde esta
    //
    nPlaceID = -1;
    nCursorID = -1;
    
    color.set(255,255);
}

void Player::set(ofPoint pos){
	x		= pos.x;
	y		= pos.y;
    timer = 10.0;
}

bool Player::inside(ofPoint p){
    return p.distance(*this) <= radio;
}

void Player::update(){
    if ( !arrive() ){
        timer = ofLerp(timer, 0.0, 0.01);
    } else if ( bLeave ){
        ofNotifyEvent(arriveToPlace, nID , this);
        bLeave = false;
    }
}

void Player::draw(){
    ofPushStyle();
    
    if ( !arrive() ){
        ofSetColor(255,170+(10.0-timer)*8.5);
        img.draw(x-25,y-25,50,50);
        
        ofPolyline  timerLine;
        timerLine.arc(x,y, 22, 22, 0, (10.0-timer)*36.0 );
        
        ofPushStyle();
        ofNoFill();
        
        ofSetLineWidth(7);
        ofSetColor(0,170);
        timerLine.draw();
        
        ofSetLineWidth(5);
        ofSetColor(color,170+(10.0-timer)*8.5);
        timerLine.draw();
        ofPopStyle();
        
        
    } else {
        ofSetColor(255,255);
        img.draw(x-25,y-25,50,50);
    }
    
    //  Debug nState
    //
    /*
    ofNoFill();
    ofSetColor(255);
    ofRect(*this);
    */
    //ofSetColor(0,255);
    //ofDrawBitmapString( ofToString(nPlaceID), x+10, y+15);
    
    ofPopStyle();
}