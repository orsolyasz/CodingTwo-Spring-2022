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
    
    cout<< blobcenterX;
    cout << " ";
    cout << blobcenterY << endl;
    
	#ifdef _USE_LIVE_VIDEO
//        vector<ofVideoDevice> devices = vidGrabber.listDevices();
//
//        for(size_t i = 0; i < devices.size(); i++){
//            if(devices[i].bAvailable){
//                //log the device
//                ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
//            }else{
//                //log the device and note it as unavailable
//                ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
//            }
//        }
//
    vidGrabber.setDeviceID(0);
       vidGrabber.setDesiredFrameRate(30);
       vidGrabber.initGrabber(320, 240);
    
        vidGrabber.setVerbose(true);
//        vidGrabber.setup(320,240);
    
	#else
        vidPlayer.load("fingers.mov");
        vidPlayer.play();
        vidPlayer.setLoopState(OF_LOOP_NORMAL);
	#endif

//    colorImg.allocate(320,240);
    colorImg.allocate(vidGrabber.getWidth(), vidGrabber.getHeight());
//	grayImage.allocate(320,240);
    grayImage.allocate(vidGrabber.getWidth(), vidGrabber.getHeight());
    
//	grayBg.allocate(320,240);
    grayBg.allocate(vidGrabber.getWidth(), vidGrabber.getHeight());
    
    grayDiff.allocate(vidGrabber.getWidth(), vidGrabber.getHeight());
//	grayDiff.allocate(320,240);

	bLearnBakground = true;
	threshold = 80;
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(100,100,100);

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
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (vidGrabber.getWidth() * vidGrabber.getHeight())/3, 10, true);	// find holes
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

    blobsumX=0;
    blobsumY=0;
    
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
//	colorImg.draw(20,20, 320,240);
//	grayImage.draw(360,20, 320,240);
//	grayBg.draw(20,280, 320, 240);
//	grayDiff.draw(360,280, 320,240);

	// then draw the contours:

	ofFill();
	ofSetHexColor(0x333333);
//	ofDrawRectangle(360,540,320,240);
    ofDrawRectangle(20,20,vidGrabber.getWidth(), vidGrabber.getHeight());
	ofSetHexColor(0xffffff);
    


	// we could draw the whole contour finder
	//contourFinder.draw(360,540);
    if (trackblobs){
	// or, instead we can draw each blob individually from the blobs vector,
	// this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
//        contourFinder.blobs[i].draw(360,540);
        //contourFinder.blobs[i].draw(20,20);
        ofDrawCircle(contourFinder.blobs[i].boundingRect.getCenter().x, contourFinder.blobs[i].boundingRect.getCenter().y, 5);
        
        blobsumX=blobsumX+contourFinder.blobs[i].boundingRect.getCenter().x;
        blobsumY=blobsumY+contourFinder.blobs[i].boundingRect.getCenter().y;
        
        cout << targetX;
        cout << " ";
        cout << blobcenterX;
        cout << " :: ";
        cout << targetY;
        cout << " ";
        cout << blobcenterY << endl;
        
		// draw over the centroid if the blob is a hole
		ofSetColor(255);
//		if(contourFinder.blobs[i].hole){
//			ofDrawBitmapString("hole",
//				contourFinder.blobs[i].boundingRect.getCenter().x + 360,
//				contourFinder.blobs[i].boundingRect.getCenter().y + 540);
//		}
    }
    }
    
//    cout<< "newframe";
    
    targetX=blobsumX/contourFinder.nBlobs;
    targetY=blobsumY/contourFinder.nBlobs;
    
    blobcenterX=blobcenterX+0.2*(targetX-blobcenterX);
    blobcenterY=blobcenterY+0.2*(targetY-blobcenterY);
    
    ofSetColor(255);
    ofDrawCircle(blobcenterX, blobcenterY,20);
    ofSetColor(150);
    ofDrawCircle(targetX, targetY,20);
//    ofDrawCircle(200, 200, 40);

//    targetX=blobsumX/contourFinder.nBlobs;
//    targetY=blobsumY/contourFinder.nBlobs;
//
//    blobcenterX=blobcenterX+0.2*(blobcenterX-targetX);
//    blobcenterY=blobcenterY+0.2*(blobcenterY-targetY);
//    cout << blobcenterX;
//    cout << blobcenterY;
//    ofSetColor(255);
//    ofDrawCircle(blobcenterX, blobcenterY,20);


	// finally, a report:
//	ofSetHexColor(0xffffff);
//	stringstream reportStr;
//	reportStr << "bg subtraction and blob detection" << endl
//			  << "press ' ' to capture bg" << endl
//			  << "threshold " << threshold << " (press: +/-)" << endl
//			  << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
//	ofDrawBitmapString(reportStr.str(), 20, 600);

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
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
