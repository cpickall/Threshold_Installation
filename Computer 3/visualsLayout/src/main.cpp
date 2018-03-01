#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
//	ofSetupOpenGL(4000,1200,OF_FULLSCREEN);			// <-------- setup the GL context
    
//    ofSetWindowPosition(0,-100);

    
    ofGLFWWindowSettings settings;
    settings.windowMode = OF_FULLSCREEN;
    settings.multiMonitorFullScreen = true;
    ofCreateWindow(settings);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
