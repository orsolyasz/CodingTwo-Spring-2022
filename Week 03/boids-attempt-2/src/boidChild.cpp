//
//  boidChild.cpp
//  boids-attempt-2
//
//  Created by Orsolya Szantho on 2/2/22.
//

#include "boidChild.h"

ofVec3f boidChild::separation(std::vector<boidChild *> &otherBoids)
{
    // finds the first collision and avoids that
    // should probably find the nearest one
    // can you figure out how to do that?
    for (int i = 0; i < otherBoids.size(); i++)
    {
        if(position.distance(otherBoids[i]->getPosition()) < separationThreshold)
        {
            ofVec3f v = position -  otherBoids[i]->getPosition();
            v.normalize();
            return v;
        }
    }
}

ofVec3f boidChild::cohesion(std::vector<boidChild *> &otherBoids)
{
    ofVec3f average(0,0,0);
    int count = 0;
    for (int i = 0; i < otherBoids.size(); i++)
    {
        if (position.distance(otherBoids[i]->getPosition()) < neighbourhoodSize)
        {
            average += otherBoids[i]->getPosition();
            count += 1;
        }
    }
    average /= count;
    ofVec3f v =  average - position;
    v.normalize();
    return v;
}

ofVec3f boidChild::alignment(std::vector<boidChild *> &otherBoids)
{
    ofVec3f average(0,0,0);
    int count = 0;
    for (int i = 0; i < otherBoids.size(); i++)
    {
        if (position.distance(otherBoids[i]->getPosition()) < neighbourhoodSize)
        {
            average += otherBoids[i]->getVelocity();
            count += 1;
        }
    }
    average /= count;
    ofVec3f v =  average - velocity;
    v.normalize();
    return v;
}


void boidChild::update(std::vector<boidChild *> &otherBoids, ofVec3f &min, ofVec3f &max)
{
    velocity += separationWeight*separation(otherBoids);
    velocity += cohesionWeight*cohesion(otherBoids);
    velocity += alignmentWeight*alignment(otherBoids);
    
    walls(min, max);
    position += velocity;
}



void boidChild::draw()
{
    ofSetColor(255, 0, 0);
    ofDrawRectangle(position.x, position.y, 5, 10);
}
