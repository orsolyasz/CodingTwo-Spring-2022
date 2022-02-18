//
//  boidChild.hpp
//  boids-attempt-1
//
//  Created by Orsolya Szantho on 2/2/22.
//

#ifndef boidChild_hpp
#define boidChild_hpp

#include <stdio.h>
#include "boid.h"

class boidChild : public Boid
{
private:
    ofVec3f position;
    ofVec3f velocity;
    
    float separationWeight;
    float cohesionWeight;
    float alignmentWeight;
    
    float separationThreshold;
    float neighbourhoodSize;
    
public:
    
    
    boidChild();
    boidChild(ofVec3f &pos, ofVec3f &vel);
    
    ~boidChild();
    
    ofVec3f getPosition();
    ofVec3f getVelocity();
    
    void update(std::vector<boidChild *> &otherBoids, ofVec3f &min, ofVec3f &max);
    void draw();
    
};


#endif /* boidChild_hpp */
