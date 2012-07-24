//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#include "testApp.h"

#ifndef SPANISH
string helpScreen = "\n \
- F1:   Fullscreen ON/OFF\n \
- F2:   Surface Edit-Mode on/off\n \
- F3:   Masking-Mode ON/OFF (need Edit-Mode ) \n \
\n \
        On mask mode on:\n \
        - x: delete mask path point\n \
        - r: reset mask path\n \
\n \
- F4:   Reset surface coorners\n \
\n \
- m/M:  show/hide mouse\n \
- d/D:  debug mode ON/OFF\n \
- h/H:  turn ON/OFF this help screen\n \
\n \
- l/L:  load/reload previus calibration setup and \n \
then load the game\n \
- n/N:  load the nextGame using previus calibration setup\n \
- c/C:  calibrate and then load the game \n";
#else
string helpScreen = "\n \
\n \
- m/M:  Ocultar/Mostrar el mouse\n \
- d/D:  Ocultar/Mostrar lo que ve la compu\n \
- h/H:  Ocultar/Mostrar este mensaje de ayuda\n \
\n \
- l/L:  Volver a cargar el juego\n \
- n/N:  Cargar el siguiente juego\n \
- c/C:  Calibrar la mesa\n \
\n \
- F1:   Pantalla completa\n \
- F2:   Editar la forma y posicion\n \
- F3:   Editar la masca (borde del juego)\n \
\n \
        Una vez activo con:\n \
        - x: borra punto por punto\n \
        - r: borra todos los puntos\n \
\n \
- F4:   Re inicia la forma y posicion de los juegos\n";
#endif

//-------------------------------------------------------------- SETING
void testApp::setup(){
    ofEnableAlphaBlending();
    //ofEnableSmoothing();
    ofSetVerticalSync(true);
    ofSetFrameRate(60);

    //  Load the logo
    logo.loadImage("logo.jpg");
    
    //  Event listeners from tSurfaces to the bridge methoths. 
    ofAddListener(iSurface.calibrationDone, this, &testApp::calibrationDone);
    ofAddListener(iSurface.handAdded, this, &testApp::handAdded);
    ofAddListener(iSurface.handMoved, this, &testApp::handMoved);
    ofAddListener(iSurface.handDeleted, this, &testApp::handDeleted);
    ofAddListener(iSurface.objectAdded, this, &testApp::objectAdded);
    ofAddListener(iSurface.objectMoved, this, &testApp::objectMoved);
    ofAddListener(iSurface.objectDeleted, this, &testApp::objectDeleted);
    
    //  Set everthing OFF for default
    //
    game = NULL;
    bStart = false;
    bHelp = false;
    bMouse = false;
    iSurface.bDebug = false;
    
    blobIDSimulator = 0;
    
    ofHideCursor();
    
    //  Load the Game
    //
    loadGame();
}

void testApp::loadGame(){
    
    ofxXmlSettings XML;
    
    if (XML.loadFile("settings.xml")){
        activeGameName  = XML.getValue("game", "pong");
        
        killGame();         //  if it«s a game running kill it
        
        iSurface.load();    //  Re-load the game. This will end in 
                            //  a "calibrationDone" event. That will l
                            //  anch the [sGameName] game
        
        ofLog(OF_LOG_NOTICE, "Loading previus calibration data");
    } else
        ofLog(OF_LOG_ERROR,"Fail to load game configuration xml");
}

void testApp::loadGame(string _gameName){
    
    ofxXmlSettings XML;
    
    if (XML.loadFile("settings.xml")){
        XML.setValue("game", _gameName);
        XML.saveFile();
        
        activeGameName  = _gameName;
        
        killGame();         //  if it«s a game running kill it
        
        iSurface.load();    //  Re-load the game. This will end in 
                            //  a "calibrationDone" event. That will l
                            //  anch the [sGameName] game
        
        ofLog(OF_LOG_NOTICE, "Seting game to " + _gameName);
    } else
        ofLog(OF_LOG_ERROR,"Fail to load game configuration xml");
}

void testApp::loadNextGame(){
    
    string list[] = {"shadows","simon","oca","kaleido","communitas"};
    for( int i = 0; i < TOTALGAMES; i++){
        if ( activeGameName == list[i] ){
            loadGame( list[(i+1)%TOTALGAMES] );
            break;
        }
    }
}

//-------------------------------------------------------------- LOOP
void testApp::update(){
    
    //  Updatign tSurface mantein the mapping between the game texture and 
    //  the tracking objects/hands. Also if it«s call do the calibration
    //
    iSurface.update();
    
    //  If the game starts. Update it
    if (bStart){
        game->update();
        game->render();
    }
}

void testApp::draw(){
    if (iSurface.bDebug)
        ofBackgroundGradient(ofColor::gray, ofColor::black);
    else
        ofBackground(ofColor::black);
    
    if (bStart){
        iSurface.draw(game->getTextureReference());
    } else {
        iSurface.draw(logo.getTextureReference());
    }

    if (bHelp){
        ofSetColor(0,200);
        ofRect(0,0,ofGetWidth(),ofGetHeight());
        ofSetColor(255,255);
        ofDrawBitmapString(helpScreen, 150,250);
    }
}

