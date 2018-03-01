#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    // listen on the given port
    cout << "listening for osc messages on port " << PORT << "\n";
    receiver.setup(PORT);
    
    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, PORT2);
    numBlobs = 0;
    
    layout.loadMovie("layoutnew.mp4");
    
    ofBackground(0);
    width = ofGetWidth();
    height = ofGetHeight();
    mode = 0;
    
    test = 0;
    
    //gradient
    black = 0;
    white = 255;
    dir = 1;
    grCount = 0;
    
    lineW = 0;
    lineH = 5;

    
    //star field
    for(int i = 0; i < 3500; i++){
        star myStar;
        
        myStar.setup();
        starList.push_back(myStar);
        
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        // check for numBlobs message
        if(m.getAddress() == "/numBlobs"){
            numBlobs = m.getArgAsInt32(0);
            cout << "num blobs: " << numBlobs << endl;;
        }
        
        if(m.getAddress() == "/blobs/position"){
            blobPos.x = m.getArgAsInt32(1);
            blobPos.y = m.getArgAsInt32(2);
            blobPos.z = m.getArgAsInt32(3);
            cout << "XYZ: " << blobPos.x << ", " << blobPos.y << ", " << blobPos.z << endl;
        }
        
        
    }
    
    //send mode info out
    ofxOscMessage s;
    s.setAddress("/mode");
    s.addIntArg(mode);
    sender.sendMessage(s, false);

    ofSoundUpdate();

    
    if(mode ==5){
    for (int i = 0; i < starList.size(); i++) {
        starList[i].update();
    }
    }


if(mode == 6){
    layout.update();
}
}
//--------------------------------------------------------------
void ofApp::draw(){

    
    ofSetColor(255);
    
    if(numBlobs > 0){
        
        if (blobPos.z > 160){
            mode = 5;
        } else if (blobPos.z < 160 && blobPos.z > 75){
            mode = 6;
        } else if (blobPos.z < 75){
            mode = 2;
        }

    
    //gradient BW and fade to black 
     if(mode == 2){
          if(grCount < 1){
                //draw gradient
                if(dir){
                    black ++;
                    white --;
                    if(black == 255){
                        dir = !dir;
                    }
                } else {
                    black--;
                    white++;
                    if(white == 255){
                        dir = !dir;
                        grCount++;
                    }
                }}
            else if (white > 0){
                white--;
            }
         ofBackgroundGradient(black, white, OF_GRADIENT_BAR);
        }
    
    //random width lines
    if(mode == 3){
        int numlines = int(ofGetWidth()/10);
        int xpos = 0;
        for (int i = 0; i < numlines; i++){
            int lineW = int(ofRandom(100));
            int spacer = int(ofRandom(50));
            ofRect(xpos, 0, lineW, ofGetHeight());
            xpos += lineW+spacer;
        }
    }
    
    
    // black white random
    if(mode == 4){
        int on = int(ofRandom(2));
        ofBackground(255*on);
    }
    
    //star field
    if(mode == 5){
        for (int i = 0; i < starList.size(); i++) {
            starList[i].draw();
        }
    }
    
    //video
    if(mode == 6){
        layout.play();
        layout.draw(1920,0);
        
    }
    

    
    //testing projectors
//    ofSetColor(255,0,0);
//    ofRect(0,0, 1920, 1080);
//    ofSetColor(0,255,0);
//    ofRect(1920,0,1920,1080);
    


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case '1':
            mode = 1;
            break;
            
        case '2':
            mode = 2;
            break;
            
        case '3':
            mode = 3;
            break;
            
        case '4':
            mode = 4;
            break;
            
        case '5':
            mode = 5;
            break;
            
        case '6':
            mode = 6;
            break;

        case '7':
            mode = 7;
            break;
            
        case '9':
            numBlobs = 1;;
            break;
    }

}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    y = mouseY;
    x = mouseX;
    
}


