#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include <vector>
#include "boid.h"
#include "babies.h"

class testApp : public ofBaseApp{
	
public:
    ~testApp();
	
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);

    //here in the main app header we create our pointer arrays
    // -- note that we use Boid* pointers for BOTH arrays - this is to make sure that
    //all the functions that take a Boid* pointer vector as an input will continue to work
    //for our Babies flock, which inherited these functions from the Boid class, as well
    std::vector<Boid *> boids;
    std::vector<Boid *> babies;
    
};

#endif	
