#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "maxiGrains.h"
#include "ofxPortSF.h"
#include "ofxInputField.h"
#include "ofxOsc.h"
#include "ofxBaseGui.h"
#include "ofxGui.h"
#include "Particle.hpp"
#include "ofxOscParameterSync.h"

//TIME:
#include <ctime>
#include <ratio>
#include <chrono>
#define PORT 12345


class ofApp : public ofBaseApp{
public:
    void setup();
    void setupGui();
    void update();
    void draw();
    void drawGui(ofEventArgs & args);
    void updateEnvelope(ofMouseEventArgs & args);
    void exit();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
   
    //grains array:
    vector<double> grainArray;
    vector<double> interpolatedSamples;
    vector<double> envelopedSamples;
    vector<float> startPoints;
    vector<float> initialPoints;
    vector<float> speeds;
    int NumberofUsers;
   
  
    int envelopeCounter;
   
    //value changesarrays:
    vector<int> grainSizeUpdates;
    vector<int> startUpdates;
    vector<float> speedUpdates;

    //USER PACKAGE:
    struct userPackage{
        string userID;
        int grainSize;
        bool swap;
        float startPoint;
        int initialLoc;
        float speed;
        int phase;
        double envelope;
        double positionCounter;
        double envPos;
        windowRedesign windowFunction;
        vector<double> leftSoundFile;
        vector<double> rightSoundFile;
        
        double interpolatedSample;
        double windowedSample;
        
        // RAMP STUFF:
        float RampValue;
        maxiOsc phasor;
        float phasorValue;
        float phasorLocation;
        int absPhasorLoc;
    };
    
    vector<userPackage*> userPackages;
    // ---- My Functions ----
    double trapezoidalWindow(int counter, int duration, double &sample);
    double window(double &originalPosition, float duration, double envelopePosition, float speed);
    double interpolator(vector<double> &arr, double index, int grainsize);
    
    void convertToMono();
    string openDialogBox();
    vector<double> openSoundFile(const char *filePath);
    double* leftChannel(vector<double> soundFile);
    vector<double> rightChannel(vector<double> soundFile);
    void sendFileSize(ofxOscSender &sender, int fileSize);
   
    //----
    
    void audioOut(float * output, int bufferSize, int nChannels);
    void audioIn(float * input, int bufferSize, int nChannels);
    //ofSoundStream soundstream;
    //ofSoundBuffer buffer1;
    //ofSoundBuffer::InterpolationAlgorithm g;
    
    int        bufferSize;
    int        initialBufferSize; /* buffer size */
    int        sampleRate;
    
    int fileSize;
    
    // AUDIO ARRAYS:
    vector<double> soundFile;//actual stored sound file
    double *leftSoundFile; // left channel not currently in use.
    vector<double> rightSoundFile;
    vector<double> leftWindow;
    vector<double> rightWindow;
    vector<double> soundFileWindowed3;
    vector<double> soundFileWindowed4;

    // --- gui ----
    ofParameterGroup parameters;
    ofxPanel gui;
    ofParameter<float> vol1;

    // LFO :
    maxiOsc phasor;
  
    // OSC RELATED:
    ofxOscSender atak;
    ofxOscReceiver mReceiver;
    int paramChange;
    int paramChange2;
    string MmessageDeets;
    vector<string> userIDs;
    vector<int> grainSizes;
    //int initialLoc;
    float StartparamChange;
    float rampParamChange;
    int grainSizeChange;
    float speedChange;
    
    
    //PARTICLE SYS:
    vector<Particle*> box;
    
    // BOOLS:
    bool windowIsLoaded;
    bool soundIsLoaded;
    bool firstSampleLoaded;
    bool  readyForPixels;
    bool guiReady;
    bool atLeastOneUserConnected;
    bool readyToSendFileSize;
    bool swapParams;
    bool user0ready;
    bool endOfGrain;
    bool state1;
    bool playSound1;
    bool newSoundLoaded;
    
    
    
    
    // IGNORE (C++ TIME STUFF) :
    uint64_t tickMS;
    uint64_t getDurationMS();
    // duration<double, std::milli>;
    std::chrono::duration<double, std::milli> ms;
    std::chrono::high_resolution_clock clock;
    
    /*
     something like:
     while clock < 0.07 do the envelope func
     
     sin((PI * startPoint) / (0.07 - 1));
     */
    
    // --------------
};
