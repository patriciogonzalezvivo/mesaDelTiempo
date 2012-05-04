//
//  TextMessage.cpp
//  mdt-Core
//
//  Created by Patricio González Vivo on 5/3/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#include "TextMessage.h"


TextMessage::TextMessage(){
    currentLine = 0;
    countDown   = 1.0;
    seconds     = 1.0;
    speed       = 1.0;
    
    defaultFontFile     =   "helvetica.ttf";
    defaultFontSize     =   30.0;
    defaultFontDpi      =   90;
    defaultShape        =   OF_TEXT_SHAPE_BLOCK;
    
    bPlay       = false;
    
    text        = NULL;
}

bool TextMessage::loadStyle(string _xmlFile){
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
        
        string defVerAlign = XML.getValue("default:hAlign", "CENTER");
        if (defVerAlign == "TOP"){
            defaultVertAlign = OF_TEXT_ALIGN_TOP;
        } else if ( defVerAlign == "BOTTOM"){
            defaultVertAlign = OF_TEXT_ALIGN_BOTTOM;
        } else if ( defVerAlign == "MIDDLE"){
            defaultVertAlign = OF_TEXT_ALIGN_MIDDLE;
        }
        
        string defHorAlign = XML.getValue("default:vAlign", "TOP");
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
        currentLine = -1;
        bWaiting = true;
    } else {
        ofLog(OF_LOG_ERROR, "File " + ofToDataPath(_xmlFile) + " could not be opened" );
    }
    
    return success;
}

void  TextMessage::addMessage(string _message){
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

void TextMessage::addMessage( textPhrase &_phrase ){

    script.push_back(_phrase);
    
    if (bWaiting){
        countDown = 0;
    } else {
        float halfOfTime = seconds*0.5;
        if (countDown < halfOfTime){
            countDown = halfOfTime + ( halfOfTime - countDown ); 
        }
        
        speed *= 2.0;
    }
}

void TextMessage::update(){
    
    countDown -= (1/ofGetFrameRate())*speed;
    
    if (bWaiting){
        if ( countDown <= 0){
            
            //  If the waiting ends load the a phrase
            //
            bWaiting = false;
            
            if ( script.size() > 0){
                
                //  Delete the first one of the pull and read the 
                //  one that it´s on the front.
                //  
                script.erase(script.begin());
                currentLine = 0;
                setNextPhrase( script[currentLine] );
            } else {
                
                //  If there are no more phrases on the pull
                //  it stay on bWaiting mode for two more seconds
                //
                bWaiting = false;
                countDown = 2.0*speed;
            }
        }
    } else {
        if ((text != NULL) && ( spin > 0)) {
            text->setText(spinningString( rawText, spin , (1.0-(countDown/seconds))*(rawText.size()+spin) ));
        }
        
        if ( countDown <= 0){
            bWaiting    = true;
            countDown   = secBetweenPhrase;
            speed       = 1.0;
        }
    }
    
    
}
