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
	avoidanceWeight = 0.1f;
    opposingWeight = 2.2f;
    
    separationWeight = 1.0f;
	cohesionWeight = 0.2f;
	alignmentWeight = 0.1f;
	
    avoidanceThreshold =20;
    opposingThreshold = 170;
    
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
    
    ofVec3f v = average.getRotated(90, ofVec3f(0, 0, 1));
    
    v.normalize();
    return v;
}


//ofVec3f Boid::opposition(std::vector<Boid *> &otherGroupBoids)
//{
//    ofVec3f average(0,0,0);
//    int count = 0;
//    for (int i = 0; i < otherGroupBoids.size(); i++)
//    {
//        if (position.distance(otherGroupBoids[i]->getPosition()) < opposingThreshold)
//        {
//            average += otherGroupBoids[i]->getVelocity();
//            count += 1;
//            std::cout<< average << endl;
//        }
//    }
//    average /= count;
//
//    ofVec3f difference = velocity-average;
//    float lengthOfDifference = (float)sqrt(difference.x*difference.x + difference.y*difference.y + difference.z*difference.z);
//
//    ofVec3f v(0,0,0);
//
//    if ( lengthOfDifference < 5){
//        ofVec3f v = -velocity;
//    }
//
//    v.normalize();
//    return v;
//}

void Boid::update(std::vector<Boid *> &otherBoids, std::vector<Boid *> &otherGroupBoids, ofVec3f &min, ofVec3f &max)
{
    //velocity += avoidanceWeight*avoidance(otherGroupBoids);
    velocity += opposingWeight*opposition(otherGroupBoids);
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
	ofSetColor(255, 100, 100);
  
    ofVec3f plusSpeed=velocity;
    
    //this bit I got from SimonS98's code to flip the triangle directions a bit
    //https://github.com/SimonS98/Open_Framework_lab/blob/main/Boids_Lab/Boids_2_2/src/boid.cpp
    if(velocity.x>0||velocity.y>0)
        {
            plusSpeed.x = velocity.x;
        }
    else if(velocity.x<0||velocity.y<0)
        {
            plusSpeed.x = velocity.x*2.5;
            plusSpeed.y = velocity.y*2.5;
        }
    ofTriangle(position.x+plusSpeed.x*2.2,position.y+plusSpeed.y*2.2,position.x+cos(60.0)*15,position.y+sin(60.0)*15,position.x+sin(60.0)*15,position.y+cos(60.0)*15);
        

}
