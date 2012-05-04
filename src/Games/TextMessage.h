//
//  TextMessage.h
//  mdt-Core
//
//  Created by Patricio González Vivo on 5/3/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef TEXTMESSAGE
#define TEXTMESSAGE

#include "ofMain.h"

#include "TextSequencer.h"
#include "ofxXmlSettings.h"


class TextMessage: public TextSequencer {
public:
    TextMessage();
    
    bool    loadStyle(string _xmlFile);
    
    void    addMessage(string _message );
    void    addMessage( textPhrase &_phrase );
    
    void    update();
};

#endif
