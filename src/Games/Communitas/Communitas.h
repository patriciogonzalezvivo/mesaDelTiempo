/*
 *
 *  testApp.h
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

#ifndef _TESTAPP
#define _TESTAPP

#include "ofMain.h"
#include "ofxFX.h"

#include "Game.h"
#include "LineDot.h"
#include "SpaceRing.h"

class Communitas : public Game {
public:
	
    Communitas();
    ~Communitas();
    
    void    init(ofRectangle _space);
    void    reset();
    
    void    update();
    void    render();
    
    void    objectAdded(ofxBlob &_blob);
    void    objectMoved(ofxBlob &_blob);
    void    objectDeleted(ofxBlob &_blob);
	
private:
    void    addParticle(ofPoint _pos, int _n);
	void    absorveParticle(int _n);
    
    SpaceRing   nucle, universe;
	
	vector<LineDot*> pAct;
	vector<LineDot*> pInact;
	
	ofxBlur blur;
	int		fboAlpha;
	
	float	fade, timer, standBy, delay;
	
	bool	drawSpaceBorders;
};

#endif
