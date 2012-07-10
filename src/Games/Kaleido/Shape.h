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
    
    Shape();
    
private:
    ofColor color;
};

#endif
