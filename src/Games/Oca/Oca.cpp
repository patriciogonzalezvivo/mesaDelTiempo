//
//  Oca.cpp
//  MesaDelTiempo for http://web.museodeljuguetesi.org.ar/
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#include "Oca.h"

Oca::Oca(){
    width = 800*1.771;
    height = 600*1.771;
    trackMode = TRACK_JUST_OBJECT;
}

Oca::~Oca(){
    int total = places.size();
    for(int i = 0; i < total; i++){
        delete places[i];
    }
    places.clear();

    for (int i = 0; i < players.size(); i++){
        ofRemoveListener( players[i].arriveToPlace, this, &Oca::playerArriveToPlace);
    }

    players.clear();
}

void Oca::init(ofRectangle _space){
    ofPoint center = ofPoint(_space.getCenter().x * width,_space.getCenter().y * height);
    scaleFactor = _space.height;// * 0.85;

    space.setFromCenter(center,
                        scaleFactor * height,
                        scaleFactor * height);

    fbo.allocate(width,height);
    fbo.begin();
    ofClear(0,255);
    fbo.end();

    mask.loadImage("Oca/mask.png");
    background.loadImage("Oca/background.jpg");

    reset();
}

void Oca::reset(){

    //  Load each place of the deck
    //
    loadXml("Oca/config.xml");

    text.setFromCenter(places[27]->getBoundingBox().x + places[27]->getBoundingBox().width*0.5,
                       places[27]->getBoundingBox().y + places[27]->getBoundingBox().height*0.4,
                       places[27]->getBoundingBox().width*0.7,
                       places[27]->getBoundingBox().height*0.8);
    text.loadSequence("Oca/config.xml");

    //  Setup spacial backgrounds
    //
    forestBackground.set( places[10]->getBoundingBox() );
    forestBackground.setZoom(30);
    forestBackground.clear();
    dragonBackground.set( places[25]->getBoundingBox() );
    dragonBackground.clear();

    //  El amigo del casillero 17 no esta presente.
    //
    obj17.loadImage("Oca/17alt/17-00.png");
    bFriend = false;

    places[27]->turnTo(0);
}

bool Oca::loadXml(string _xmlConfigFile){
    bool loaded = false;

    ofxXmlSettings XML;

    if (XML.loadFile(_xmlConfigFile)){
        places.clear();
        players.clear();

        //  Places ( casilleros )
        //
        int totalPatchs = XML.getNumTags("place");

        for(int i = 0; i < totalPatchs ; i++){
            XML.pushTag("place", i);

            Place *newPlace = new Place( XML.getValue("id", -1) );

            newPlace->angle = XML.getValue("angle", 0.0);
            newPlace->message = XML.getValue("message", "NULL");

            // Load the mask path
            if ( XML.pushTag("mask") ){
                int totalMaskCorners = XML.getNumTags("point");
                if (totalMaskCorners > 0){
                    newPlace->clear();
                }

                for(int i = 0; i < totalMaskCorners; i++){
                    XML.pushTag("point",i);
                    newPlace->addVertex( space.x + XML.getValue("x", 0.0) * space.width, space.y + XML.getValue("y", 0.0)*space.height);
                    XML.popTag();
                }
                XML.popTag(); // Pop "mask"
            }

            newPlace->scale = scaleFactor;
            newPlace->bLoop = XML.getValue("loop", false);
            newPlace->lockUntil = XML.getValue("lock", -1);

            if ( XML.tagExists("link")){
                if ( XML.pushTag("link") ){
                    int totalLinks = XML.getNumTags("id");

                    for(int i = 0; i < totalLinks; i++){
                        int linkId = XML.getValue("id", i);
                        newPlace->linked.push_back(linkId);

                    }
                    XML.popTag(); // Pop "link"
                }
            }

            newPlace->setImage( XML.getValue("baseImage", "00.png") );
            newPlace->bColored = XML.getValue("colored", false);

            places.push_back(newPlace);

            XML.popTag();   // Pop "place"
        }

        //  Players ( fichas )
        //
        int totalPlayer = XML.getNumTags("player");
        for(int i = 0; i < totalPlayer ; i++){
            XML.pushTag("player", i);

            Player newPlayer = Player(i);

            newPlayer.set(XML.getValue("pos:x", 0.0),
                          XML.getValue("pos:y", 0.0) );

            newPlayer.color.set(XML.getValue("color:r", 255),
                                XML.getValue("color:g", 255),
                                XML.getValue("color:b", 255),
                                100);

            newPlayer.img.loadImage( XML.getValue("image", "player01.png")  );

            players.push_back(newPlayer);

            XML.popTag();   // Pop "player"
        }
        for (int i = 0; i < players.size(); i++){
            ofAddListener( players[i].arriveToPlace, this, &Oca::playerArriveToPlace);
        }
    } else
        ofLog(OF_LOG_ERROR,"Oca: loading file " + _xmlConfigFile );

    return loaded;
}

