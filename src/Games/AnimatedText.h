//
//  AnimatedText.h
//  animatedText
//
//  Created by Patricio González Vivo on 4/9/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef ANIMATEDTEXT
#define ANIMATEDTEXT

#include "ofMain.h"
#include "ofxXmlSettings.h"

typedef struct {
    string  text;
    float   speed;
    float   seconds;
    int     align;
} StringLine;

typedef struct{  
    string character;  
    string code;  
} charSubstitution; 

class AnimatedText: public ofRectangle {
public:
    AnimatedText();
    
    bool    load(string _xmlFile);
    void    setSpeed(float _speed){speed = _speed;};
    void    setWaitingTime( float _time){ waitingTime = _time;};
    
    float   getNormTransitionValue();
    void    draw();
    
private:
    void    subsChars(string & origString);
    vector<StringLine>  script;
    
    ofTrueTypeFont      font;
    float   speed;
    float   waitingTime;
    float   secondsForChar;
    
    float   countDown;
    float   fontSize;
    //float   fixedWidth;
    float   interLineSpace;
    int     currentLine;
    int     alignment;
    
    bool    bWaiting;
};

#endif
