#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {
    
    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, PORT);

    
    cout << "Kinect W: " << kinect.width << endl;
    cout << "Kinect H: " << kinect.height << endl;
    
    gui.setup("Parameters", "settings.xml");
    gui.add(thresh.setup("thresh", 12,0,255));
    gui.add(minBlobSize.setup("minBlobSize", 2500, 0, 50000));
    gui.add(maxBlobSize.setup("maxBlobSize", 800000, 25000,2500000));
    gui.add(blur.setup("blur", 3, 0, 100));
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    loc.x = 0;
    loc.y = 0;
    
    // enable depth->video image calibration
    kinect.setRegistration(true);
    
    kinect.init();
    //kinect.init(true); // shows infrared instead of RGB video image
    //kinect.init(false, false); // disable video image (faster fps)
    
    kinect.open();		// opens first available kinect
    //kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
    //kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
    
    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }
    
#ifdef USE_TWO_KINECTS
    kinect2.init();
    kinect2.open();
#endif
    
    grayImage.allocate(kinect.width, kinect.height);
    grayBg.allocate(kinect.width, kinect.height);
    grayDiff.allocate(kinect.width, kinect.height);
    
    
    bLearnBackground = true;
    
    
    ofSetFrameRate(60);
    
    // zero the tilt on startup
    angle = 0;
    kinect.setCameraTiltAngle(angle);
    

}

//--------------------------------------------------------------
void ofApp::update() {
    
    thresholdValue = thresh;
    
    ofBackground(100, 100, 100);
    
    kinect.update();
    
    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {
        
        // load grayscale depth image from the kinect source
        grayImage.setFromPixels(kinect.getDepthPixels());
        
        // learn the background - triggered by keypress 'b'
        // copy the data from grayImage into grayBg
        if (bLearnBackground == true){
            grayBg = grayImage;
            bLearnBackground = false;
        }
        
        // Take the absolute value of the difference
        // between the background and incoming images.
        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.blur(blur);
        // Perform an in-place thresholding of the difference image.
        grayDiff.threshold(thresholdValue);
        
        // Find contours whose areas are betweeen 200 and 1/3 w*h pixels.
        // "Find holes" is false, so we won't get interior contours.
        contourFinder.findContours(grayDiff, minBlobSize, maxBlobSize, 1, false);
        ofxOscMessage m;
        m.setAddress("/numBlobs");
        m.addIntArg(contourFinder.nBlobs);
        sender.sendMessage(m, false);

        //if contour found, find center
        if(contourFinder.nBlobs > 0){
            //get depth values
            ofPixels & pix = grayImage.getPixels();
            //print blob locations
            for (int i=0; i<contourFinder.nBlobs; i++){
                loc.x = int((contourFinder.blobs.at(i).centroid.x));
                loc.y = int((contourFinder.blobs.at(i).centroid.y));
                loc.z = pix[loc.y*kinect.width + loc.x];
                ofxOscMessage m;
                m.setAddress("/blobs/position");
                m.addIntArg(i);
                m.addIntArg(loc.x);
                m.addIntArg(loc.y);
                m.addIntArg(loc.z);
                sender.sendMessage(m, false);
            }
            
        }
//        cout << "X value: " << loc.x << endl << "Y value: " << loc.y << endl;
//
//        cout << "Z value: " << loc.z << endl;
        
    
    }
    
#ifdef USE_TWO_KINECTS
    kinect2.update();
#endif
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    ofSetColor(255, 255, 255);
    ofCircle(loc.x, loc.y, 25);
    
    grayDiff.draw(0,0);  // The thresholded difference image
    
    int numBlobs = contourFinder.nBlobs;
    for (int i=0; i<numBlobs; i++){
        contourFinder.blobs[i].draw(0,0);
    }
    
    
    // draw instructions
    ofSetColor(255, 255, 255);
    stringstream reportStream;
    
    if(kinect.hasAccelControl()) {
        reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
        << ofToString(kinect.getMksAccel().y, 2) << " / "
        << ofToString(kinect.getMksAccel().z, 2) << endl;
    } else {
        reportStream << "Note: this is a newer Xbox Kinect or Kinect For Windows device," << endl
        << "motor / led / accel controls are not currently supported" << endl << endl;
    }
    
    reportStream << "num blobs found " << contourFinder.nBlobs << " ";
    
    if(contourFinder.nBlobs > 0){
        //        reportStream << "Blob 1 centroid: " << contourFinder.blobs.at(0).centroid;
        reportStream << " X: " << loc.x << " Y: " << loc.y << " Z: " << loc.z << endl;
        
    }
    

    
    if(kinect.hasCamTiltControl()) {
        reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
        << "press 1-5 & 0 to change the led mode" << endl;
    }
    
    ofDrawBitmapString(reportStream.str(), 20, 652);
    
    gui.draw();
    
}



//--------------------------------------------------------------
void ofApp::exit() {
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();
    
#ifdef USE_TWO_KINECTS
    kinect2.close();
#endif
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
    switch (key) {
        case 'b':
            bLearnBackground = true;
            break;

        case 'p':
            if(contourFinder.nBlobs > 0){
                cout << "Blob 1 centroid: " << contourFinder.blobs.at(0).centroid << endl;
            } else {
                cout << "No blob detected on print" << endl;
            }

            
        case 'w':
            kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
            break;
            
        case 'o':
            kinect.setCameraTiltAngle(angle); // go back to prev tilt
            kinect.open();
            break;
            
        case 'c':
            kinect.setCameraTiltAngle(0); // zero the tilt
            kinect.close();
            break;
            
            
        case OF_KEY_UP:
            angle++;
            if(angle>30) angle=30;
            kinect.setCameraTiltAngle(angle);
            break;
            
        case OF_KEY_DOWN:
            angle--;
            if(angle<-30) angle=-30;
            kinect.setCameraTiltAngle(angle);
            break;
    }
}

