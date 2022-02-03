//
//  babies.cpp
//  boids-attempt-4
//
//  Created by Orsolya Szantho on 2/2/22.
//
//#include "boid.h"
#include "babies.h"

Babies::Babies()
{
//    setAvoidanceWeight(0.6f);
//    setSeparationWeight(1.0f);
//    setCohesionWeight(0.2f);
//    setAlignmentWeight(0.1f);
//    setAvoidanceThreshold(25);
//    setSeparationThreshold(15);
//    setNeighbourhoodSize(100);
    
    ofVec3f inPos = ofVec3f(ofRandom(800, 900), ofRandom(600,700));
    ofVec3f inVel = ofVec3f(ofRandom(-2, 2), ofRandom(-2, 2));
    
    setPosition(inPos);
    setVelocity(inVel);
}

void Babies::draw()
{
    ofSetColor(255,0,0);
    ofCircle(getPosition().x, getPosition().y, 5);
}
