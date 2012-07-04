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

class SpaceRing{
public:
	SpaceRing(){		
		mass = 1;
		
		vortex		= maxDistance/2;
		maxDistance	= ofGetWindowWidth() /2 ;
		
		density		= 1;			
		ph			= 7;	// con 7 las partículas no se disuelven
		
		joinParticles	= false;
		correctOrbits	= false;
		proportional	= false;
		clockwise		= false;
		impulse		= 0;
		angle		= 0;
	};
	
    void    set(ofRectangle &area){
        center = area.getCenter();
    }
    
	SpaceRing& findCenter(){center = ofVec2f(ofGetWindowWidth()/2, ofGetWindowHeight()/2);return * this;};
	SpaceRing& findCenter(int _width, int _height){center = ofVec2f(_width/2, _height/2);return * this;};
	bool	onSpace(ofVec2f _pos){ if (center.distance(_pos) <= maxDistance) return true; else return false; };
	
	ofPoint center;
	
	float	mass, vortex, maxDistance, density, ph, joinParticles, impulse, angle;
	bool	correctOrbits, clockwise, proportional;
};