//
//  boidChild.cpp
//  boids-attempt-2
//
//  Created by Orsolya Szantho on 2/2/22.
//

#include "boidChild.h"

boidChild::boidChild()
{
    separationWeight = 1.0f;
    cohesionWeight = 0.2f;
    alignmentWeight = 0.1f;
    
    separationThreshold = 15;
    neighbourhoodSize = 100;
    
    position = ofVec3f(ofRandom(800, 1000), ofRandom(600, 700));
    velocity = ofVec3f(ofRandom(-2, 2), ofRandom(-2, 2));
}


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
//            std::cout << v << endl;
            return v;
        }
    }
}

//ofVec3f boidChild::avoidance(std::vector<boidChild *> &otherBoids, std::vector<Boid *> &otherKindBoids)
//{
//    // finds the first collision and avoids that
//    // should probably find the nearest one
//    // can you figure out how to do that?
//    for (int i = 0; i < otherBoids.size(); i++)
//    {
//        for (int j = 0; j < otherKindBoids.size(); j++)
//        {
//            if(position.distance(otherBoids[j]->getPosition()) < separationThreshold)
//            {
//                ofVec3f v = position -  otherKindBoids[j]->getPosition();
//                v.normalize();
//                return v;
//            }
//
//        }
//    }
//}

ofVec3f boidChild::avoidance(std::vector<Boid *> &otherKindBoids)
{
    // finds the first collision and avoids that
    // should probably find the nearest one
    // can you figure out how to do that?
    for (int i = 0; i < otherKindBoids.size(); i++)
    {
        if(position.distance(otherKindBoids[i]->getPosition()) < separationThreshold)
        {
            ofVec3f v = position -  otherKindBoids[i]->getPosition();
            v.normalize();
            std::cout << v << endl;
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
//    std::cout << v << endl;
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


void boidChild::update(std::vector<boidChild *> &otherBoids, std::vector<Boid *> &otherKindBoids, ofVec3f &min, ofVec3f &max)
{
    velocity += 0.5f*avoidance(otherKindBoids);
    velocity += separationWeight*separation(otherBoids);
    velocity += cohesionWeight*cohesion(otherBoids);
    velocity += alignmentWeight*alignment(otherBoids);
    
    walls(min, max);
    position += velocity;
//    std::cout << position << endl;
}



void boidChild::draw()
{
    ofSetColor(255, 0, 0);
    ofDrawRectangle(position.x, position.y, 5, 10);
}
