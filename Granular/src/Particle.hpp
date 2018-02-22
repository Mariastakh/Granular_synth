//
//  Particle.hpp
//  Wavetable
//
//  Created by Default on 21/01/2018.
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include <iostream>
#include "ofMain.h"

class Particle{
public:
    //default constructor
    Particle();
    ~Particle();
    //overload constructor
    Particle(ofVec2f l);
    
    void update();
    void display();
   
    ofVec2f location;
    ofVec2f velocity;
    ofVec2f acceleration;
    ofVec2f initiLoc;
    float mass;
    
    //LIFESPAN:
    float lifespan;
    bool isDead(){
        if(lifespan < 0.0){
            return true;
        } else {
            return false;
        }
    }
    
private:
    
    //members that the child classes wont need
};

#endif /* Particle_hpp */
