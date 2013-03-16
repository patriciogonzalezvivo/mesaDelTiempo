/*
 *
 *  ofxParticle.cpp
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
#include "SpaceRing.h"

class LineDot {
public:
	LineDot(){
	    idN = 0;
		lnk = NULL;
		color.set(255,255,255,255);

		acc.set(0,0);
		vel.set(0,0);
		mass = 1;
		render_radio = radio = 4;
		alpha = 255;
	};

	LineDot(int _idN, const SpaceRing *_toRing, ofPoint _loc, bool _fromCenter){
		idN = _idN;
		lnk = NULL;
		setSpace(_toRing);
		setColor(_loc);

		if (_fromCenter){
			loc = *ring;
			sLoc.set(_loc);
			bArrive = false;
		} else {
			loc.set(_loc);
			setColor(_loc);
			bArrive = true;
			save();
		}

		acc.set(0,0);
		vel.set(0,0);
		mass = 1;
		render_radio = radio = 4;
		alpha = 255;
	};

	~LineDot(){

	};


	LineDot& save(){sLoc.set(loc); return * this;};
	LineDot& setSpace(const SpaceRing *_toRing)	{ ring = _toRing; return * this;};
	LineDot& setLnk(const LineDot *toLnk)	{ lnk = toLnk; return * this;};
	LineDot& setColor(ofVec2f _loc){
	    ofVec2f l = _loc - *ring;
	    color.setHsb((ofRadToDeg(atan2(l.x,l.y)+PI)/360.0f)*255, 255, 255);
	    return * this;
	    };
	LineDot& setColor(int _hue){ color.setHsb(_hue, 255, 255); return * this; };
	LineDot& addOrbitalForce(float _force){	applyForce(getOrbitalForce(_force)); return * this;};
	LineDot& applyForce(ofVec2f _force){
		_force /= mass;
		acc += _force;
		return * this;
	};

	int getColor(){ return color.getHue();};
	ofPoint steer(ofPoint target, bool arrival = true) {
		ofPoint steerForce;

		if (arrival){
			ofVec2f desired = target - loc;  // A vector pointing from the location to the target
			float d = desired.length(); // Distance from the target is the magnitude of the vector
			// If the distance is greater than 0, calc steering (otherwise return zero vector)
			if (d > 0.0f) {
				// Normalize desired
				desired.normalize();
				// Two options for desired vector magnitude (1 -- based on distance, 2 -- maxspeed)
				if (d < 1.0f)
					desired *= (d); // This damping is somewhat arbitrary
				//else
				//desired *= 2.0f;
				// Steering = Desired minus Velocity
				steerForce = desired - vel; //PVector.sub(desired,vel);
				//steer.limit(0.9);  // Limit to maximum steering force
			} else
				steerForce = ofPoint(0,0);

		} else {
			steerForce = target - loc;
			//steer.limit(maxforce);
		}

		return steerForce;
	}

	ofPoint	getOrbitalForce(float _force){
		ofPoint toCenter = *ring;
		toCenter -= loc;

		ofVec2f toCenterOFX(toCenter.x,toCenter.y);
		toCenterOFX.perpendicular();
		toCenterOFX.normalize();

		if ( ring->proportional ) toCenterOFX *= loc.distance( *ring )/(10000.0f*_force);
		else toCenterOFX *= _force;

		toCenterOFX.rotate(ring->angle);

		if (ring->clockwise) toCenterOFX *= -1.0f;

		return toCenterOFX;
	};

	void update(){
		if (bArrive){
			if (ring->correctOrbits) {
                addOrbitalForce(ring->impulse);
            }
			ofPoint direction = *ring - loc;     //  Primero calcula la dirección al centro del espacio que lo rije
			float d = direction.length();				//  De allí calcula la fuerza con que lo atrae
			d = ofClamp(d,5.0f,25.0f);
			float force = (9.8f * ring->mass * mass) / (d * d);
			direction.normalize();						//  Vuelve la dirección a un parámetro entre 0 y 1;
			applyForce(direction * force);				//  De la multiplicación del vector con la dirección 
                                                        //  con la fuerza de atracción la suma al desplazamiento
		} else {
			if (sLoc != loc)
				applyForce(steer(sLoc));
			else
				bArrive = true;
		}

		//Update en si mismo
		vel += acc;
		loc += vel;
		vel *= ring->density;
		acc *= 0.0;

		float distance = loc.distance(*ring);
		if ( distance <= ring->vortex)
			render_radio = ofMap(distance,0,(float)ring->vortex,0.0f,radio, true);
		else
			render_radio = radio;

		alpha += (ring->ph/10.0f-0.7f);
		if (alpha >= 1) render_radio = render_radio * alpha/255;
		color.a = alpha;
	};

	void draw(){
		ofSetColor( color );

		if ( ring->joinParticles ){
			if ( lnk != NULL )
				if ( ( lnk->ring->joinParticles) && (lnk->loc.distance(loc) < 40)){
					ofSetLineWidth(render_radio*2);
					ofLine(loc,lnk->loc);
				} else {
                    lnk = NULL;
                }
		}
		ofCircle(loc,render_radio*0.6f);
	};

	ofColor	color;
	ofPoint loc,acc,vel,sLoc;
	float	mass, radio, render_radio, alpha;
	int		idN;
	bool	bArrive;

private:
	const	SpaceRing   *ring;
	const	LineDot     *lnk;
};
