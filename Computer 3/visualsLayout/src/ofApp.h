#pragma once

#include "ofMain.h"
#include "star.h"
#include "ofxOsc.h"


// listen on port 12345
#define PORT 12345
//send to host
#define HOST "localhost"
#define PORT2 54321



class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void mouseMoved(int x, int y );
    
    float width, height;
    int mode;
    
    ofxOscSender sender;
    ofxOscReceiver receiver;
    int numBlobs;
    ofVec3f blobPos;
    
    //gradient
    void drawGradient();
    int black, white;
    bool dir;
    int grCount;
    int lineW, lineH;
    
    //stars
    vector<star> starList;
    float dx;

    ofVideoPlayer layout;
    
    int test;
    
};


