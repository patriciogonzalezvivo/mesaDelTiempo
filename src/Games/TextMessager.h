//
//  TextMessage.h
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//
#ifndef TEXTMESSAGER
#define TEXTMESSAGER

#include "ofMain.h"

#include "TextSequencer.h"
#include "ofxXmlSettings.h"


class TextMessager: public TextSequencer {
public:
    TextMessager();
    
    bool    loadStyle(string _xmlFile);
    
    float   getNormTransitionValue();
    
    void    addMessage(string _message );
    void    addMessage( textPhrase &_phrase );
    
    void    speedUp(){ speed *= 2.0;}
    
    void    update();
    void    draw();
    
    float   angle;
private:
    textPhrase  message;
    bool        bMessage;
};

#endif
