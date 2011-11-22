#include "ofxSimpleGuiToo.h"
#include "reactiveSineLine.h"
#include "testApp.h"



// Sound


int buffer;

int 	bufferCounter;
int 	drawCounter;

float smoothedVol;
float scaledVol;

ofSoundStream soundStream;


reactiveSineLine sineLine;


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	
	sineLine.setup();
	
	ofxSimpleGuiPage &page= gui.addPage("Reactive Sine Line");
	sineLine.setupGUI(&page);
	gui.loadFromXML();
	

	soundStream.listDevices();
	//soundStream.setDeviceID(3); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
	int bufferSize = 256;
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);
	
	bufferCounter	= 0;
	drawCounter		= 0;
	smoothedVol     = 0.0;
	scaledVol		= 0.0;
	
	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);

	
}



//--------------------------------------------------------------
void testApp::update(){

	sineLine.update(smoothedVol);
}




//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(255, 255, 255);
	ofSetColor(100, 200, 100,100);
	
	//sineLine.draw();
	
	vector<ofPoint>& points= sineLine.getLine();
	
	for (int i=0; i<points.size(); i++) {
		ofRect(points[i].x, points[i].y,2,2);
	}
	
	gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if(key>='0' && key<='9') {
		gui.setPage(key - '0');
		gui.show();
	} else {
		switch(key) {
			case ' ': gui.toggleDraw(); break;
			case '[': gui.prevPage(); break;
			case ']': gui.nextPage(); break;
			case 'p': gui.nextPageWithBlank(); break;
		}
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

void testApp::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){	
	
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;	
	
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;
		
		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}
	
	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :) 
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.6;
	smoothedVol += 0.4 * curVol;
	
	bufferCounter++;
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}
