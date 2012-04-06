//
//  Shadows.cpp
//  mdt-Core
//
//  Created by Patricio González Vivo on 4/6/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#define STRINGIFY(A) #A
#include "Shadows.h"

Shadows::Shadows(){
    width = 800*1.5;
    height = 600*1.5;
}

void Shadows::init(ofRectangle _space){
    ofPoint center = ofPoint(_space.getCenter().x * width,_space.getCenter().y * height);
    space.setFromCenter(center, 
                        _space.width*width, 
                        _space.height*height);
    
    fbo.allocate(width, height);
    fbo.begin();
    ofClear(0,0);
    fbo.end();
    
    for(int i = 0; i < 2; i++){
        preBlurFbo[i].allocate(width, height);
        preBlurFbo[i].begin();
        ofClear(0,0);
        preBlurFbo[i].end();
    }
    
    string fragmentHorizontalBlurShader = STRINGIFY(
                                                    uniform sampler2DRect backbuffer;
                                                    uniform float radius;
                                                    
                                                    const float total = (1. + 8. + 28. + 56.) * 2. + 70.;
                                                    
                                                    void main(void) {
                                                        vec2 st = gl_TexCoord[0].st;
                                                        
                                                        vec4 color = vec4(0.0,0.0,0.0,0.0);
                                                        color += (1. / total) * texture2DRect(backbuffer, st - radius * vec2(4. / 4., 0.));
                                                        color += (8. / total)  * texture2DRect(backbuffer, st - radius * vec2(3. / 4., 0.));
                                                        color += (28. / total)  * texture2DRect(backbuffer, st - radius * vec2(2. / 4., 0.));
                                                        color += (56. / total)  * texture2DRect(backbuffer, st - radius * vec2(1. / 4., 0.));
                                                        
                                                        color +=  (70. / total) * texture2DRect(backbuffer, st);
                                                        
                                                        color += (1. / total) * texture2DRect(backbuffer, st + radius * vec2(4. / 4., 0.));
                                                        color += (8. / total)  * texture2DRect(backbuffer, st + radius * vec2(3. / 4., 0.));
                                                        color += (28. / total)  * texture2DRect(backbuffer, st + radius * vec2(2. / 4., 0.));
                                                        color += (56. / total)  * texture2DRect(backbuffer, st + radius * vec2(1. / 4., 0.));
                                                        
                                                        gl_FragColor = color;
                                                    } 
                                                    );
    blurShader[0].unload();
    blurShader[0].setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentHorizontalBlurShader);
    blurShader[0].linkProgram();
    
    string fragmentVerticalBlurShader = STRINGIFY(
                                                  uniform sampler2DRect backbuffer;
                                                  uniform float radius;
                                                  
                                                  const float total = (1. + 8. + 28. + 56.) * 2. + 70.;
                                                  
                                                  void main(void) {
                                                      vec2 st = gl_TexCoord[0].st;
                                                      
                                                      vec4 color = vec4(0.0,0.0,0.0,0.0);
                                                      color += (1. / total) * texture2DRect(backbuffer, st - radius * vec2(0., 4. / 4.));
                                                      color += (8. / total)  * texture2DRect(backbuffer, st - radius * vec2(0., 3. / 4.));
                                                      color += (28. / total)  * texture2DRect(backbuffer, st - radius * vec2(0., 2. / 4.));
                                                      color += (56. / total)  * texture2DRect(backbuffer, st - radius * vec2(0., 1. / 4.));
                                                      
                                                      color +=  (70. / total) * texture2DRect(backbuffer, st);
                                                      
                                                      color += (1. / total) * texture2DRect(backbuffer, st + radius * vec2(0., 4. / 4.));
                                                      color += (8. / total)  * texture2DRect(backbuffer, st + radius * vec2(0., 3. / 4.));
                                                      color += (28. / total)  * texture2DRect(backbuffer, st + radius * vec2(0., 2. / 4.));
                                                      color += (56. / total)  * texture2DRect(backbuffer, st + radius * vec2(0., 1. / 4.));
                                                      
                                                      gl_FragColor = color;
                                                  } 
                                                  );
    blurShader[1].unload();
    blurShader[1].setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentVerticalBlurShader);
    blurShader[1].linkProgram();
    
}

void Shadows::reset(){
    
}

void Shadows::update(){
    preBlurFbo[0].begin();
    ofPushStyle();
    
    ofClear(0,255);
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    // this could be optimized by building a single mesh once, then copying
    // it and just adding the colors whenever the function is called.
    ofVec2f center(width / 2, height / 2);
    mesh.addVertex(center);
    mesh.addColor(ofColor(252,239,233,255));
    int n = 32; // circular gradient resolution
    float angleBisector = TWO_PI / (n * 2);
    float smallRadius = ofDist(0, 0, width / 2, height / 2);
    float bigRadius = smallRadius / cos(angleBisector);
    for(int i = 0; i <= n; i++) {
        float theta = i * TWO_PI / n;
        mesh.addVertex(center + ofVec2f(sin(theta), cos(theta)) * bigRadius);
        mesh.addColor(ofColor(158,130,90,100));
    }
	glDepthMask(false);
	mesh.draw();
	glDepthMask(true);
    
    ofFill();
    for(map<int,AnimatedShadow>::iterator it = hands.begin(); it != hands.end(); it++ ){
        it->second.draw();
    }
    
    ofPopStyle();
    preBlurFbo[0].end();
        
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 2; j++) {    
            preBlurFbo[(j+1)%2].begin();
            blurShader[j].begin();
            blurShader[j].setUniform1f("radius", 5);
            preBlurFbo[j].draw(0,0);
            blurShader[j].end();
            preBlurFbo[(j+1)%2].end();
        }
    }
    
    fbo.begin();
    preBlurFbo[1].draw(0,0);
    fbo.end();
}

void Shadows::handAdded(ofxBlob &_blob){
    AnimatedShadow newShadow;
    
    newShadow.addFrame(_blob, width, height);
    hands[newShadow.getId()] = newShadow;
}

void Shadows::handMoved(ofxBlob &_blob){
    hands[ _blob.id ].addFrame(_blob,width,height);
}

void Shadows::handDeleted(ofxBlob &_blob){
    //cout << "Hand out nº " << _blob.id << endl;
    
    if ( !hands[ _blob.id ].isHand() ){
        hands.erase( _blob.id );
    } else {
        hands[ _blob.id ].bActive = true;
    }
}