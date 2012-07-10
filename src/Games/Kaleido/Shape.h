//
//  Shape.h
//  mdt-Core
//
//  Created by Patricio González Vivo on 7/10/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef mdt_Core_Shape_h
#define mdt_Core_Shape_h

#include "ofMain.h"

class Shape : public ofPolyline {
public:
    
    Shape(){
        clear();
        color.set(255,0,0);
    }
    
    Shape(int _nId, vector<ofPoint>& verts){
        nId = _nId;
        clear();
        addVertexes(verts);
        
        color.set(255,0,0);
    }
    
    int     getId() const { return nId; };
    
    void    changeHue(float _value){
        float hue = color.getHue();
        if ( hue+_value >= 255 ){
            color.setHue( (hue+_value)-255 );
        } else { 
            color.setHue( hue+_value );
        }
    }
    
    void    draw(){
        
    }

private:
    ofColor color;
    int     nId;
};

#endif
