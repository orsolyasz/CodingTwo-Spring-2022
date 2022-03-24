//
//  babies.h
//  boids-attempt-4
//
//  Created by Orsolya Szantho on 2/2/22.
//

#ifndef babies_h
#define babies_h

#include <stdio.h>
#include <boid.h>

class Babies : public Boid
{
    public:
    //this is a new constructor for our derived Babies class
        Babies();
    
    //here in the new class's header file we announce that we are
    //going to change what the inherited draw() function does
        void draw();
};

#endif /* babies_h */
