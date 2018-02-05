//
//  Window.hpp
//  Granular_11th_jan
//
//  Created by Default on 11/01/2018.
//

#ifndef Window_hpp
#define Window_hpp
#include "ofMain.h"
#include <stdio.h>
#include <math.h>


class window{
public:
    window();
    ~window();
    
    double hann(int point, int numSamples);
    
    double cosineHann(int point, int numSamples);
private:
};

class windowRedesign{
public:
   windowRedesign();
    ~windowRedesign();
    double sum;
    double cosineHann(int originalPosition, float duration, float speed);
private:
    float phase;
    int newPosition;
    int indexPos;
    int newPos;
    float newSpeed;
};

class oscil{
public:
    oscil();
    ~oscil();
    double frequency;
    double oscillator();
    double output;
private:
    double phase;
};

class envelope{
public:
    envelope();
    ~envelope();
    
    vector<double> apply(vector<double> soundFile, int size, int lengthOfPause);
    vector<double> applyTime(vector<double> soundFile, int size, int lengthOfPause);
    vector<double> singleEnv(vector<double> soundFile, int size);
private:
    vector<double> temp;
    vector<double> tempSingle;
};

class range{
public:
    range();
    ~range();
    
    vector<double> applyRange(int segmentSize, int startpoint, int endpoint, vector<double> soundFile);
    vector<double> applyPause(int lengthOfPause, vector<double> soundFile, int grainSize);
private:
    
};

#endif /* Window_hpp */
