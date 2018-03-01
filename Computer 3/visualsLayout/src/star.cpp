//
//  star.cpp
//  visualsLayout
//
//  Created by Caitlin Pickall on 5/17/16.
//
//

#include <stdio.h>

#include "star.h"

void star::setup() {
    
    pos.x = ofRandom(ofGetWidth());
    pos.y = ofRandom(ofGetHeight());
    
    //pos = ofGetWindowSize()/2;
    
//    vel.x = ofRandom(-3, 3);
    vel.x = (0);
    vel.y = ofRandom(-2, 2);
    
    size = ofRandom(4);
}

void star::update() {
    
    pos += vel;
    
    if (pos.x >= ofGetWidth()) {
        pos.x = ofGetWidth();
        vel.x *= -1;
    }
    
    if (pos.x <= 0) {
        pos.x = 0;
        vel.x *= -1;
    }
    
    if (pos.y >= ofGetHeight()) {
        pos.y = ofGetHeight();
        vel.y *= -1;
    }
    
    if (pos.y <= 0) {
        pos.y = 0;
        vel.y *= -1;
    }
    
}

void star::draw() {
    
    ofCircle(pos, size);
    
}