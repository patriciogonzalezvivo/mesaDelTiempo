//
//  TextSequencer.cpp
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#include "TextSequencer.h"

TextSequencer::TextSequencer(){
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

bool TextSequencer::loadSequence(string _xmlFile){
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
        
        int totalLines = XML.getNumTags("phrase");
        for(int i = 0; i < totalLines; i++){
            XML.pushTag("phrase",i);
            textPhrase newPhrase;
            
            //  Text Phrase
            //
            newPhrase.text      =   XML.getValue("text", "NO TEXT FOUND");
            newPhrase.seconds   =   newPhrase.text.length() * secForChar;
            
            //  Atributes
            //
            newPhrase.fontFile  =   XML.getValue("font:file", defaultFontFile);
            newPhrase.fontSize  =   XML.getValue("font:size", defaultFontSize);
            newPhrase.fontDpi   =   XML.getValue("font:dpi", defaultFontDpi);
            newPhrase.speed     =   XML.getValue("speed", defaultSpeed);
            newPhrase.scale     =   XML.getValue("scale", 1.0);
            newPhrase.spin      =   XML.getValue("spin", defaultSpin);
            
            string shape = XML.getValue("shape", defShape);
            if (shape == "BLOCK"){
                newPhrase.shape = OF_TEXT_SHAPE_BLOCK;
            } else if (shape == "ARC"){
                newPhrase.shape = OF_TEXT_SHAPE_ARC;
            } else if (shape == "SPIRAL"){
                newPhrase.shape = OF_TEXT_SHAPE_SPIRAL;
            }
            
            string alignment = XML.getValue("hAlign", defVerAlign);
            if (alignment == "LEFT"){
                newPhrase.hAlign = OF_TEXT_ALIGN_LEFT;
            } else if ( alignment == "RIGHT"){
                newPhrase.hAlign = OF_TEXT_ALIGN_RIGHT;
            } else if ( alignment == "JUSTIFIED"){
                newPhrase.hAlign = OF_TEXT_ALIGN_JUSTIFIED;
            } else if ( alignment == "CENTER"){
                newPhrase.hAlign = OF_TEXT_ALIGN_CENTER;
            }
            
            alignment = XML.getValue("vAlign", defHorAlign);
            if (alignment == "TOP"){
                newPhrase.vAlign = OF_TEXT_ALIGN_TOP;
            } else if ( alignment == "BOTTOM"){
                newPhrase.vAlign = OF_TEXT_ALIGN_BOTTOM;
            } else if ( alignment == "MIDDLE"){
                newPhrase.vAlign = OF_TEXT_ALIGN_MIDDLE;
            } 
            
            script.push_back(newPhrase);
            
            XML.popTag();
        }
        
        countDown = secBetweenPhrase;
        currentLine = -1;
        bWaiting = true;
    } else {
        ofLog(OF_LOG_ERROR, "File " + ofToDataPath(_xmlFile) + " could not be opened" );
    }

    return success;
}

void TextSequencer::setNextPhrase(textPhrase &_phrase ){
    rawText     = _phrase.text;
    spin        = _phrase.spin;

    if (spin > 0){
        seconds = (rawText.length() + spin*2) * secForChar;
    } else {
        seconds = _phrase.seconds;
    }
    
    countDown   = seconds;
    speed       = _phrase.speed;

    delete  text;
    text = NULL;
    
    switch (_phrase.shape) {
        case OF_TEXT_SHAPE_BLOCK:
            text = new ofxTextBlock();
            break;
        case OF_TEXT_SHAPE_ARC:
            text = new ofxTextArc();
            break;
        case OF_TEXT_SHAPE_SPIRAL:
            text = new ofxTextSpiral();
            break;
    }
    
    if (text != NULL){
        text->set(x,y,width,height);
        text->loadFont( _phrase.fontFile, _phrase.fontSize, _phrase.fontDpi );
        text->setScale( _phrase.scale );
        text->setText( _phrase.text );
        text->setAlignment(_phrase.hAlign, _phrase.vAlign);
    }
}

void TextSequencer::update(){
    
    countDown -= (1/ofGetFrameRate())*speed;
    
    if (bWaiting){
        
        if ( countDown <= 0){
            
            //  If the waiting ends load the a phrase
            //
            bWaiting = false;
            
            //  If there is not any message on the pull
            //  jump to next phrase of the script
            //
            currentLine = (currentLine+1)%script.size();
            setNextPhrase( script[currentLine] );
        }
    } else {
        if ((text != NULL) && ( spin > 0)) {
            text->setText( spinningString( rawText, spin , (1.0-(countDown/seconds))*(rawText.size()+spin) ));
        }
        
        if ( countDown <= 0){
            bWaiting    = true;
            countDown   = secBetweenPhrase;
            speed       = 1.0;
        }
    }
}

void TextSequencer::draw(){
    if ( !bWaiting && (script.size() > 0) ){
        if (text != NULL){
            text->draw();
        } else
            ofLog(OF_LOG_ERROR,"Text trying to be render with out loading it");
    } 
}

string TextSequencer::spinningString(string _orginalText, int _nChars, int _offset){
    string rta;
    
    int indexText = 0;
    
    if ( _offset > _nChars){
        indexText = _offset - _nChars;
    }
    for (int i = 0; i < (_orginalText.size()+_nChars); i++){
        
        if ( i < _nChars){
            if ( i >= _nChars - _offset){
                if ( indexText < _orginalText.size())
                    rta += _orginalText[indexText];
                else 
                    rta += " ";
                indexText++;
            } else 
                rta += " ";
        }
    }
    
    return rta;
}