//
//  Place.h
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
    void    setScale(float _scale){ scale = _scale;};
    void    setAngle(float _angle){ angle = _angle;};
    void    setState(float _state){ nState = _state;};
    void    setImage(string _imgFile);
    void    setMessage(string _message){ message = _message;};
    
    float   getState(){return nState;};
    float   getAngle(){return angle;};
    string  getMessage(){ return message; };
    
    void    turnTo(int _state){turnToState = _state;};
    void    turnToMax(){ if ( turnToState < 1.0 ) turnTo(2.0); }
    void    draw();

private:
    ofImage             image;
    ofxImageSequence    sequence;
    
    string  message;
    
    float   x,y,width,height;
    float   scale,angle;
    float   nState;     //  0: oculto
                        //  1: visible
                        //  2: animado
                        //  3: coloreado
    
    int     turnToState;
    int     nId;
    bool    bAnimated;
};

#endif
