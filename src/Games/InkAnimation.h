//
//  TransitionRect.h
//  mdt-Core
//
//  Created by Patricio González Vivo on 5/4/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#pragma once

#include "ofMain.h"

#include "TextSequencer.h"

#include "ofxFX.h"
#include "ofxSwapBuffer.h"

class InkAnimation: public ofRectangle {
public:
    InkAnimation(){
        x = 0;
        y = 0;
        width = 0;
        height = 0;
    }
    
    InkAnimation(ofRectangle const & _rect){
        set(_rect);
    }
    
    void set (float px, float py, float w, float h){
        x		= px;
        y		= py;
        width	= w;
        height	= h;
        
        blur.allocate( width , height );
        tint.allocate( width , height );
        tint.setZoom(60);
    }

    void    set( ofRectangle const & _rect){
        x = _rect.x;
        y = _rect.y;
        width = _rect.width;
        height = _rect.height;
        
        blur.allocate( width , height );
        tint.allocate( width , height );
        tint.setZoom(60);
    }
    
    void    update( TextSequencer & _tSeq ){
        ofPushView();
        ofPushStyle();
        
        begin();
        ofPushMatrix();
        //ofTranslate(getCenter().x - _tSeq.width*0.5,getCenter().y - _tSeq.height*0.5);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofClear(0, 255);
        ofSetColor( ofMap( _tSeq.getNormTransitionValue(), 0.0, 0.5, 0.0, 1.0, true) * 255,255);
        //ofDrawBitmapString("TEST", ofPoint(width*0.5,height*0.5));
        _tSeq.draw();
        
        ofDisableBlendMode();
        ofPopMatrix();
        end();
        
        update(_tSeq.getNormTransitionValue());
        
        ofPopView();
        ofPopStyle();
    }
    
    void    begin(){
        blur.begin();
    }
    
    void    end(){
        blur.end();
    }
    
    void    update( float _transFade ){

        blur.setRadius( 1.0 + ( 1.0-ofMap(_transFade , 0.0, 0.5, 0.0, 1.0, true) ) * 10 );
        blur.setPasses( 1.0 + ( 1.0-ofMap(_transFade, 0.0, 0.5, 0.0, 1.0, true) ) * 10 );
        blur.update();
        
        tint.setFade( 0.2 + (1.0- _transFade ) *0.8 );
        
        if ( _transFade < 0.01 ){
            tint.clear();
        }
        
        tint << blur;
        tint.update();
    }
    
    void draw(){
        //ofSetColor(255,255);
        tint.getTextureReference().draw(x,y,width,height);
    }
    
    ofxBlur         blur;
    ofxTint         tint;
    
};
