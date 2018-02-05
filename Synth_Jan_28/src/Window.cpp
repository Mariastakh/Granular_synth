//
//  Window.cpp
//  Granular_11th_jan
//
//  Created by Default on 11/01/2018.
//

#include "Window.hpp"

window::window()
{
    
}

double window::hann(int point, int numSamples) //point = position, and numsamples = length
{
    return 0.5 * ( 1.0 - cos ( (2.0 * M_PI * point) / ( (float)numSamples - 1.0) ) );
}

double window::cosineHann(int point, int numSamples)
{
    return sin((PI * point) / (numSamples - 1));
}

window::~window()
{
    
}

windowRedesign::windowRedesign()
{
    phase = 0;
    newPosition = 0;
    indexPos = 0;
    newPos = 0;
}

double windowRedesign::cosineHann(int originalPosition, float duration, float speed)
{
    newPosition = phase + originalPosition; // shifts the position within the envelope/sin function
    
//    if(speed == 1.0)
//    {
//        newSpeed = 1;
//    }
    
    if(speed <= 0.9 && speed >= 0.6)
    {
        newSpeed = ofMap(speed, 0.6, 0.9, 1.8, 1.2);
    }
    
    if(speed >= 0.1 && speed <= 0.5)
    {
        newSpeed = ofMap(speed, 0.1, 0.5, 10, 2);
    }
    
    if(speed >= 1.0 && speed <=2.0 )
    {
        newSpeed = ofMap(speed, 1.0, 2.0, 1, 0.5);
    }
    
  //ofMap(speed, 0.1, 2.0, <#float outputMin#>, <#float outputMax#>)
//    cout << "NEWSPEED" << newSpeed << endl;
    sum = sin( ( (PI * newPosition) / ((duration*newSpeed) - 1
        )) ); // * speed; //why the minus 1? 
    
    phase += 1; // shift newPosition by 1
    
    // RESTART THE ENVELOPE:
    if(phase >= duration)
    {
        phase = 0;
    }
    return sum;
}

windowRedesign::~windowRedesign()
{
    
}

//---
oscil::oscil()
{
    phase = 0.0;
    frequency = 400;
}

double oscil::oscillator()
{
        //This is a sinewave oscillator
        output=sin (phase*(TWO_PI));
        if ( phase >= 1.0 ) phase -= 1.0;
        phase += (1./(44100/(frequency)));
        return(output);
        
}

oscil::~oscil()
{
    
}

//---

envelope::envelope()
{
    
}

vector<double> envelope::apply(vector<double> soundFile, int size, int lengthOfPause) //point = position, and numsamples = length
{
    temp.clear();
    for (int i = 0; i < soundFile.size(); i++)
    {
        double multiplier;
        
        //        int sum = lengthOfPause + size;
        //        int index = i%sum;
        //        if(index > size && index < lengthOfPause)
        //        {
        //            multiplier = 0;
        //        } else {
        //            multiplier = 0.5 * (1 - cos(2*PI*i/size));
        //        }
        
        
        
        int sum = size*lengthOfPause;
        int index = i%sum;
        if(index > size)
        {
            multiplier = 0;
        } else {
            multiplier = 0.5 * (1 - cos(2 * PI * i / size));
        }
        
        temp.push_back(multiplier * soundFile[i]);
    }
    
    return temp;
}

vector<double> envelope::singleEnv(vector<double> soundFile, int size)
{
    tempSingle.clear();
    for (int i = 0; i < size; i++)
    {
        double multiplier;
        
        multiplier = 0.5 * (1 - cos(2 * PI * i / size));
        
        
        tempSingle.push_back(multiplier * soundFile[i]);
    }
    cout << "window size" <<  tempSingle.size() << endl;
    return  tempSingle;
}

vector<double> envelope::applyTime(vector<double> soundFile, int size, int lengthOfPause) //point = position, and numsamples = length
{
    int numFrames =  soundFile.size() / 2;
    uint64_t durationInMicroSecs;
    durationInMicroSecs = numFrames * uint64_t(1000) / uint64_t(48000);
    
    
}

envelope::~envelope()
{
    
}

range::range()
{
    
}

