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
    ofVec3f inPos = ofVec3f(ofRandom(800, 900), ofRandom(600,700));
    ofVec3f inVel = ofVec3f(ofRandom(-2, 2), ofRandom(-2, 2));
    
    setPosition(inPos);
    setVelocity(inVel);
}

//here we redefine what the draw function should do for the child Babies class
void Babies::draw()
{
    ofSetColor(240,150,0);
    ofTriangle(getPosition().x, getPosition().y, getPosition().x+10*cos(60), getPosition().y+10*sin(60), getPosition().x+10*sin(60), getPosition().y+10*cos(60));
}