void Oca::update(){

    //  Actualiza la posición de las fichas
    //
    for(int i = 0; i < players.size(); i++){
        players[i].update();
    }

    //  Actualizar el estado de los casilleros
    //
    updatePlacesStatus();

    //  Animate the text
    //
    text.update();

    //  Forest and Dragon Background update
    //
        updatePlacesBackground(10, forestBackground);

    if ( places[25]->nState > 1.1 )
        updatePlacesBackground(25, dragonBackground);
}


//  Este metodo lo separa x q no hace falta actualizar los casilleros todo el tiempo de
//  estado. Tán sólo cuando soltamos la ficha ver -> objectDeleted
//
void Oca::updatePlacesStatus(){
    //  Animate passed places
    //
    int higherPlace = -1;    //  Primero averigua cual es el casillero activo con numero más alto
    for(int i = 0; i < players.size(); i++){
        if ( players[i].nPlaceID > higherPlace ){
            higherPlace = players[i].nPlaceID;
        }
    }

    if ( higherPlace >= 0){
        for(int i = 0; i < places.size() - 1; i++){
            if (i <= higherPlace){
                places[i]->turnToMax(); // Animado ( si tiene animación )
            } else {
                places[i]->turnTo(1); // Visible
            }
        }
    }
}

void Oca::updatePlacesBackground(int _placeNumber, ofxTint& _backgroundEffect){
    _backgroundEffect.setFade( 0.2 + (1.0- ofClamp(places[_placeNumber]->nState, 0.0, 1.0) ) *0.8  );

    if (places[_placeNumber]->nState < 0.01)
        _backgroundEffect.clear();

    _backgroundEffect.begin();
        ofTranslate(-places[_placeNumber]->getBoundingBox().x,
                    -places[_placeNumber]->getBoundingBox().y);

        ofPushMatrix();
            ofClear(0,255);
            ofSetColor( ofClamp(places[_placeNumber]->nState, 0.0, 1.0) *200,255);
            ofBeginShape();
                for(int i = 0; i < places[_placeNumber]->size(); i++)
                    ofVertex( places[_placeNumber]->getVertices()[i] );
            ofEndShape();
        ofPopMatrix();

    _backgroundEffect.end();

    _backgroundEffect.update();
}

void Oca::render(){
    ofPushStyle();
    fbo.begin();
    ofClear(0,255);

    //  Draw Background
    //
    ofSetColor(255,255);
    background.draw(space);

    //  Draw forest and dragon background
    //
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    ofSetColor(255,255);
    forestBackground.draw();
    dragonBackground.draw();

    ofEnableAlphaBlending();

    //  Draw Places
    //
    for(int i = 0; i < places.size(); i++){
        ofSetColor(255, 255);
        places[i]->draw();
        //ofSetColor(0);
        //ofDrawBitmapString( ofToString(i) , places[i]->getCentroid2D() );
    }

    //  Draw Friend
    //
    if (bFriend){
        ofSetColor( 255, ofClamp( places[13]->nState, 0.0, 1.0)*255);
        obj17.draw(places[17]->getBoundingBox());
    }

    //  Draw the deck mask
    //
    ofSetColor(255, 255);
    mask.draw(space);

    //  Draw text
    //
    ofSetColor(0, ofMap( text.getNormTransitionValue(), 0.0, 0.5, 0.0, 1.0, true) * 255);
    text.draw();

    //  Draw players
    //
    for(int i = 0; i < players.size(); i++){
        players[i].draw();
    }

    ofPopStyle();
    fbo.end();
}