//-------------------------------------------------------------- EVENTS

void testApp::calibrationDone(ofPolyline &_surface){
    ofLog(OF_LOG_NOTICE, "Calibration done");
    
    if ( bStart )
        killGame();
    
    if (activeGameName == "simon"){
        game = new Simon();
    } else if (activeGameName == "pong"){
        game = new Pong();
    } else if (activeGameName == "shadows"){
        game = new Shadows();
    } else if (activeGameName == "oca"){
        game = new Oca();
    } else if (activeGameName == "communitas"){
        game = new Communitas();
    } else if (activeGameName == "kaleido"){
        game = new Kaleido();
    }
    
    if (game != NULL){
        ofLog(OF_LOG_NOTICE, "Game " + activeGameName + " loaded");
        game->init( _surface.getBoundingBox() );
        iSurface.setTrackMode( game->getTrackMode() );
        bStart = true;
    } else {
        ofLog(OF_LOG_ERROR, "Game " + activeGameName + " not loaded.");
        bStart = false;
    }
}

void testApp::killGame(){
    if ( bStart ){
        delete game;
        game = NULL;
        ofLog(OF_LOG_NOTICE, "Game killed!");
    }
    
    bStart = false;
}

void testApp::handAdded(ofxBlob &_blob){
    if (bStart && (game != NULL )){
        game->handAdded(_blob);
    }
}

void testApp::handMoved(ofxBlob &_blob){
    if (bStart && (game != NULL )){
        game->handMoved(_blob);
    }
}

void testApp::handDeleted(ofxBlob &_blob){
    if (bStart && (game != NULL )){
        game->handDeleted(_blob);
    }
}

void testApp::objectAdded(ofxBlob &_blob){
    if (bStart && (game != NULL )){
        game->objectAdded(_blob);
    }
}

void testApp::objectMoved(ofxBlob &_blob){
    if (bStart && (game != NULL )){
        game->objectMoved(_blob);
    }
}
void testApp::objectDeleted(ofxBlob &_blob){
    if (bStart && (game != NULL )){
        game->objectDeleted(_blob);
    }
}

void testApp::keyPressed(int key){
    switch (key) {
        case OF_KEY_F1:
            ofToggleFullscreen();
            break;
        case 'd':
        case 'D':
            iSurface.bDebug = !iSurface.bDebug;
            break;
        case 'c':
        case 'C':
            bStart = false;
            delete game;
            iSurface.calibrate();
            break;
        case 'l':
        case 'L':
            loadGame();
            break;
        case 'r':
        case 'R':
            game->reset();
            break;
        case 'h':
        case 'H':
            bHelp = !bHelp;
            break; 
        case 'n':
        case 'N':
            loadNextGame();
            break; 
        case 'm':
        case 'M':
            bMouse = !bMouse;
            break; 
        case '1':
            loadGame("shadows");
            break; 
        case '2':
            loadGame("simon");
            break; 
        case '3':
            loadGame("oca");
            break;
        case '4':
            loadGame("kaleido");
            break; 
        case '5':
            loadGame("communitas");
            break; 
    }
    
    if (bMouse){
        ofShowCursor();
    } else {
        ofHideCursor();
    }
}

void testApp::keyReleased(int key){
}

void testApp::mouseMoved(int x, int y ){
}

void testApp::mousePressed(int x, int y, int button){
    if ((game != NULL) && bMouse && !iSurface.bDebug){
        ofPoint mouse = ofPoint(x,y);
        
        if ( iSurface.getView().isOver(mouse) && !iSurface.getView().bEditMode ){
            ofxBlob pretendBlob;
            
            pretendBlob.id = blobIDSimulator;
            
            ofPoint  pos = iSurface.getView().getScreenToSurface(mouse);
            blobResolution = ofRandom(3,20);
            blobRadio = ofRandom(10.0,30.0);
            blobAngle = (PI*2.0)/(float)blobResolution;
            
            pretendBlob.pts.clear();
            for(int i = 0; i < blobResolution + 1; i++){
                ofPoint pt;
                pt.x = (blobRadio * cos( blobAngle*i) + pos.x)/(float)iSurface.getView().getWidth();
                pt.y = (blobRadio * sin( blobAngle*i) + pos.y)/(float)iSurface.getView().getHeight();
                pretendBlob.pts.push_back(pt);
            }
            pretendBlob.nPts = blobResolution;
            pretendBlob.nFingers = 0;
            
            if ((game->getTrackMode() == TRACK_JUST_OBJECT) || 
                (game->getTrackMode() == TRACK_ACTIVE_OBJECT) ||
                (game->getTrackMode() == TRACK_BOTH)){
                pretendBlob.centroid = iSurface.getView().getScreenToSurface(mouse);
                pretendBlob.centroid.x /= iSurface.getView().getWidth();
                pretendBlob.centroid.y /= iSurface.getView().getHeight();
                objectAdded(pretendBlob);
            }
            
            if ((game->getTrackMode() == TRACK_JUST_HANDS) || 
                (game->getTrackMode() == TRACK_ACTIVE_HANDS) ||
                (game->getTrackMode() == TRACK_BOTH)){
                pretendBlob.palm    = iSurface.getView().getScreenToSurface(mouse);
                pretendBlob.palm.x /= iSurface.getView().getWidth();
                pretendBlob.palm.y /= iSurface.getView().getHeight();
                pretendBlob.fingers.push_back(iSurface.getView().getScreenToSurface(mouse));
                pretendBlob.gotFingers  = true;
                handAdded(pretendBlob);
            }
        }
    }
}

