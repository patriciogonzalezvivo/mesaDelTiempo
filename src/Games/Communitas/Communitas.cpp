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
}

Communitas::~Communitas(){
    
}

void Communitas::init(ofRectangle _space){
    ofPoint center = ofPoint(_space.getCenter().x * width,_space.getCenter().y * height);
    
    space.setFromCenter(center,
                        height,
                        height);
    
	blur.allocate(space.width, space.height);
	blur.setRadius(3);
	blur.setPasses(1);

	timer			= 0;
	standBy			= 30;
	interact		= true;

	universe.findCenter(space.width, space.height);
	universe.mass		= 0.75;
	universe.vortex		= space.height*0.1;
	universe.maxDistance	= space.height*0.5;

	//universe.density		= 0.4;
	universe.ph				= 6.5;
	universe.joinParticles	= true;
	universe.correctOrbits	= true;
	universe.proportional	= true;
	universe.clockwise	= true;

	//universe.impulse		= 0.4;
	universe.angle			= 0;

	nucle.findCenter(space.width, space.height);
	nucle.mass				= 10;
	nucle.vortex			= space.height*0.1;
	nucle.maxDistance		= space.height*0.1;

	nucle.density			= 1;	//= 0.05;
	nucle.ph	       		= 7;	//= 6.9;
	nucle.joinParticles		= true;

	nucle.correctOrbits		= false;// = true;
	nucle.proportional		= false;
	nucle.clockwise			= false;
	nucle.impulse			= 0.19;
	nucle.angle		       	= 0;	//= -25;
	ofVec2f a;
}

void Communitas::reset(){
    
}

//--------------------------------------------------------------
void Communitas::update(){

	if (timer <= standBy){
		nucle.mass				= 10;
		nucle.maxDistance		= space.height*0.07;

		if ( timer >= (standBy-standBy/3)){
			universe.mass		= 10;
			universe.proportional= false;
			universe.clockwise	= false;
			universe.impulse	= timer - (standBy - standBy/3);
			universe.angle		= -40;
			universe.density	= 0.8;
			blur.setRadius(1);
			fboAlpha			= 200;
		} else {
			universe.mass		= 0.75;
			universe.proportional= true;
			universe.clockwise	= true;
			universe.impulse	= 0.3;
			universe.angle		= 0;
			universe.density	= 0.7;
			blur.setRadius(0.5);
			fboAlpha			= 150;
		}
	} else {
		nucle.maxDistance		= space.height*0.01;
		nucle.mass				= 0.7;
		blur.setRadius(2);
		fboAlpha				= 255;
	}

	for (int i=0; i < pAct.size() ; i++)
		if (universe.onSpace(pAct[i]->loc) && (pAct[i]->alpha > 5) ){
			pAct[i]->update();

			if (nucle.onSpace(pAct[i]->loc))
				if (pAct[i]->bArrive)
					killParticle(i);

		} else killParticle(i);


	for (int i=0; i < pInact.size() ; i++)
		if (!universe.onSpace(pInact[i]->loc) || (pInact[i]->alpha < 5) )
			pInact.erase(pInact.begin()+i);
		else pInact[i]->update();

	timer += 1/ofGetFrameRate();
}

//--------------------------------------------------------------
void Communitas::render(){
    ofPushStyle();
    fbo.begin();
    ofClear(0,255);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	blur.begin();
	for (int i = pInact.size()-1; i > 0 ; i--)
		pInact[i]->draw();
	blur.end(false);

	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(fboAlpha, fboAlpha, fboAlpha,fboAlpha);
	blur.draw();

	for (int i = pInact.size()-1; i > 0 ; i--)
		pInact[i]->draw();

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofSetColor(255, 255, 255);
	for (int i = pAct.size()-1; i > 0 ; i--)
		pAct[i]->draw();


	if (drawSpaceBorders){
		universe.drawBorder();
		nucle.drawBorder();
	}
    ofPopStyle();
    fbo.end();
}

//----------------------------------------------------------- Agrega una particula

void Communitas::addParticle(int x, int y, int _n) {
	if ( universe.onSpace( ofVec2f(x,y) ) ){
		timer = 0;

		int nLnk = 0;
		for (int i = pAct.size()-1; i >= 0 ; i--){
			if (nLnk == 0)
				if (pAct[i]->idN == _n)
					nLnk = i;

			if ( ofVec2f(x,y).distance(pAct[i]->loc) <= 40)
				pAct[i]->alpha = 255;
		}

		ofxParticle * obj = new ofxParticle(_n,&universe,ofVec2f(x,y),false);

		if ( nLnk > 0) obj->setLnk(pAct[nLnk]);

		pAct.push_back(obj);
	}
}

void Communitas::insertParticle(int x, int y, int _n, int _hue = -1) {
	if ( universe.onSpace( ofVec2f(x,y) + universe.center ) ){
		timer = 0;
		ofxParticle * obj = new ofxParticle(_n,&universe,ofVec2f(x,y)+universe.center,true);

		int nLnk = 0;
		for (int i = 0; i < pAct.size() ; i++){
			if (nLnk == 0) {
				if (pAct[i]->idN == _n-1)
					nLnk = i;
				else if (pAct[i]->idN == _n+1)
					pAct[i]->setLnk(obj);
			}

			if ( ofVec2f(x,y).distance(pAct[i]->loc) <= 40)
				pAct[i]->alpha = 255;
		}

		if ( nLnk > 0) obj->setLnk(pAct[nLnk]);
		if ( _hue != -1) obj->setColor(_hue);

		pAct.push_back(obj);
	}
}

void Communitas::killParticle(int _n){
	pAct[_n]->setSpace(&nucle);
	pAct[_n]->vel = pAct[_n]->getOrbitalForce(1);
	pAct[_n]->alpha /= 1.5;
	if (pInact.size() > 2000) pInact.erase(pInact.begin());
	pInact.push_back(pAct[_n]);
	pAct.erase(pAct.begin()+_n);
}

/*
void testApp::windowResized(int w, int h){
    screenWidth = ofGetWindowWidth();
	screenHeight = ofGetWindowHeight();
    
	blur.setup(screenWidth, screenHeight);
	blur.begin();
	ofClear(0, 0, 0, 255);
	blur.end(false);

	universe.findCenter(screenWidth,screenHeight);
	nucle.findCenter(screenWidth,screenHeight);
}
*/

/*
void testApp::tuioAdded(ofxTuioCursor & tuioCursor){
	addParticle(tuioCursor.getX()*screenWidth,tuioCursor.getY()*screenHeight,tuioCursor.getSessionId());
}

void testApp::tuioUpdated(ofxTuioCursor &tuioCursor){
	addParticle(tuioCursor.getX()*screenWidth,tuioCursor.getY()*screenHeight,tuioCursor.getSessionId());
}

void testApp::tuioRemoved(ofxTuioCursor & tuioCursor){
}
*/