void Oca::objectAdded(ofxBlob &_blob){
    ofPoint blobPos = ofPoint(_blob.centroid.x * width,
                              _blob.centroid.y * height);

    //  Buscar que ficha fue seleccionada.
    //  En otras palabras que a que ficha le toca mover
    //
    for(int i = 0; i < players.size(); i++){
        if (players[i].inside( blobPos )){
            players[i].nCursorID = _blob.id;
            text.speedUp();
        }
    }
}

void Oca::objectMoved(ofxBlob &_blob){
    ofPoint blobPos = ofPoint(_blob.centroid.x * width,
                              _blob.centroid.y * height);

    //  Mueve la ficha seleccionada
    //
    for(int i = 0; i < players.size(); i++){

        //  Doble chequeo por ID y por posición.
        //  Con lo último por si esta sigue estando sobre la figura
        //  (esto puede evitar q las fichas se vallan para cualquier lado)
        //
        if ( players[i].nCursorID == _blob.id ) {
            players[i].setFromCenter(blobPos, players[i].width, players[i].height);
            break;
        }
    }
}

void Oca::objectDeleted(ofxBlob &_blob){
    ofPoint blobPos = ofPoint(_blob.centroid.x * width,
                              _blob.centroid.y * height);

    for(int i = 0; i < players.size(); i++){

        //  Chequear por id cual ficha fue soltada
        //
        if (players[i].nCursorID == _blob.id){

            //  Chequear sobre donde esta la ficha soltada
            //
            int oldPlaceID = players[i].nPlaceID;
            bool overSomething = false;
            for(int j = 0; j < places.size(); j++){
                if ( places[j]->inside( players[i].getCenter() ) ){
                    players[i].nPlaceID = j;
                    overSomething = true;
                }
            }

            if ( (oldPlaceID != players[i].nPlaceID) && overSomething ){

                //  Termino una jugada.
                //  En otras palabras movio la ficha hacia otro casillero y la soltó
                //  Entonces activa el flag Leave para decir q fue movido y salió en
                //  busqueda de su posición.
                //  Cuando llegue a ella desatará un evento ( arriveToPlace )

                players[i].bLeave = true;

            } else if ( !overSomething ){

                //  NO esta arriba de ningún casillero
                //
                players[i].nPlaceID = -1;
            }

            players[i].nCursorID = -1;
            break;                  // No hace falta q busque más
        }
    }

    int higherPlace = -1;    //  Primero averigua cual es el casillero activo con numero más alto
    for(int i = 0; i < players.size(); i++){
        if ( players[i].nPlaceID > higherPlace ){
            higherPlace = players[i].nPlaceID;
        }
    }

    if ( higherPlace == -1){
        // NO ARRANCO EL JUEGO;

        for(int i = 0; i < places.size(); i++){
            places[i]->turnTo(1.0);
            if ( places[i]->bColored )
                places[i]->color.set(0,0,0,0);
        }

        places[27]->turnTo(0);

        obj17.loadImage("Oca/17alt/17-01.png");
        bFriend = false;
    }
}

