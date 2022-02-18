/*
 *  boid.cpp
 *  boids
 *
 *  Created by Marco Gillies on 05/10/2010.
 *  Copyright 2010 Goldsmiths, University of London. All rights reserved.
 *
 */

#include "boid.h"
#include "ofMain.h"

Boid::Boid()
{
	avoidanceWeight = 0.6f;
    opposingWeight = 0.3f;
    
    separationWeight = 1.0f;
	cohesionWeight = 0.2f;
	alignmentWeight = 0.1f;
	
    avoidanceThreshold =50;
    opposingThreshold = 400;
    
	separationThreshold = 15;
	neighbourhoodSize = 100;
	
	position = ofVec3f(ofRandom(0, 200), ofRandom(0, 200));
	velocity = ofVec3f(ofRandom(-2, 2), ofRandom(-2, 2));
}

Boid::Boid(ofVec3f &pos, ofVec3f &vel)
{
    avoidanceWeight = 1.0f;
	separationWeight = 1.0f;
	cohesionWeight = 0.2f;
	alignmentWeight = 0.1f;

    avoidanceThreshold = 15;
	separationThreshold = 15;
	neighbourhoodSize = 100;

	position = pos;
	velocity = vel;
}

Boid::~Boid()
{
	
}

float Boid::getAvoidanceWeight()
{
    return avoidanceWeight;
}

float Boid::getSeparationWeight()
{
	return separationWeight;
}
float Boid::getCohesionWeight()
{
	return cohesionWeight;
}

float Boid::getAlignmentWeight()
{
	return alignmentWeight;
}

float Boid::getAvoidanceThreshold()
{
    return avoidanceThreshold;
}

float Boid::getSeparationThreshold()
{
	return separationThreshold;
}

float Boid::getNeighbourhoodSize()
{
	return neighbourhoodSize;
}

void Boid::setAvoidanceWeight(float f)
{
    avoidanceWeight = f;
}


void Boid::setSeparationWeight(float f)
{
	separationWeight = f;
}
void Boid::setCohesionWeight(float f)
{
	cohesionWeight = f;
}

void Boid::setAlignmentWeight(float f)
{
	alignmentWeight = f;
}

void Boid::setAvoidanceThreshold(float f)
{
    avoidanceThreshold = f;
}


void Boid::setSeparationThreshold(float f)
{
	separationThreshold = f;
}

void Boid::setNeighbourhoodSize(float f)
{
	neighbourhoodSize = f;
}


ofVec3f Boid::getPosition()
{
	return position;
}

ofVec3f Boid::getVelocity()
{
	return velocity;
}

void Boid::setPosition(ofVec3f &v)
{
    position = v;
}

void Boid::setVelocity(ofVec3f &v)
{
    velocity = v;
}


ofVec3f Boid::separation(std::vector<Boid *> &otherBoids)
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

ofVec3f Boid::avoidance(std::vector<Boid *> &otherGroupBoids)
{
    // finds the first collision and avoids that
    // should probably find the nearest one
    // can you figure out how to do that?
    for (int i = 0; i < otherGroupBoids.size(); i++)
    {
        if(position.distance(otherGroupBoids[i]->getPosition()) < avoidanceThreshold)
        {
            ofVec3f v = position -  otherGroupBoids[i]->getPosition();
            v.normalize();
            return v;
        }
    }
}

ofVec3f Boid::cohesion(std::vector<Boid *> &otherBoids)
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

ofVec3f Boid::alignment(std::vector<Boid *> &otherBoids)
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

ofVec3f Boid::opposition(std::vector<Boid *> &otherGroupBoids)
{
    ofVec3f average(0,0,0);
    int count = 0;
    for (int i = 0; i < otherGroupBoids.size(); i++)
    {
        if (position.distance(otherGroupBoids[i]->getPosition()) < opposingThreshold)
        {
            average += otherGroupBoids[i]->getVelocity();
            count += 1;
            std::cout<< average << endl;
        }
    }
    average /= count;
    //ofVec3f v =  average - velocity;
//    ofVec3f v = velocity - average;
    ofVec3f v = -average;
    v.normalize();
    return v;
}

void Boid::update(std::vector<Boid *> &otherBoids, std::vector<Boid *> &otherGroupBoids, ofVec3f &min, ofVec3f &max)
{
    velocity += avoidanceWeight*avoidance(otherGroupBoids);
    velocity += 2.0f*opposition(otherGroupBoids);
	velocity += separationWeight*separation(otherBoids);
	velocity += cohesionWeight*cohesion(otherBoids);
	velocity += alignmentWeight*alignment(otherBoids);
    
    float length = (float)sqrt(velocity.x*velocity.x + velocity.y*velocity.y + velocity.z*velocity.z);
    if( length > 10 ) {
        velocity*=0.5;
    }
    
//    velocity.normalize();
//    velocity*=ofRandom(0.5f, 2.0f);
	
	walls(min, max);
	position += velocity;
    
    
}

void Boid::walls(ofVec3f &min, ofVec3f &max)
{
	if (position.x < min.x){
		position.x = min.x;
		velocity.x *= -1;
	} else if (position.x > max.x){
		position.x = max.x;
		velocity.x *= -1;
	}
	
	if (position.y < min.y){
		position.y = min.y;
		velocity.y *= -1;
	} else if (position.y > max.y){
		position.y = max.y;
		velocity.y *= -1;
	}
	
	
}

void Boid::draw()
{
	ofSetColor(155, 0, 255);
	ofCircle(position.x, position.y, 5);
//    ofCircle(getPosition().x, getPosition().y, 5);

}
