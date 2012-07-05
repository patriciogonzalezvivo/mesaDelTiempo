/*
 *
 *  ofxSpace.h
 *	Communitas
 *
 *  Created by Patricio González Vivo on 05/07/10.
 *  Copyright 2010 http://PatricioGonzalezVivo.com All rights reserved.
 *
 *
 *  This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 
 *	Unported License. To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ 
 *	or send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 *
 */

#pragma once

#include "ofMain.h"

class SpaceRing : public ofPoint {
public:
	SpaceRing(){		
		mass            = 1.0;
		
		vortex          = maxDistance*0.5;
		maxDistance     = ofGetWindowWidth()*0.5;
		
		density         = 1.0;			
		ph              = 7.0;      // con 7 las partículas no se disuelven
		
		joinParticles	= false;
		correctOrbits	= false;
		proportional	= false;
		clockwise		= false;
		impulse         = 0.0;
		angle           = 0.0;
	};
	
    void    set(ofRectangle &area){ 
        x = area.getCenter().x; 
        y = area.getCenter().y; 
    };
    
	bool	inside(ofPoint _pos){ 
        if (distance(_pos) <= maxDistance) 
            return true; 
        else 
            return false; 
    };
	
    void    drawBorder(){
		ofNoFill();
		ofSetLineWidth(1);
		ofSetLineWidth(1);
		ofCircle(x,y, maxDistance);
		ofFill();
	};
    
	float	mass, vortex, maxDistance, density, ph, joinParticles, impulse, angle;
	bool	correctOrbits, clockwise, proportional;
};