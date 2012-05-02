//
//  Place.h
//  OcaSketch
//
//  Created by Patricio González Vivo on 4/15/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
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
    void    setScale(float _scale){scale = _scale;};
    void    setImage(string _imgFile);
    void    setMessage(string _message){ message = _message;};
    
    string  getMessage(){ return message; };
    
    void    turnTo(int _state){turnToState = _state;};
    void    draw();

private:
    ofImage             image;
    ofxImageSequence    sequence;
    
    string  message;
    
    float   x,y,width,height;
    float   scale;
    float   nState;     //  0: oculto
                        //  1: visible
                        //  2: animado
                        //  3: coloreado
    
    int     turnToState;
    int     nId;
    bool    bAnimated;
};

#endif
