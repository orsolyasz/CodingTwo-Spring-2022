/*
 Based on:
 Project Title:
 Description: Music for home made airports: RandomAudio by
 ©Daniel Buzzo 2021
 https://github.com/danbz
 */



#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    ofSetCircleResolution(60);
    //load sound files
    loadMagic();
    
    tempo = 5000; // starting with five seconds tempo
    b_autoPlay = false;
    currTime = ofGetSystemTimeMillis();
    
    ofSetBackgroundColor(0);
   // ofSetBackgroundAuto(false);
    b_Gui = true;
    
//    drums.load("Dirty-electronic-samples/sample13.aif");
//    drums.setLoop(true);
//    drums.play();
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    float time = ofGetElapsedTimef();

    ofSetColor(255);
    if (b_autoPlay){ // choose a voice to play
        if (ofGetSystemTimeMillis() > currTime + tempo){
            int num = ofRandom(totalVoices);
            voices[num].play();
            currTime = ofGetSystemTimeMillis();
            cout << "playing voice " << num << endl;
        }
    }
    

    // we make our FFT audio visualisation here
    //     int barWidth = 40;
    ofColor drawColor;
    //     int length = ofGetWidth()/ 2;
    int length = 10000;
    int barWidth = 10;
    float bandsToGet = 1024;
    float width = (float)(5*128) / bandsToGet;
    float * val = ofSoundGetSpectrum(bandsToGet);
    for (int i= 0; i < bandsToGet; i ++){
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2 , ofGetHeight() /2);
        ofRotateDeg(  360.0 / bandsToGet * i );
        
        //  ofDrawRectangle(100+i*width,ofGetHeight()-100,width,-(val[i] * 200));
        //
       drawColor.setHsb(255/bandsToGet* i, 255 , 255, 50);
       ofSetColor(drawColor);
       //ofDrawRectangle(100,-barWidth/2, val[i] * length, barWidth/2);
        ofPopMatrix();
        
        drawColor.setHsb(255/bandsToGet* i, 255 , 255, 50);
        ofSetColor(drawColor);
        //ofDrawCircle(20+4*i, 5+i, val[i]*length*0.02);
        ofDrawCircle(200+100*sin(i*0.01+time),200+100*sin(i*0.01+time)+10, val[i]*length*0.02);

    }
    
 
for (unsigned int i = 0; i < 360; i++){
        ofDrawCircle(200+100*sin(i*0.01+time),50+i,10);
        
        drawColor.set(i*0.5, 255 , 255,50);
        ofSetColor(drawColor);
        
        ofPushStyle();
        ofSetColor(drawColor);
        ofNoFill();
        ofSetLineWidth(2);
        ofBeginShape();
        ofVertex(300,300);
        ofVertex(300+cos(i)*(200+sin(i*0.2+time)*120), 300+sin(i)*(200+sin(i*0.2+time)*120));
        ofVertex(300,300);
        ofEndShape();
        ofPopStyle();
        
        
    }
  
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case '1':
            voices[0].play();
            break;
        case '2':
            voices[1].play();
            break;
        case '3':
            voices[2].play();
            break;
        case '4':
            voices[3].play();
            break;
        case '5':
            voices[4].play();
            break;
        case '6':
            voices[5].play();
            break;
        case '7':
            voices[6].play();
            break;
            
        case '8':
            if (voices.size()>7){
                voices[7].play();
            }
            break;
            
        case 'a':
            b_autoPlay= !b_autoPlay;
            break;
            
        case '=':
        case'+':
            tempo +=500;
            break;
            
        case '-':
        case'_':
            if (tempo > 1000){
                tempo -=500;
            }
            break;
        
        case 'm':
            loadMagic();
            break;
            
            
        case 'f':
            ofToggleFullscreen();
            break;
        
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------

void ofApp::loadMagic(){
    // load vocal samples
    totalVoices = 8;
    voices.clear();
    for (int i = 0; i< totalVoices; i++){
        ofSoundPlayer voice;
        string path ="magic-cut/Eno-Magic-0" + ofToString( i+1 ) + ".wav";
        cout << "loading " << path << endl;
        voice.load(path);
        voice.setMultiPlay(true);
        voices.push_back(voice);
    }
}
