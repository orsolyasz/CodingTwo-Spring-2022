//
//  boidChild.cpp
//  boids-attempt-1
//
//  Created by Orsolya Szantho on 2/2/22.
//

#include "boidChild.h"

boidChild::boidChild()
{
    separationWeight = 0.8f;
    cohesionWeight = 0.3f;
    alignmentWeight = 0.2f;
    
    separationThreshold = 10;
    neighbourhoodSize = 200;
    
    position = ofVec3f(ofRandom(0, 150), ofRandom(0, 100));
    velocity = ofVec3f(ofRandom(-2, 1.7), ofRandom(-1, 1.5));
}

boidChild::boidChild(ofVec3f &pos, ofVec3f &vel)
{
//    separationWeight = 0.8f;
//    cohesionWeight = 0.3f;
//    alignmentWeight = 0.2f;
//
//    separationThreshold = 10;
//    neighbourhoodSize = 200;
    
    position = pos;
    velocity = vel;
}

boidChild::~boidChild()
{
    
}


void boidChild::draw()
{
    ofSetColor(255, 0, 0);
    ofCircle(position.x, position.y, 5);
}
