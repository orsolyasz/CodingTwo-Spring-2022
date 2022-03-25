#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    blobsumY=100;
    blobsumX=100;
    targetX=100;
    targetY=100;
    blobcenterX=100;
    blobcenterY=100;
    trackblobs=false;
    holes=0;
    
    cout<< blobcenterX;
    cout << " ";
    cout << blobcenterY << endl;
    
    
    ofSetVerticalSync(true);
    ofSetCircleResolution(80);
    
    
    
    //for sound input
    soundStream.printDeviceList();
    
    int bufferSize = 256;

    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    
    bufferCounter    = 0;
    drawCounter        = 0;
    smoothedVol     = 0.0;
    scaledVol        = 0.0;

    ofSoundStreamSettings settings;

    // if you want to set the device id to be different than the default
    // auto devices = soundStream.getDeviceList();
    // settings.device = devices[4];
    // or by name
    
    auto devices = soundStream.getMatchingDevices("default");
    if(!devices.empty()){
        settings.setInDevice(devices[0]);
    }

    settings.setInListener(this);
    settings.sampleRate = 44100;
    settings.numOutputChannels = 0;
    settings.numInputChannels = 2;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);
        
    
    //for sound output
    int bufferSize2        = 512;
    sampleRate             = 44100;
    phase                 = 0;
    phaseAdder             = 0.0f;
    phaseAdderTarget     = 0.0f;
    volume                = 0.1f;
    bNoise                 = false;

    lAudio.assign(bufferSize2, 0.0);
    rAudio.assign(bufferSize2, 0.0);
    
    soundStream2.printDeviceList();

    ofSoundStreamSettings settings2;
    
#ifdef TARGET_LINUX
    // Latest linux versions default to the HDMI output
    // this usually fixes that. Also check the list of available
    // devices if sound doesn't work
    auto devices = soundStream2.getMatchingDevices("default");
    if(!devices.empty()){
        settings2.setOutDevice(devices[0]);
    }
#endif

    settings2.setOutListener(this);
    settings2.sampleRate = sampleRate;
    settings2.numOutputChannels = 2;
    settings2.numInputChannels = 0;
    settings2.bufferSize = bufferSize2;
    soundStream2.setup(settings2);
    
    
    
    
    
    //for camera input
	#ifdef _USE_LIVE_VIDEO

       vidGrabber.setDeviceID(0);
       vidGrabber.setDesiredFrameRate(30);
       vidGrabber.initGrabber(320, 240);
       vidGrabber.setVerbose(true);
    
	#else
        vidPlayer.load("fingers.mov");
        vidPlayer.play();
        vidPlayer.setLoopState(OF_LOOP_NORMAL);
	#endif

    colorImg.allocate(vidGrabber.getWidth(), vidGrabber.getHeight());
    grayImage.allocate(vidGrabber.getWidth(), vidGrabber.getHeight());
    grayBg.allocate(vidGrabber.getWidth(), vidGrabber.getHeight());
    grayDiff.allocate(vidGrabber.getWidth(), vidGrabber.getHeight());

	bLearnBakground = true;
	threshold = 80;
}

//--------------------------------------------------------------
void ofApp::update(){
//	ofBackground(100,100,100);
    ofBackground(0);

    
    //1. video input handling
    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.update();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.update();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels());
       
        #else
            colorImg.setFromPixels(vidPlayer.getPixels());
        #endif

        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (vidGrabber.getWidth() * vidGrabber.getHeight())/3, 10, true);	// find holes
	}
    
    
    
    //2. audio handling

    //lets scale the vol up to a 0-1 range
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);

    //lets record the volume into an array
    volHistory.push_back( scaledVol );
    
    //if we are bigger the the size we want to record - lets drop the oldest value
    if( volHistory.size() >= 400 ){
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

    blobsumX=0;
    blobsumY=0;
    holes=0;
    
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
//	colorImg.draw(vidGrabber.getWidth()+20,20, -vidGrabber.getWidth(),vidGrabber.getHeight());
//	grayImage.draw(360,20, 320,240);
//	grayBg.draw(20,280, 320, 240);
//	grayDiff.draw(360,280, 320,240);

	// then draw the contours:

	ofFill();
//	ofSetHexColor(0x000000);
//	ofDrawRectangle(360,540,320,240);
//    ofDrawRectangle(20,20,vidGrabber.getWidth(), vidGrabber.getHeight());
	ofSetHexColor(0xffffff);
    


	// we could draw the whole contour finder
	//contourFinder.draw(360,540);
    if (trackblobs){
	// or, instead we can draw each blob individually from the blobs vector,
	// this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
//        contourFinder.blobs[i].draw(360,540);
//        contourFinder.blobs[i].draw(20,20);
//        ofDrawCircle(contourFinder.blobs[i].boundingRect.getCenter().x,       contourFinder.blobs[i].boundingRect.getCenter().y, 5);
        if(!contourFinder.blobs[i].hole){
        blobsumX=blobsumX+contourFinder.blobs[i].boundingRect.getCenter().x;
        blobsumY=blobsumY+contourFinder.blobs[i].boundingRect.getCenter().y;
        } else{
            holes++;
        }
    }
    }
    
