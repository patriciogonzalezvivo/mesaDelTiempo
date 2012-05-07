 #include "testApp.h"

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
    - d/D:  debug mode ON/OFF\n \
    - h/H:  turn ON/OFF this help screen\n \
    \n \
    - l/L:  load/reload previus calibration setup and then load the game\n \
    - c/C:  calibrate and then load the game \n";

//-------------------------------------------------------------- SETING
void testApp::setup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
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
    bHelp = false;
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
        game->render();
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

    if (bHelp){
        ofSetColor(255);
        ofDrawBitmapString(helpScreen, 100,100);
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
        bUpdateHands = false;
        bUpdateObjects = true;
        bStart = true;
    } else if (sGameName == "pong"){
        game = new Pong();
        ofLog(OF_LOG_NOTICE, "Loading Pong game");
        game->init( _surface.getBoundingBox() );
        bUpdateHands = true;
        bUpdateObjects = false;
        bStart = true;
    } else if (sGameName == "shadows"){
        game = new Shadows();
        ofLog(OF_LOG_NOTICE, "Loading Shadows game");
        game->init( _surface.getBoundingBox() );
        bUpdateHands = true;
        bUpdateObjects = false;
        bStart = true;
    } else if (sGameName == "oca"){
        game = new Oca();
        ofLog(OF_LOG_NOTICE, "Loading Oca game");
        game->init( _surface.getBoundingBox() );
        bUpdateHands = false;
        bUpdateObjects = true;
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
        case 'r':
        case 'R':
            game->reset();
            break;
        case 'h':
        case 'H':
            bHelp = !bHelp;
            break;    
    }
}

void testApp::keyReleased(int key){
}

void testApp::mouseMoved(int x, int y ){
}

void testApp::mousePressed(int x, int y, int button){
    ofPoint mouse = ofPoint(x,y);
    
    if ( tSurface.getSurface().isOver(mouse) && !tSurface.getSurface().bEditMode){
        ofxBlob pretendBlob;
        
        pretendBlob.id = 0;
    
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
    
    if ( tSurface.getSurface().isOver(mouse) && !tSurface.getSurface().bEditMode ){
        ofxBlob pretendBlob;
        
        pretendBlob.id = 0;
        
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
    
    if ( tSurface.getSurface().isOver(mouse) && !tSurface.getSurface().bEditMode){
        ofxBlob pretendBlob;
        
        pretendBlob.id = 0;
        
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
    if (game != NULL)
        game->reset();
    
    tSurface.exit();
}