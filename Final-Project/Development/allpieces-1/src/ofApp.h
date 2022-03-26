#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"


#define _USE_LIVE_VIDEO        // uncomment this to use a live camera
                                // otherwise, we'll use a movie file


class ofApp : public ofBaseApp{
	public:
		
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    void drawFlowers();
    void drawMeshGrid();
    
    int noiseWeight;
    int imageWeight;

    ofShader shader;
    ofPlanePrimitive plane;
    ofImage img;
    
#ifdef _USE_LIVE_VIDEO
  ofVideoGrabber         vidGrabber;
#else
  ofVideoPlayer         vidPlayer;
#endif

ofxCvColorImage            colorImg;

ofxCvGrayscaleImage     grayImage;
ofxCvGrayscaleImage     grayBg;
ofxCvGrayscaleImage     grayDiff;

ofxCvContourFinder     contourFinder;

int                 threshold;
bool                bLearnBakground;



// for blob tracking as controls
bool trackblobs;
int holes;
float blobcenterX;
float blobcenterY;
float blobsumX;
float blobsumY;
float targetX;
float targetY;


//for audio input

void audioIn(ofSoundBuffer & input);
vector <float> left;
vector <float> right;
vector <float> volHistory;
int     bufferCounter;
int     drawCounter;
float smoothedVol;
float scaledVol;
ofSoundStream soundStream;


//for audio output
void audioOut(ofSoundBuffer & buffer);
ofSoundStream soundStream2;

float    pan;
int      sampleRate;
bool     bNoise;
float    volume;

vector <float> lAudio;
    vector <float> lAudioValues;
vector <float> rAudio;
    vector <float> rAudioValues;

//------------------- for the simple sine wave synthesis
float     targetFrequency;
float     phase;
float     phaseAdder;
float     phaseAdderTarget;

    bool flowersvis;
    bool gridvis;

};
