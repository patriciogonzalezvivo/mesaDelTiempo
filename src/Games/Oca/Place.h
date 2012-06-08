//
//  Place.h
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#ifndef PLACE
#define PLACE

#include "ofMain.h"
#include "ofxImageSequence.h"

class Place : public ofPolyline {
public:
    Place();
    Place( int _nId );
    
    void    setId(int _nid){nId == _nid;};
    void    setImage(string _imgFile);
    
    bool    getActive(){ return (turnToState == nState);};
    float   getTransition(){ return ofClamp(abs(turnToState - nState),0.0,1.0); };
    
    void    turnTo(int _state){ turnToState = _state; };
    void    turnToMax();
    void    randomActive();
    
    void    draw();
    
    string  message;
    
    ofColor color;
    vector<int> linked;
    int     lockUntil;
    
    float   scale;
    float   angle;
    float   nState;     //  0: oculto
                        //  1: visible
                        //  2: animado
    bool    bLoop;
    bool    bColored;

private:
    ofImage             image;
    ofxImageSequence    sequence;
    
    float   x,y,width,height;
    int     turnToState;
    int     nId;
    bool    bAnimated;
};

#endif
