//
//  Shape.h
//  mdt-Core
//
//  Created by Patricio González Vivo on 7/10/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef mdt_Core_Shape_h
#define mdt_Core_Shape_h

#include "ofMain.h"

class Shape : public ofPolyline {
public:
    
    Shape(){
        clear();
        color.set(255,0,0);
        image = NULL;
    }
    
    Shape(int _nId, vector<ofPoint>& verts){
        nId = _nId;
        clear();
        addVertexes(verts);
        color.set(255,0,0);
        image = NULL;
    }
    
    int setImage( ofImage &_image){ image = &_image; };
    int setId( int _nId){ nId = _nId; };
    int getId() const { return nId; };
    
    void    changeHue(float _value){
        float hue = color.getHue();
        if ( hue+_value >= 255 ){
            color.setHue( (hue+_value)-255 );
        } else { 
            color.setHue( hue+_value );
        }
    }
    
    void    draw(){
        ofPushStyle();
        
        if (image != NULL){
            ofSetColor(255,200);
            ofPoint texCenter = ofPoint(image->getWidth()*0.5,image->getHeight()*0.5);
            ofPoint center = getCentroid2D();
            
            image->getTextureReference().bind();
            glBegin(GL_TRIANGLE_FAN);
            
            //  Center
            glTexCoord2f(texCenter.x, texCenter.y);
            glVertex3f(center.x, center.y, 0.0);
            
            for(int i = 0; i < getVertices().size(); i++){    
                ofPoint texCoord = getVertices()[i] - center;
                texCoord += texCenter;
                glTexCoord2f( texCoord.x , texCoord.y );
                glVertex3f(getVertices()[i].x, getVertices()[i].y, 0.0);
            } 
            
            // Close 
            ofPoint texCoord = getVertices()[0] - center;
            texCoord += texCenter;
            glTexCoord2f( texCoord.x , texCoord.y );
            glVertex3f(getVertices()[0].x, getVertices()[0].y, 0.0);
            
            glEnd();
            image->getTextureReference().unbind();
        } else {
            ofSetColor(color);
            ofBeginShape();
            for (int i = 0; i < getVertices().size(); i++){
                ofVertex( getVertices()[i] );
            }
            ofEndShape(true);
        }
        
        ofPopStyle();
    }
    
    void    drawContour(){
        
        if (ripple >= 1.0){
            ripple = ofLerp(ripple,3.0,0.005);
            
            if (ripple < 2.0){
                ofPushMatrix();
                ofTranslate(getCentroid2D().x, getCentroid2D().y);
                ofScale(ripple,ripple);
                ofTranslate(-getCentroid2D().x, -getCentroid2D().y);
                ofSetColor(0,255-(ripple-1.0)*255);
                ofSetLineWidth((ripple-1.0)*5);
                ofPolyline::draw();
                ofPopMatrix();
            } else if (ripple > 2.8){
                ripple = 0.0;
            }
        } else {
            //ofPolyline::draw();
        }
    }
    
    float   ripple;
    
private:
    ofImage *image;
    
    ofColor color;
    int     nId;
};

#endif
