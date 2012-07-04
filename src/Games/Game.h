//
//  Game.h
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxInteractiveSurface.h"

class Game {
public:
    Game(){};
    
    virtual void    init(ofRectangle _space){};                 //  The game it´s initialized seting the space
                                                                //  space area where the interactions it´s going 
                                                                //  to happend. This responces to the perimeter
                                                                //  ( contour ) of the tracked surface
    virtual void    reset(){};
    
    int             getWidth() const { return width;};
    int             getHeight() const { return height;};
    ofxTrackMode    getTrackMode() const { return trackMode;};  //  See ofxTrackMode on ofxInteractiveSurface
                                                                //  It's important to set the right trackMode
                                                                //  this will deliver hands/object events of
                                                                //  both with diferent creiterials
    //
    //  Hand Events: the blobs are hands (or other objects) over the surface
    //  that are not touching the surface but are over it.
    //
    virtual void    handAdded(ofxBlob &_blob) {};
    virtual void    handMoved(ofxBlob &_blob) {};
    virtual void    handDeleted(ofxBlob &_blob) {};
    
    //
    //  Objets Events: anything that touch the surface it's consider a objetc
    //
    virtual void    objectAdded(ofxBlob &_blob) {};
    virtual void    objectMoved(ofxBlob &_blob) {};
    virtual void    objectDeleted(ofxBlob &_blob) {};
    
    virtual void    update(){};     //  Update game
    virtual void    render(){};     //  Render the game inside a FBO. 
                                    //  The owner of the game then can call the 
                                    //  texture of the render with getTextureReference();
    
    ofTexture&      getTextureReference(){ return fbo.getTextureReference(); };
    
protected:
    ofRectangle     space;          //  TODO: this is not well designed. Found a better solution
                                    //  where the game it´s independent form the tracking area. 
                                    //  and the ofxInteractiveSurface more inteligent
    ofFbo           fbo;
    ofxTrackMode    trackMode;
    int             width, height;
};
