
#include "ofMain.h"
#include "testApp.h"


//========================================================================
int main( ){
	ofSetupOpenGL(1000, 600, OF_WINDOW);			// <-------- setup the GL context
	ofRunApp(new testApp());
}
