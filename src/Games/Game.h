//
//  Game.h
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxTrackedSurface.h"

class Game {
public:
    Game(){};
    
    virtual void    init(ofRectangle _space){};
    virtual void    reset(){};
    
    int             getWidth() const { return width;};
    int             getHeight() const { return height;};
    TrackedSurface  getTrackedSurfaceID() const { return trackedSurfaceID;};
    
    virtual void    handAdded(ofxBlob &_blob) {};
    virtual void    handMoved(ofxBlob &_blob) {};
    virtual void    handDeleted(ofxBlob &_blob) {};
    
    virtual void    objectAdded(ofxBlob &_blob) {};
    virtual void    objectMoved(ofxBlob &_blob) {};
    virtual void    objectDeleted(ofxBlob &_blob) {};
    
    virtual void    update(){};
    virtual void    render(){};
    
    ofTexture&      getTextureReference(){ return fbo.getTextureReference(); };
    
protected:
    ofRectangle     space;
    ofFbo           fbo;
    
    TrackedSurface  trackedSurfaceID;
    
    int             width, height;
};