//    cout<< "newframe";
    
    targetX=vidGrabber.getWidth()-blobsumX/(contourFinder.nBlobs-holes);
    targetY=blobsumY/(contourFinder.nBlobs-holes);
    
    blobcenterX=blobcenterX+0.2*(targetX-blobcenterX);
    blobcenterY=blobcenterY+0.2*(targetY-blobcenterY);
    
    ofSetColor(255);
   // ofDrawCircle(blobcenterX, blobcenterY,scaledVol * 190.0f);
    drawCounter++;
    
    ofNoFill();
    // draw the audio input visual
    ofPushStyle();
        ofPushMatrix();
        ofTranslate(32, blobcenterY, 0);
            
        ofSetColor(225);
        //ofDrawBitmapString("Left Channel", 4, 18);
        
        ofSetLineWidth(1);
        //ofDrawRectangle(0, 0, 900, 200);

        //ofSetColor(245, 58, 135);
        ofSetLineWidth(1);
                    
            //ofBeginShape();
            for (unsigned int i = 0; i < lAudio.size(); i++){
                //float x =  ofMap(i, 0, lAudio.size(), 0, 900, true);
                //     ofVertex(x, 100 -lAudio[i]*180.0f);
                //ofVertex(500,500);
                
                ofSetColor(180+ofMap(i,0,lAudio.size(), 0,50), 180+ofMap(i,0,lAudio.size(), 0,50), 180+ofMap(i,0,lAudio.size(), 0,50));
                ofBeginShape();
                ofVertex(blobcenterX+cos(i*PI*2/512)*100*lAudio[i], sin(i*PI*2/512)*lAudio[i]);
                ofVertex(blobcenterX+cos(i*PI*2/512)*2400*scaledVol*lAudio[i], sin(i*PI*2/512)*2400*scaledVol*lAudio[i]);
                ofVertex(blobcenterX+cos(i*PI*2/512)*100*lAudio[i], sin(i*PI*2/512)*lAudio[i]);
               // ofVertex(500,500);
                
                ofEndShape(false);
                
            }
            //ofEndShape(false);
            
        ofPopMatrix();
    ofPopStyle();
    
    
//    pan = blobcenterX / ofGetWidth();
//    targetFrequency = 2000.0f * ((ofGetHeight()-blobcenterY) / ofGetHeight());
//    phaseAdderTarget = (targetFrequency / sampleRate) * TWO_PI;
    
    


