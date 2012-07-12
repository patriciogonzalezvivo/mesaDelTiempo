//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#pragma once
#include "ofMain.h"

//  MAPPING & TRACKING
#include "ofxInteractiveSurface.h"

//  GAME
#include "Game.h"   // This is the parent game class

#define SPANISH
#define TOTALGAMES  5

#include "Pong.h"       //  Classic Ateri Pong game
#include "Simon.h"      //  Classic SIMON game
#include "Shadows.h"    //  Chinise Shadows game
#include "Oca.h"        //  Oca Game
#include "Communitas.h" //  Communitas http://patriciogonzalezvivo.com/communitas.html
#include "Kaleido.h"

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
    ofxInteractiveSurface   iSurface;   // it«s in charge of the matching the tracked objects/hands
                                        // with the proyected game.
    //  tSurface Events bridge
    void    calibrationDone(ofPolyline &_surface);  // Call when the calibration it«s done

    void    handAdded(ofxBlob &_blob);              // hand appear over the surface
    void    handMoved(ofxBlob &_blob);              // hand moved over the surface
    void    handDeleted(ofxBlob &_blob);            // hand deleted over the surface

    void    objectAdded(ofxBlob &_blob);            // something touch the surface
    void    objectMoved(ofxBlob &_blob);            // something it«s dragging the surface
    void    objectDeleted(ofxBlob &_blob);          // something release the surface

    //  GAME
    //  ---------------------------
    //
    void    loadGame();                 //  Load the game responding to sGameName
    void    loadGame(string _gameName); //  Set the sGameName and load it
    void    loadNextGame();             //  It have a list of games, and load the next one on the list
    void    killGame();                 //  Unload the game removing everthing from memory and point *game to NULL
        
    Game    *game;                  //  pointer to the actual game
    string  activeGameName;         //  game string name
    
    ofImage logo;
    
    float   blobRadio, blobAngle;
    int     blobIDSimulator;        //  fake ID for simulating blobs with mouse
    int     blobResolution;
    
    bool    bStart, bHelp, bMouse;  //  Flags that respond to the state of the game, 
                                    //  help message and mouse visibility
};
