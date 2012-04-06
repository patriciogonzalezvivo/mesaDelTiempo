 #include "testApp.h"

//-------------------------------------------------------------- SETING
void testApp::setup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetVerticalSync(false);
    
    logo.loadImage("logo.jpg");
    
    ofAddListener(tSurface.calibrationDone, this, &testApp::calibrationDone);
    ofAddListener(tSurface.handAdded, this, &testApp::handAdded);
    ofAddListener(tSurface.handMoved, this, &testApp::handMoved);
    ofAddListener(tSurface.handDeleted, this, &testApp::handDeleted);
    ofAddListener(tSurface.objectAdded, this, &testApp::objectAdded);
    ofAddListener(tSurface.objectMoved, this, &testApp::objectMoved);
    ofAddListener(tSurface.objectDeleted, this, &testApp::objectDeleted);
    
    //  Set everthing OFF for default
    //
    game = NULL;
    bStart = false;
    bUpdateHands = false;
    bUpdateObjects = false;
    tSurface.bDebug = false;
    
    //  Load the Game
    //
    loadGame();
}

void testApp::loadGame(){
    
    ofxXmlSettings XML;
    
    if (XML.loadFile("settings.xml")){
        sGameName   = XML.getValue("game", "pong");
        bUpdateHands = XML.getValue("updatehands", 1);
        bUpdateObjects = XML.getValue("updateobjects", 1);
        
        killGame();         //  if it«s a game running kill it
        
        tSurface.load();    //  Re-load the game. This will end in 
                            //  a "calibrationDone" event. That will l
                            //  anch the [sGameName] game
        
        ofLog(OF_LOG_NOTICE, "Loading previus calibration data");
    } else
        ofLog(OF_LOG_ERROR,"Fail to load game configuration xml");
}

//-------------------------------------------------------------- LOOP
void testApp::update(){
    
    //  Updatign tSurface mantein the mapping between the game texture and 
    //  the tracking objects/hands. Also if it«s call do the calibration
    //
    tSurface.update(bUpdateObjects,bUpdateHands);
    
    //  If the game starts. Update it
    if (bStart){
        game->update();
    }
}

void testApp::draw(){
    if (tSurface.bDebug)
        ofBackgroundGradient(ofColor::gray, ofColor::black);
    else
        ofBackground(ofColor::black);
    
    if (bStart){
        tSurface.draw(game->getTextureReference());
    } else {
        tSurface.draw(logo.getTextureReference());
    }
    
}

//-------------------------------------------------------------- EVENTS

void testApp::calibrationDone(ofPolyline &_surface){
    ofLog(OF_LOG_NOTICE, "Calibration done");
    
    if ( bStart )
        killGame();
    
    if (sGameName == "simon"){
        game = new Simon();
        ofLog(OF_LOG_NOTICE, "Loading Simon game");
        game->init( _surface.getBoundingBox() );
        bStart = true;
    } else if (sGameName == "pong"){
        game = new Pong();
        ofLog(OF_LOG_NOTICE, "Loading Pong game");
        game->init( _surface.getBoundingBox() );
        bStart = true;
    } else if (sGameName == "shadows"){
        game = new Shadows();
        ofLog(OF_LOG_NOTICE, "Loading Shadows game");
        game->init( _surface.getBoundingBox() );
        bStart = true;
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
            tSurface.bDebug = !tSurface.bDebug;
            break;
        case 'c':
        case 'C':
            bStart = false;
            delete game;
            tSurface.calibrate();
            break;
        case 'l':
        case 'L':
            bStart = false;
            delete game;
            tSurface.load();
            break;
    }
}

void testApp::keyReleased(int key){
}

void testApp::mouseMoved(int x, int y ){
}

void testApp::mousePressed(int x, int y, int button){
    ofPoint mouse = ofPoint(x,y);
    
    if ( tSurface.getSurface().isOver(mouse) ){
        ofxBlob pretendBlob;
    
        if (bUpdateObjects){
            pretendBlob.centroid = tSurface.getSurface().getScreenToSurface(mouse);
            pretendBlob.centroid.x /= tSurface.getSurface().getWidth();
            pretendBlob.centroid.y /= tSurface.getSurface().getHeight();
            objectAdded(pretendBlob);
        }
        
        if (bUpdateHands){
            pretendBlob.palm    = tSurface.getSurface().getScreenToSurface(mouse);
            pretendBlob.palm.x /= tSurface.getSurface().getWidth();
            pretendBlob.palm.y /= tSurface.getSurface().getHeight();
            pretendBlob.gotFingers  = true;
            handAdded(pretendBlob);
        }
    }
}

void testApp::mouseDragged(int x, int y, int button){
    ofPoint mouse = ofPoint(x,y);
    
    if ( tSurface.getSurface().isOver(mouse) ){
        ofxBlob pretendBlob;
        
        if (bUpdateObjects){
            pretendBlob.centroid = tSurface.getSurface().getScreenToSurface(mouse);
            pretendBlob.centroid.x /= tSurface.getSurface().getWidth();
            pretendBlob.centroid.y /= tSurface.getSurface().getHeight();
            objectMoved(pretendBlob);
        }
        
        if (bUpdateHands){
            pretendBlob.palm    = tSurface.getSurface().getScreenToSurface(mouse);
            pretendBlob.palm.x /= tSurface.getSurface().getWidth();
            pretendBlob.palm.y /= tSurface.getSurface().getHeight();
            pretendBlob.gotFingers  = true;
            handMoved(pretendBlob);
        }
    }
}

void testApp::mouseReleased(int x, int y, int button){
    ofPoint mouse = ofPoint(x,y);
    
    if ( tSurface.getSurface().isOver(mouse) ){
        ofxBlob pretendBlob;
        
        if (bUpdateObjects){
            pretendBlob.centroid = tSurface.getSurface().getScreenToSurface(mouse);
            pretendBlob.centroid.x /= tSurface.getSurface().getWidth();
            pretendBlob.centroid.y /= tSurface.getSurface().getHeight();
            objectDeleted(pretendBlob);
        }
        
        if (bUpdateHands){
            pretendBlob.palm    = tSurface.getSurface().getScreenToSurface(mouse);
            pretendBlob.palm.x /= tSurface.getSurface().getWidth();
            pretendBlob.palm.y /= tSurface.getSurface().getHeight();
            pretendBlob.gotFingers  = true;
            handDeleted(pretendBlob);
        }
    }
}

void testApp::windowResized(int w, int h){
}

void testApp::gotMessage(ofMessage msg){
}

void testApp::dragEvent(ofDragInfo dragInfo){
}

void testApp::exit(){
    tSurface.exit();
}