//    ofNoFill();
    
    // draw the left channel:
    ofPushStyle();
        ofPushMatrix();
        ofTranslate(32, ofGetHeight()-ofGetMouseY(), 0);
            
        ofSetColor(225);
        //ofDrawBitmapString("Left Channel", 4, 18);
        
        ofSetLineWidth(1);
        //ofDrawRectangle(0, 0, 900, 200);

        //ofSetColor(245, 58, 135);
        ofSetLineWidth(1);
                    
            //ofBeginShape();
            for (unsigned int i = 0; i < lAudio.size(); i++){
                //float x =  ofMap(i, 0, lAudio.size(), 0, 900, true);
                //     ofVertex(x, 100 -lAudio[i]*180.0f);
                //ofVertex(500,500);
                
                ofSetColor(180+ofMap(i,0,lAudio.size(), 0,50), 50, 70+ofMap(i,0,lAudio.size(), 180,0));
                ofBeginShape();
                ofVertex(ofGetMouseX()+cos(i*PI*2/256+i)*100*lAudio[i], sin(i*PI*2/256)*lAudio[i]);
                ofVertex(ofGetMouseX()+cos(i*PI*2/256+i)*2400*lAudio[i], sin(i*PI*2/256)*2400*lAudio[i]);
                ofVertex(ofGetMouseX()+cos(i*PI*2/256+i)*100*lAudio[i], sin(i*PI*2/256)*lAudio[i]);
               // ofVertex(500,500);
                
                ofEndShape(false);
                
                
                
            }
            //ofEndShape(false);
            
        ofPopMatrix();
    ofPopStyle();

    // draw the right channel:
    ofPushStyle();
        ofPushMatrix();
        ofTranslate(32, ofGetMouseY(), 0);
            
        ofSetColor(225);
        //ofDrawBitmapString("Right Channel", 4, 18);
        
        ofSetLineWidth(1);
        //ofDrawRectangle(0, 0, 900, 200);

        //ofSetColor(245, 58, 135);
        ofSetLineWidth(1);
                    
            for (unsigned int i = 0; i < rAudio.size(); i++){
                //float x =  ofMap(i, 0, lAudio.size(), 0, 900, true);
                //     ofVertex(x, 100 -lAudio[i]*180.0f);
                //ofVertex(500,500);
        
                ofSetColor(70+ofMap(i,0,rAudio.size(), 180,0), 50, 180+ofMap(i,0,rAudio.size(), 0,50));
                ofBeginShape();
                ofVertex((ofGetWidth()-ofGetMouseX())+cos(i*PI*2/512)*100*rAudio[i], sin(i*PI*2/512)*rAudio[i]);
                ofVertex((ofGetWidth()-ofGetMouseX())+cos(i*PI*2/512)*3000*rAudio[i], sin(i*PI*2/512)*3000*rAudio[i]);
                    ofVertex((ofGetWidth()-ofGetMouseX())+cos(i*PI*2/512)*100*rAudio[i], sin(i*PI*2/512)*rAudio[i]);
                // ofVertex(500,500);
        
                ofEndShape(false);
            }
            
        ofPopMatrix();
    ofPopStyle();

}



//-------------------------------------------------------------

void ofApp::audioIn(ofSoundBuffer & input){
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;

    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (size_t i = 0; i < input.getNumFrames(); i++){
        left[i]        = input[i*2]*0.5;
        right[i]    = input[i*2+1]*0.5;

        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    bufferCounter++;
    
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & buffer){
    //pan = 0.5f;
    float leftScale = 1 - pan;
    float rightScale = pan;

    // sin (n) seems to have trouble when n is very large, so we
    // keep phase in the range of 0-TWO_PI like this:
    while (phase > TWO_PI){
        phase -= TWO_PI;
    }

    if ( bNoise == true){
        // ---------------------- noise --------------
        for (size_t i = 0; i < buffer.getNumFrames(); i++){
            lAudio[i] = buffer[i*buffer.getNumChannels()    ] = ofRandom(0, 1) * volume * leftScale;
            rAudio[i] = buffer[i*buffer.getNumChannels() + 1] = ofRandom(0, 1) * volume * rightScale;
        }
    } else {
        phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
        for (size_t i = 0; i < buffer.getNumFrames(); i++){
            phase += phaseAdder;
            float sample = sin(phase);
            lAudio[i] = buffer[i*buffer.getNumChannels()    ] = sample * volume * leftScale;
            rAudio[i] = buffer[i*buffer.getNumChannels() + 1] = sample * volume * rightScale;
        }
    }

}




//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key){
		case ' ':
			bLearnBakground = true;
            trackblobs = false;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
        case 'b':
            blobsumY=100;
            blobsumX=100;
            targetX=100;
            targetY=100;
            blobcenterX=100;
            blobcenterY=100;
            trackblobs=true;
            break;
        case 's':
            soundStream.start();
        case 'e':
            soundStream.stop();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    int width = ofGetWidth();
    pan = (float)x / (float)width;
    float height = (float)ofGetHeight();
    float heightPct = ((height-y) / height);
    targetFrequency = 2000.0f * heightPct;
    phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    int width = ofGetWidth();
    pan = (float)x / (float)width;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    bNoise = true;
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    bNoise = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
