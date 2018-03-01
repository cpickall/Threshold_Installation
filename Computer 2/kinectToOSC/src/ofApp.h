#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxGui.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12345


class ofApp : public ofBaseApp {
public:
    
    void setup();
    void update();
    void draw();
    void exit();
    
    void drawPointCloud();
    
    void keyPressed(int key);

    
    ofxKinect kinect;
    
#ifdef USE_TWO_KINECTS
    ofxKinect kinect2;
#endif
    
    ofxCvColorImage colorImg;
    
    ofxCvGrayscaleImage grayImage; // grayscale depth image

    ofxCvGrayscaleImage 	grayBg;     // the background image to test against
    ofxCvGrayscaleImage 	grayDiff;    // absolute difference between current frame and background image
    
    ofxCvContourFinder contourFinder;
    
    
    int angle;
    
    int	thresholdValue;     //openCV threshold
    bool bLearnBackground;
    
    ofxPanel gui;
    ofxIntSlider thresh;
    ofxIntSlider minBlobSize;
    ofxIntSlider maxBlobSize;
    ofxIntSlider blur;
    
    ofVec3f loc;
    
    ofxOscSender sender;
};
