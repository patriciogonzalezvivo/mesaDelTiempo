/*
 *
 *  testApp.cpp
 *	Communitas
 *
 *  Created by Patricio Gonz‡lez Vivo on 05/07/10.
 *  Copyright 2010 http://PatricioGonzalezVivo.com All rights reserved.
 *
 *
 *  This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0
 *	Unported License. To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/
 *	or send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 *
 */

#include "Communitas.h"

//--------------------------------------------------------------

Communitas::Communitas(){
    width = 800*1.771;
    height = 600*1.771;
    trackMode = TRACK_JUST_OBJECT;
}

Communitas::~Communitas(){
    int total = pAct.size();
    for(int i = 0; i < total; i++){
        delete pAct[i];
    }
    pAct.clear();
    
    total = pAct.size();
    for(int i = 0; i < total; i++){
        delete pInact[i];
    }
    pInact.clear();
}

void Communitas::init(ofRectangle _space){
    ofPoint center = ofPoint(_space.getCenter().x * width,_space.getCenter().y * height);
    
    space.setFromCenter(center, MIN(width, height), MIN(width, height));
    
    //  Allocate the fbo and clean it
    //
    fbo.allocate(width, height);
    fbo.begin();
    ofClear(0,0);
    fbo.end();
    
    fade    =    1.0f;
    blur.allocate(width, height);
    blur.setFade(fade);
	blur.setPasses(5);
    
    universe.set(space);
    nucle.set(space);
    
	universe.mass           = 0.75f;
	universe.vortex         = space.height*0.1f;
	universe.maxDistance	= space.height*0.4f;
    
	//universe.density		= 0.4f;
	universe.ph				= 6.5f;
	universe.joinParticles	= true;
	universe.correctOrbits	= true;
	universe.proportional	= true;
	universe.clockwise      = true;
    
	//universe.impulse		= 0.4f;
	universe.angle			= 0.0f;
    
	nucle.mass				= 10.0f;
	nucle.vortex			= space.height*0.1;
	nucle.maxDistance		= space.height*0.1;
    
	nucle.density			= 1.0f;	//= 0.05;
	nucle.ph	       		= 7.0f;	//= 6.9;
	nucle.joinParticles		= true;
    
	nucle.correctOrbits		= false;// = true;
	nucle.proportional		= false;
	nucle.clockwise			= false;
	nucle.impulse			= 0.19f;
	nucle.angle		       	= 0.0f;	//= -25;
    
	reset();
}

void Communitas::reset(){
    blur.clear(255);
    
	int total = pAct.size();
    for(int i = 0; i < total; i++){
        delete pAct[i];
    }
    pAct.clear();
    
    total = pAct.size();
    for(int i = 0; i < total; i++){
        delete pInact[i];
    }
    pInact.clear();
    
    timer                   = 0.0f;
	standBy                 = 30.0f;
}

//--------------------------------------------------------------
void Communitas::update(){

	if (timer <= standBy){
		nucle.mass				= ofLerp(nucle.mass,10.0f,0.01f);
		nucle.maxDistance		= ofLerp(nucle.maxDistance,space.height*0.07f,0.1f);

		if ( timer >= (standBy-standBy/3.0f)){
			universe.mass		= ofLerp(universe.mass,10.0f,0.01f);
			universe.proportional= false;
			universe.clockwise	= false;
			universe.impulse	= timer - (standBy - standBy/3.0f);
			universe.angle		= ofLerp(universe.angle,-40.0f,0.1);
			universe.density	= ofLerp(universe.density,0.8f,0.01f);
			fade                = ofLerp(fade,0.5f,0.01f);
			fboAlpha            = ofLerp(fboAlpha, 242.0, 0.1f);
		} else {
			universe.mass		= ofLerp(universe.mass,0.75f,0.01f);
			universe.proportional= true;
			universe.clockwise	= true;
			universe.impulse	= 0.3f;
			universe.angle		= ofLerp(universe.angle,0.0f,0.1f);
			universe.density	= ofLerp(universe.density,0.7f,0.01f);
			fade                = ofLerp(fade,1.0f,0.01f);
			fboAlpha            = ofLerp(fboAlpha, 230.0, 0.1f);
		}
	} else {
		nucle.maxDistance		= ofLerp(nucle.maxDistance,space.height*0.01f,0.1f);
		nucle.mass				= 0.7f;
		fade                    = ofLerp(fade,0.0f,0.01f);
		fboAlpha = ofLerp(fboAlpha,255.0,0.1f);
	}

    blur.setFade(fade);
    
	for (int i = 0; i < pAct.size() ; i++)
		if (universe.inside(pAct[i]->loc) && (pAct[i]->alpha > 5) ){
			pAct[i]->update();
			if (nucle.inside(pAct[i]->loc)){
				if (pAct[i]->bArrive){
					absorveParticle(i);
                }
            }
		} else {
            absorveParticle(i);
        }


	for (int i = 0; i < pInact.size() ; i++){
		if (!universe.inside(pInact[i]->loc) || (pInact[i]->alpha < 5) ){
            //delete pInact[i];
			pInact.erase(pInact.begin()+i);
		} else { 
            pInact[i]->update();
        }
    }

	timer += 1/ofGetFrameRate();
}

//--------------------------------------------------------------
void Communitas::render(){
    blur.begin();
	for (int i = pInact.size()-1; i > 0 ; i--){
		pInact[i]->draw();
    }
	blur.end();
    blur.update();
    
    fbo.begin();
    ofPushStyle();
    
    ofClear(0,255);
	ofSetColor(fboAlpha,fboAlpha);
	blur.draw(0,0);
    
/*
	for (int i = pInact.size()-1; i > 0 ; i--){
		pInact[i]->draw();
    }
*/
	ofSetColor(255,255);
	for (int i = pAct.size()-1; i > 0 ; i--){
		pAct[i]->draw();
    }
    
    //nucle.drawBorder();
    //universe.drawBorder();

    ofPopStyle();
    fbo.end();
}

//----------------------------------------------------------- Agrega una particula

void Communitas::addParticle(ofPoint _pos, int _n) {
	if ( universe.inside( _pos ) ){
		timer = 0;

		int nLnk = 0;
		for (int i = pAct.size()-1; i >= 0 ; i--){
			if (nLnk == 0)
				if (pAct[i]->idN == _n)
					nLnk = i;

			if ( _pos.distance(pAct[i]->loc) <= 40)
				pAct[i]->alpha = 255;
		}

		LineDot * dot = new LineDot(_n, &universe, _pos ,false);

		if ( nLnk > 0) 
            dot->setLnk(pAct[nLnk]);

		pAct.push_back(dot);
	}
}

void Communitas::absorveParticle(int _n){
	pAct[_n]->setSpace(&nucle);
	pAct[_n]->vel = pAct[_n]->getOrbitalForce(1);
	pAct[_n]->alpha /= 1.5;
    
	if (pInact.size() > 2000) 
        pInact.erase(pInact.begin());
    
	pInact.push_back(pAct[_n]);
	pAct.erase(pAct.begin()+_n);
}


void Communitas::objectAdded(ofxBlob &_blob){
    ofPoint blobPos = ofPoint(_blob.centroid.x * width,
                              _blob.centroid.y * height);
    
    addParticle(blobPos,_blob.id);
}

void Communitas::objectMoved(ofxBlob &_blob){
    ofPoint blobPos = ofPoint(_blob.centroid.x * width,
                              _blob.centroid.y * height);
    
    addParticle(blobPos,_blob.id);
}

void Communitas::objectDeleted(ofxBlob &_blob){
}