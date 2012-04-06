#pragma once
#include "ofMain.h"

/*
 *  Keys:
 *
 *      - F1:   Fullscreen ON/OFF
 *      - F2:   Masking mode ON/OFF
 *
 *              On mask mode on:
 *                              - x: delete mask path point
 *                              - r: reset mask path
 *
 *      - F3:   Surface edit mode on/off
 *      - F4:   Reset surface coorners
 *
 *      - d/D:  debug mode ON/OFF
 *
 *      - l/L:  load/reload previus calibration setup and then load the game
 *      - c/C:  calibrate and then load the game
 *
 *
 */

//  MAPPING & TRACKING
#include "ofxTrackedSurface.h"

//  GAME
#include "Game.h"   // This is the parent game class
//
#include "Pong.h"   //  Classic Ateri Pong game 
#include "Simon.h"  //  Classic SIMON game
#include "Shadows.h"//  Chinise Shadows game

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
    void exit();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    //  MAPPING & TRACKING
    //  ---------------------------
    //
    ofxTrackedSurface   tSurface;   // it«s in charge of the matching the tracked objects/hands
                                    // with the proyected game.
    //  tSurface Events
    void    calibrationDone(ofPolyline &_surface);
    
    void    handAdded(ofxBlob &_blob);
    void    handMoved(ofxBlob &_blob);
    void    handDeleted(ofxBlob &_blob);
    
    void    objectAdded(ofxBlob &_blob);
    void    objectMoved(ofxBlob &_blob);
    void    objectDeleted(ofxBlob &_blob);
    
    //  GAME
    //  ---------------------------
    //
    Game    *game;
    string  sGameName;
    bool    bUpdateObjects;
    bool    bUpdateHands;
    bool    bStart;
    
    void    loadGame();
    void    killGame();
    
    //  Other data
    //
    ofImage logo;
};