void Oca::playerArriveToPlace( int &playerID){
    int arrivalPlaceID = players[playerID].nPlaceID;

    if ( arrivalPlaceID != -1){

        //-------------------- Eventos genéricos

        //  Si el casillero tiene texto lo muestra
        //
        //  Checkea si el casillero donde calló es una oca que te hace avanzar.
        //  Si es así mueve la ficha hasta allá.
        //
        int nowGoTo = places[ arrivalPlaceID ]->lockUntil;

        if ( places[ arrivalPlaceID ]->message != "NULL" ){

            //  Escepciones
            //
            if ( (arrivalPlaceID == 4) && (places[ 4 ]->color != ofColor(0,0,0,0) ) ){   //  Manzana Tomada
                text.addMessage( "Esta manzana ya esta tomada!" );
            } else if ( (arrivalPlaceID == 5) && (places[ 5 ]->color != ofColor(0,0,0,0) ) ){   //  Vela Tomada
                text.addMessage( "Esta vela ya esta tomada!" );
            } else if ( (arrivalPlaceID == 6) && (places[ 6 ]->color != ofColor(0,0,0,0) ) ){   //  Anillo Tomado
                text.addMessage( "Este anillo ya esta tomado!" );
            } else if ( (arrivalPlaceID == 10) && (places[ 4 ]->color == players[playerID].color) ){   //  Bosque y manzana
                text.addMessage( "Tenés la manzana, no perdés un turno." );
            } else if ( ( arrivalPlaceID == 16 ) && bFriend ){
                text.addMessage( "El amigo te rescata, no perdes un turno." );                  // Pozo y amigo
            } else if ( (arrivalPlaceID == 10) && (places[ 5 ]->color == players[playerID].color) ){   //  Bosque y vela
                text.addMessage( "Tenés la vela, no perdés un turno." );
            } else if ( (arrivalPlaceID == 18) && (places[ 18 ]->color != ofColor(0,0,0,0) ) ){   //  Brújula tomada
                text.addMessage( "Esta brújula ya esta tomada!" );
            } else if ( (arrivalPlaceID == 19) && (places[ 19 ]->color != ofColor(0,0,0,0) ) ){   //  Llave tomada
                text.addMessage( "Esta llave ya esta tomada!" );
            } else if ( (arrivalPlaceID == 21) && (places[ 18 ]->color == players[playerID].color) ){   //  Laberinto y Brújula
                text.addMessage( "Tenés una brújula. Encontras la salida y no perdés un turno." );
            } else if ( (arrivalPlaceID == 23) && (places[ 19 ]->color == players[playerID].color) ){   //  Jaula y Llave
                text.addMessage( "Tenés la llave y salis sin problemas. No perdés un turno." );
            } else if ( (arrivalPlaceID == 25) && (places[ 5 ]->color == players[playerID].color) ){   //  Dragon y vela
                text.addMessage( "Tenés una la vela que te protege. No tenés que volver al segundo casillero." );
                nowGoTo = -1;
            } else if ( (arrivalPlaceID == 25) && (places[ 6 ]->color == players[playerID].color) ){   //  Dragon y Anillo
                text.addMessage( "Tenés el anillo que te protege. No tenés que volver al segundo casillero." );
                nowGoTo = -1;
            } else {
                text.addMessage( places[ arrivalPlaceID ]->message );
            }

            text.angle = places[arrivalPlaceID]->angle;
        }

        if ( nowGoTo != -1 ){
            players[playerID].setFromCenter(places[ nowGoTo ]->getCentroid2D(), 70, 70);
        }

        //  Existen algunos casilleros que son objetos mágicos.
        //  Cuando un jugador cae sobre ellos estos tiñen del color de quien
        //  los obtuvo
        //
        if (( places[ arrivalPlaceID ]->bColored ) && (places[ arrivalPlaceID ]->color == ofColor(0,0,0,0) ) ) {
            places[ arrivalPlaceID ]->color = players[playerID].color;
        }

        //-------------------- Eventos especiales

        //  Checkear si cae en el 13. En este casillero el jugador encuentra
        //  un amigo. El mismo es beneficioso para todos los jugadores.
        //
        if (( arrivalPlaceID == 13 ) && !bFriend ){
            obj17.loadImage("Oca/17alt/17-01.png");
            bFriend = true;
        }

        //  Si cae en la oca 11 y esta el amigo ir hacia allí
        //
        if ( arrivalPlaceID == 11){
            if (bFriend)
                players[playerID].setFromCenter(places[ 17 ]->getCentroid2D(), 70, 70);
            else
                players[playerID].setFromCenter(places[ 20 ]->getCentroid2D(), 70, 70);
        }

        //  Si cae en la oca 17 y esta el amigo avanzas hacia el 20
        //
        if ( (arrivalPlaceID == 17) && bFriend ){
            players[playerID].setFromCenter(places[ 20 ]->getCentroid2D(), 70, 70);
        }

        //  Si cae en la oca 27 y Gana!
        //
        if ( arrivalPlaceID == 27){
            places[27]->turnToMax();
        } else {
            places[27]->turnTo(0.0);
        }

        updatePlacesStatus();
    }
}