void testApp::mouseDragged(int x, int y, int button){
    if ((game != NULL) && bMouse && !iSurface.bDebug){
        ofPoint mouse = ofPoint(x,y);
        
        if ( iSurface.getView().isOver(mouse) && !iSurface.getView().bEditMode ){
            ofxBlob pretendBlob;
            
            pretendBlob.id = blobIDSimulator;
            
            ofPoint  pos = iSurface.getView().getScreenToSurface(mouse);
            
            pretendBlob.pts.clear();
            for(int i = 0; i < blobResolution + 1; i++){
                ofPoint pt;
                pt.x = (blobRadio * cos( blobAngle*i) + pos.x)/(float)iSurface.getView().getWidth();
                pt.y = (blobRadio * sin( blobAngle*i) + pos.y)/(float)iSurface.getView().getHeight();
                pretendBlob.pts.push_back(pt);
            }
            pretendBlob.nPts = blobResolution;
            pretendBlob.nFingers = 0;
            
            if ((game->getTrackMode() == TRACK_JUST_OBJECT) || 
                (game->getTrackMode() == TRACK_ACTIVE_OBJECT) ||
                (game->getTrackMode() == TRACK_BOTH)){
                pretendBlob.centroid = iSurface.getView().getScreenToSurface(mouse);
                pretendBlob.centroid.x /= iSurface.getView().getWidth();
                pretendBlob.centroid.y /= iSurface.getView().getHeight();
                objectMoved(pretendBlob);
            }
            
            if ((game->getTrackMode() == TRACK_JUST_HANDS) || 
                (game->getTrackMode() == TRACK_ACTIVE_HANDS) ||
                (game->getTrackMode() == TRACK_BOTH)){
                pretendBlob.palm    = iSurface.getView().getScreenToSurface(mouse);
                pretendBlob.palm.x /= iSurface.getView().getWidth();
                pretendBlob.palm.y /= iSurface.getView().getHeight();
                pretendBlob.fingers.clear();
                pretendBlob.gotFingers  = true;
                handMoved(pretendBlob);
            }
        }
    }
}

void testApp::mouseReleased(int x, int y, int button){
    if (game != NULL){
        ofPoint mouse = ofPoint(x,y);
        
        if ( iSurface.getView().isOver(mouse) && !iSurface.getView().bEditMode ){
            ofxBlob pretendBlob;
            
            pretendBlob.id = blobIDSimulator;
            
            ofPoint  pos = iSurface.getView().getScreenToSurface(mouse);
            
            pretendBlob.pts.clear();
            for(int i = 0; i < blobResolution + 1; i++){
                ofPoint pt;
                pt.x = (blobRadio * cos( blobAngle*i) + pos.x)/(float)iSurface.getView().getWidth();
                pt.y = (blobRadio * sin( blobAngle*i) + pos.y)/(float)iSurface.getView().getHeight();
                pretendBlob.pts.push_back(pt);
            }
            pretendBlob.nPts = blobResolution;
            pretendBlob.nFingers = 0;
            
            if ((game->getTrackMode() == TRACK_JUST_OBJECT) || 
                (game->getTrackMode() == TRACK_ACTIVE_OBJECT) ||
                (game->getTrackMode() == TRACK_BOTH)){
                pretendBlob.centroid = iSurface.getView().getScreenToSurface(mouse);
                pretendBlob.centroid.x /= iSurface.getView().getWidth();
                pretendBlob.centroid.y /= iSurface.getView().getHeight();
                objectDeleted(pretendBlob);
            }
            
            if ((game->getTrackMode() == TRACK_JUST_HANDS) || 
                (game->getTrackMode() == TRACK_ACTIVE_HANDS) ||
                (game->getTrackMode() == TRACK_BOTH)){
                pretendBlob.palm    = iSurface.getView().getScreenToSurface(mouse);
                pretendBlob.palm.x /= iSurface.getView().getWidth();
                pretendBlob.palm.y /= iSurface.getView().getHeight();
                pretendBlob.gotFingers  = true;
                objectDeleted(pretendBlob);
            }
        }
    }
    
    blobIDSimulator++;
}

void testApp::windowResized(int w, int h){
}

void testApp::gotMessage(ofMessage msg){
}

void testApp::dragEvent(ofDragInfo dragInfo){
}

void testApp::exit(){
    if (game != NULL)
        game->reset();
    
    iSurface.exit();
}