//
//  myModules.hpp
//  Granular_11th_jan
//
//  Created by Default on 11/01/2018.
//

#ifndef myModules_hpp
#define myModules_hpp
#include "ofMain.h"
#include <stdio.h>
#include <math.h>
#include "ofxMaxim.h"
#include "maxiGrains.h"

class Lfo{
public:
    Lfo();
    ~Lfo();
    
    maxiOsc LowFreq;
    
private:
};

#endif /* myModules_hpp */
