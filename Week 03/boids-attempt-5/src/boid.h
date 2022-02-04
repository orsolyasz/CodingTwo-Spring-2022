/*

 *
 */

#ifndef _BOID
#define _BOID
#include <vector>
#include "ofMain.h"

class Boid
{
// all the methods and variables after the
// private keyword can only be used inside
// the class
private:	
	ofVec3f position;
	ofVec3f velocity;
    
    float avoidanceWeight;
    float opposingWeight;
    
	float separationWeight;
	float cohesionWeight;
	float alignmentWeight;
	
	float separationThreshold;
    float opposingThreshold;
    float avoidanceThreshold;
	float neighbourhoodSize;
	
	ofVec3f avoidance(std::vector<Boid *> &otherBoids);
    ofVec3f separation(std::vector<Boid *> &otherBoids);
	ofVec3f cohesion(std::vector<Boid *> &otherBoids);
	ofVec3f alignment(std::vector<Boid *> &otherBoids);
    ofVec3f opposition(std::vector<Boid*> &otherBoids);
	
// all the methods and variables after the
// public keyword can only be used by anyone
public:	
	Boid();
	Boid(ofVec3f &pos, ofVec3f &vel);
	
	~Boid();
	
	ofVec3f getPosition();
	ofVec3f getVelocity();
    
	
    float getAvoidanceWeight();
	float getSeparationWeight();
	float getCohesionWeight();
	float getAlignmentWeight();
	
    float getAvoidanceThreshold();
	float getSeparationThreshold();
	float getNeighbourhoodSize();
	
    void setAvoidanceWeight(float f);
	void setSeparationWeight(float f);
	void setCohesionWeight(float f);
	void setAlignmentWeight(float f);
	
    void setAvoidanceThreshold(float f);
	void setSeparationThreshold(float f);
	void setNeighbourhoodSize(float f);
    
    void setPosition(ofVec3f &v);
    void setVelocity(ofVec3f &v);
	
	void update(std::vector<Boid *> &otherBoids, std::vector<Boid *> &inBabies, ofVec3f &min, ofVec3f &max);
	
	void walls(ofVec3f &min, ofVec3f &max);
	
	virtual void draw();
};

#endif
