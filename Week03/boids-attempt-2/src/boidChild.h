//
//  boidChild.h
//  boids-attempt-2
//
//  Created by Orsolya Szantho on 2/2/22.
//

#ifndef boidChild_h
#define boidChild_h
#include "boid.h"

#include <stdio.h>


class boidChild: public Boid
{
  public:
    ofVec3f separation(std::vector<boidChild *> &otherBoids);
    ofVec3f cohesion(std::vector<boidChild *> &otherBoids);
    ofVec3f alignment(std::vector<boidChild *> &otherBoids);
    void update(std::vector<boidChild *> &otherBoids, ofVec3f &min, ofVec3f &max);
    void draw();
    
};



#endif /* boidChild_h */