vector<double> range::applyRange(int segmentSize, int startpoint, int endpoint, vector<double> soundFile)
{
    vector<double> temp;
    for (int i = 0; i < soundFile.size(); i++)
    {
        int multiplier;
        if(i%segmentSize > startpoint && i%segmentSize < endpoint) //controls pause between grains
        {
            multiplier = 1;
        } else {
            multiplier = 0;
        }
        temp.push_back(multiplier * soundFile[i]);
    }
    return temp;
}

vector<double> range::applyPause(int lengthOfPause, vector<double> soundFile, int grainSize)
{                                   //frequency, soundFile, EnvSize
    //the idea is to only have one slider control this thing.
    //so what we say is the startpoint and the endpoint are the same - one grain.
    //the segment size will simply change the distance between these start/
    //should I measure things on a time bases, or sample per frame basis?
    vector<double> temp;
    for (int i = 0; i < soundFile.size(); i++)
    {
        int multiplier;
        //        if(i%lengthOfPause > 0 && i%lengthOfPause < 1024) //i.e. 1 //1024 should be grain size!
        //        {
        //            multiplier = 1;
        //        } else {
        //            multiplier = 0;
        //        }
        
        //this function is good. the problem is that when the pause is not a multiple of the grainsize, the pause runs its course and then sound starts plays midway through the next grainwindow, instead of at the start.
        //this is because the array we've defined remains the same, we're just applying 0s
        //where the pauses are, but then continuing playing through the unpaused values.
        //which don't care that the grain should begin play from the start at the beginning of each pause.
        //so actually we need to make a whole new array from the start, and take into
        //consideration the pause immediately instead of working
        //with the one we've got.
        int sum = lengthOfPause + grainSize;
        int index = i%sum;
        if(index > grainSize && index < lengthOfPause)
        {
            multiplier = 0;
        } else {
            multiplier = 1;
        }
        
        temp.push_back(multiplier * soundFile[i]);
    }
    return temp;
}

range::~range()
{
    
}



/*
 struct hannWinFunctor {
 inline double operator()(ulong windowLength, ulong windowPos) {
 return 0.5 * (1.0 - cos((2.0 * PI * windowPos) / (windowLength - 1)));
 }
 };
 
 //this window can produce clicks
 struct hammingWinFunctor {
 inline double operator()(ulong windowLength, ulong windowPos) {
 return 0.54 - (0.46 * cos((2.0 * PI * windowPos) / (windowLength - 1)));
 }
 };
 
 
 struct cosineWinFunctor {
 inline double operator()(ulong windowLength, ulong windowPos) {
 return sin((PI * windowPos) / (windowLength - 1));
 }
 };
 
 struct rectWinFunctor {
 inline double operator()(ulong windowLength, ulong windowPos) {
 return 1;
 }
 };
 
 struct triangleWinFunctor {
 inline double operator()(ulong windowLength, ulong windowPos) {
 return (2.0 / (windowLength-1.0)) * (((windowLength-1.0)/2.0) - fabs(windowPos - ((windowLength-1.0)/2.0)));
 }
 };
 
 struct triangleNZWinFunctor {
 //non zero end points
 inline double operator()(ulong windowLength, ulong windowPos) {
 return (2.0 / windowLength) * ((windowLength/2.0) - fabs(windowPos - ((windowLength-1.0)/2.0)));
 }
 };
 
 struct blackmanHarrisWinFunctor {
 inline double operator()(ulong windowLength, ulong windowPos) {
 return 0.35875 -
 (0.48829 * cos((2 * PI * windowPos) / (windowLength-1))) +
 (0.14128 * cos((4 * PI * windowPos) / (windowLength-1))) +
 (0.01168 * cos((6 * PI * windowPos) / (windowLength-1)));
 }
 };
 
 struct blackmanNutallWinFunctor {
 inline double operator()(ulong windowLength, ulong windowPos) {
 return 0.3635819 -
 (0.4891775 * cos((2 * PI * windowPos) / (windowLength-1))) +
 (0.1365995 * cos((4 * PI * windowPos) / (windowLength-1))) +
 (0.0106411 * cos((6 * PI * windowPos) / (windowLength-1)));
 }
 };
 
 struct gaussianWinFunctor {
 double o;
 gaussianWinFunctor() {
 o = 0.02; //compatible with MPTK
 }
 inline double operator()(ulong windowLength, ulong windowPos) {
 double p = 1.0/(2.0*o*(windowLength+1)*(windowLength+1));
 double k = (double)windowPos-((double)(windowLength-1))/2.0;
 return exp(-k*k*p);
 }
 };
 */
