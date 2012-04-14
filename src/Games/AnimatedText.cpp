//
//  AnimatedText.cpp
//  animatedText
//
//  Created by Patricio González Vivo on 4/9/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#include "AnimatedText.h"

AnimatedText::AnimatedText(){
    currentLine = 0;
}

bool AnimatedText::load(string _xmlFile){
    bool    success = false;
    
    ofxXmlSettings XML;
    
    if (XML.loadFile(_xmlFile)){
        string fontFile = XML.getValue("font", "helvetica.ttf");
        fontSize = XML.getValue("fontSize", 30);
        secondsForChar = XML.getValue("secondForChar", 0.2);
        waitingTime = XML.getValue("waitingTime", 2.0);
        interLineSpace = XML.getValue("interLineSpace", 1.5);
        
        success = font.loadFont(fontFile,fontSize, true, true);
        font.setGlobalDpi(XML.getValue("dpi", 90));
        
        int totalLines = XML.getNumTags("line");
        
        for(int i = 0; i < totalLines; i++){
            XML.pushTag("line",i);
            StringLine newLine;
            
            newLine.text = XML.getValue("text", "Text");
            newLine.seconds = newLine.text.length() * secondsForChar;
            newLine.speed = XML.getValue("speed", 1.0);
            
            subsChars(newLine.text);
            string alignment = XML.getValue("align", "LEFT");
            
            if (alignment == "LEFT"){
                newLine.align = 0;
            } else if ( alignment == "CENTER"){
                newLine.align = 1;
            } else if ( alignment == "RIGHT"){
                newLine.align = 2;
            }
            
            script.push_back(newLine);
            
            XML.popTag();
        }
        
        countDown = waitingTime;
        currentLine = -1;
        bWaiting = true;
    }
    
    return success;
}

void AnimatedText::draw(){

    if ( bWaiting ){
        if ( countDown <= 0){
            bWaiting = false;
            currentLine = (currentLine+1)%script.size();
            countDown = script[currentLine].seconds;
        }
        
        countDown -= 1/ofGetFrameRate();
        
    } else {
        
        int textWidth = font.stringWidth( script[currentLine].text );
        int textHeight = font.stringHeight( script[currentLine].text );

        if ( textWidth < width*0.9 ){
            if (script[currentLine].align == 0){
                // LEFT ALIGN
                font.drawString(script[currentLine].text , 
                                x, 
                                y);
            } else if (script[currentLine].align == 1){
                // CENTER ALIGN
                font.drawString(script[currentLine].text , 
                                getCenter().x - textWidth*0.5, 
                                getCenter().y - textHeight*0.5);
            } else if (script[currentLine].align == 2){
                // RIGHT ALIGN
                font.drawString(script[currentLine].text , 
                                x + width - textWidth, 
                                y);
            }
            
        } else {
            vector <string> words = ofSplitString(script[currentLine].text, " ");
            vector <string> lines;
            string tempText = "";
            
            for (int i = 0; i < words.size(); i++){
                if (font.stringWidth(tempText + " " + words[i]) < width*0.9 ) {
                    tempText = tempText + " " + words[i];
                } else {
                    lines.push_back(tempText);
                    tempText = " " + words[i];
                }
            }
            lines.push_back(tempText);
            
            for (int i = 0; i < lines.size(); i++){
                if (script[currentLine].align == 0){
                    font.drawString(lines[i], 
                                    x, 
                                    getCenter().y - (fontSize*interLineSpace)*lines.size()*0.5 + (fontSize*interLineSpace)*i );
                
                } else if (script[currentLine].align == 1){
                    font.drawString(lines[i], 
                                    getCenter().x - font.stringWidth(lines[i])*0.5, 
                                    getCenter().y - (fontSize*interLineSpace)*lines.size()*0.5 + (fontSize*interLineSpace)*i );
                } else if (script[currentLine].align == 2){
                    font.drawString(lines[i], 
                                    x + width*0.9 - font.stringWidth(lines[i]), 
                                    getCenter().y - (fontSize*interLineSpace)*lines.size()*0.5 + (fontSize*interLineSpace)*i );
                }
            }
        }
        
        
        if ( countDown <= 0){
            bWaiting = true;
            countDown = waitingTime;
        }
        
        countDown -= (1/ofGetFrameRate())*script[currentLine].speed;
    } 
    
    
}

float AnimatedText::getNormTransitionValue(){
    float rta = 0;
    if ((currentLine > -1) &&
        (currentLine < script.size()) ){
            
        rta = sin( (countDown/script[currentLine].seconds ) * PI );
    }
        
    return rta;
}

void AnimatedText::subsChars(string & origString){  
    static charSubstitution chars[]={ {"à","\xE0"}, {"á","\xE1"}, {"â","\xE2"}, {"ã","\xE3"}, {"ä","\xE4"}, {"æ","\xE6"}, {"ò","\xF2"},{"ó","\xF3"}, {"ô","\xF4"}, {"õ","\xF5"}, {"ö","\xF6"}, {"ù","\xF9"}, {"ú","\xFA"}, {"û","\xFB"}, {"ü","\xFC"}, {"è","\xE8"}, {"é","\xE9"}, {"ê","\xEA"}, {"ë","\xEB"}, {"ì","\xEC"}, {"í","\xED"}, {"î","\xEE"}, {"ï","\xEF"}, {"ç","\xE7"}, {"Ç","\xC7"} };  
    
    for(int i=0; i<24; i++){  
        while(origString.find(chars[i].character) !=string::npos){  
            origString = origString.substr(0,origString.find(chars[i].character)) + chars[i].code + origString.substr(origString.find(chars[i].character)+2);  
        }  
    };  
}