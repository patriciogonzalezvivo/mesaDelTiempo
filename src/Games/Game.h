//
//  Game.h
//  mdt-Core
//
//  Created by Patricio González Vivo on 4/2/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef mdt_Core_Game_h
#define mdt_Core_Game_h

#include "ofMain.h"
#include "ofxBlob.h"

class Game {
public:
    Game(){};
    
    virtual void    init(ofRectangle _space){};
    virtual void    reset(){};
    
    int             getWidth() const { return width;};
    int             getHeight() const { return height;};
    
    virtual void    handAdded(ofxBlob &_blob) {};
    virtual void    handMoved(ofxBlob &_blob) {};
    virtual void    handDeleted(ofxBlob &_blob) {};
    
    virtual void    objectAdded(ofxBlob &_blob) {};
    virtual void    objectMoved(ofxBlob &_blob) {};
    virtual void    objectDeleted(ofxBlob &_blob) {};
    
    virtual void    update(){};
    
    ofTexture&  getTextureReference(){ return fbo.getTextureReference(); };
    
protected:
    ofRectangle space;
    
    ofFbo       fbo;
    
    int         width, height;
};


#endif
