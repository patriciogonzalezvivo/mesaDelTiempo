//
//  TextMessage.cpp
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 5/3/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#include "TextMessager.h"


TextMessager::TextMessager(){
    countDown   = 1.0;
    seconds     = 1.0;
    speed       = 1.0;
    spin        = 0;
    angle       = 0;
    
    defaultFontFile     =   "helvetica.ttf";
    defaultFontSize     =   30.0;
    defaultFontDpi      =   90;
    defaultShape        =   OF_TEXT_SHAPE_BLOCK;

    text        = NULL;
    bMessage    = false;
    bWaiting    = true;
}

bool TextMessager::loadStyle(string _xmlFile){
    bool    success = false;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_xmlFile)){
        
        //  Font
        //
        defaultFontFile     = XML.getValue("default:font:file", "helvetica.ttf");
        defaultFontSize     = XML.getValue("default:font:size", 30.0);
        defaultFontDpi      = XML.getValue("default:font:dpi", 90);
        
        defaultSpeed        = XML.getValue("default:speed", 1.0); 
        secForChar          = XML.getValue("default:secForChar", 0.2);
        secBetweenPhrase    = XML.getValue("default:secBetweenPhrase", 2.0);
        
        defaultSpin         = XML.getValue("default:spin", 0);
        
        string defShape     = XML.getValue("default:shape", "BLOCK");
        if (defShape == "BLOCK"){
            defaultShape = OF_TEXT_SHAPE_BLOCK;
        } else if (defShape == "ARC"){
            defaultShape = OF_TEXT_SHAPE_ARC;
        } else if (defShape == "SPIRAL"){
            defaultShape = OF_TEXT_SHAPE_SPIRAL;
        }
        
        string defVerAlign = XML.getValue("default:vAlign", "MIDDLE");
        if (defVerAlign == "TOP"){
            defaultVertAlign = OF_TEXT_ALIGN_TOP;
        } else if ( defVerAlign == "BOTTOM"){
            defaultVertAlign = OF_TEXT_ALIGN_BOTTOM;
        } else if ( defVerAlign == "MIDDLE"){
            defaultVertAlign = OF_TEXT_ALIGN_MIDDLE;
        }
        
        string defHorAlign = XML.getValue("default:hAlign", "CENTER");
        if (defHorAlign == "LEFT"){
            defaultHoriAlign = OF_TEXT_ALIGN_LEFT;
        } else if ( defHorAlign == "RIGHT"){
            defaultHoriAlign = OF_TEXT_ALIGN_RIGHT;
        } else if ( defHorAlign == "JUSTIFIED"){
            defaultHoriAlign = OF_TEXT_ALIGN_JUSTIFIED;
        } else if ( defHorAlign == "CENTER"){
            defaultHoriAlign = OF_TEXT_ALIGN_CENTER;
        }
        
        countDown = secBetweenPhrase;
        bWaiting = true;
        bMessage = false;
    } else {
        ofLog(OF_LOG_ERROR, "File " + ofToDataPath(_xmlFile) + " could not be opened" );
    }
    
    return success;
}

void  TextMessager::addMessage(string _message){
    textPhrase  nPhrase;
    nPhrase.text    =   _message;
    nPhrase.seconds =   _message.length() * secForChar;
    nPhrase.speed   =   defaultSpeed;
    
    nPhrase.hAlign  =   defaultHoriAlign;
    nPhrase.vAlign  =   defaultVertAlign;
    nPhrase.scale   =   1.0;
    nPhrase.shape   =   defaultShape;
    nPhrase.fontFile=   defaultFontFile;
    nPhrase.fontSize=   defaultFontSize;
    nPhrase.fontDpi =   defaultFontDpi;
    nPhrase.spin    =   defaultSpin;
    
    addMessage(nPhrase);
}

void TextMessager::addMessage( textPhrase &_phrase ){

    message = _phrase;
    bMessage = true;
    
    if (bWaiting){
        countDown = 0;
    } else {
        speed *= 2.0;
    }
}

void TextMessager::update(){
    
    countDown -= (1/ofGetFrameRate())*speed;
    
    if (bWaiting){
        if ( countDown <= 0){
            
            //  If the waiting ends load the a phrase
            //
            bWaiting = false;
            
            if ( bMessage ){
                setNextPhrase( message );
                bMessage = false;
            } else {
                bWaiting = false;
                countDown = 1.0;
            }
        }
    } else {
        if ( (text != NULL) && (  spin > 0) ){
            text->setText(spinningString( rawText, spin , (1.0-(countDown/seconds))*(rawText.size()+spin) ));
        }
        
        if ( countDown <= 0){
            bWaiting    = true;
            countDown   = secBetweenPhrase;
            speed       = 1.0;
            
            if ( (text != NULL) && (!bMessage) ){
                rawText = " ";
                text->setText(rawText);
            }
        }
    }
}

void TextMessager::draw(){
    if ( !bWaiting ){
        if (text != NULL){
            
            if (angle != 0.0){
                ofPushMatrix();
                ofTranslate( ofRectangle::getCenter() );
                ofRotateZ( angle );
                ofTranslate(-ofRectangle::getCenter().x,
                            -ofRectangle::getCenter().y);
                text->draw();
                ofPopMatrix();
            } else {
                text->draw();
            }
        } else
            ofLog(OF_LOG_ERROR,"Text trying to be render with out loading it");
    } 
}

float TextMessager::getNormTransitionValue(){ 
    if ( bWaiting || ( rawText == " ") || (text == NULL) ){
        return 0;
    } else {  
        return sin( (countDown/seconds) * PI );
    }
};
