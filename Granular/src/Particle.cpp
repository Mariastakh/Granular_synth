//
//  Particle.cpp
//  Wavetable
//
//  Created by Default on 21/01/2018.
//

#include "Particle.hpp"


// ----- CONSTRUCTOR ------
Particle::Particle(){
    
}

// ----- OVERLOAD CONSTRUCTOR -----
Particle::Particle(ofVec2f l){
    initiLoc.set(l);
    location.set(l); //set COPIES the vector x and y components.
    velocity.set(0, 0);
    acceleration.set(0, 0);
    mass = 10.0;
    //lifespan = 255;
}

void Particle::update(){
    
}

void Particle::display(){
    ofDrawRectangle(location.x, location.y, 10, 10);
}

// DESTRUCTOR
Particle::~Particle(){
    
}
