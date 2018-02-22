#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    //ofSetupOpenGL(1024,768,OF_WINDOW);            // <-------- setup the GL context
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    //ofRunApp(new ofApp());
    
    ofGLFWWindowSettings settings;
    settings.width = 600; // synth window
    settings.height = 400;
    settings.setPosition(ofVec2f(300, 0));
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
    settings.width = 300;
    settings.height = 600;
    settings.setPosition(ofVec2f(0, 0));
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
    guiWindow->setVerticalSync(false);
    
    shared_ptr<ofApp> mainApp(new ofApp);
    mainApp->setupGui();
    ofAddListener(guiWindow->events().draw,mainApp.get(),&ofApp::drawGui);
    //ofEvent<ofMouseEventArgs>     mouseReleased;
    // /*
    ofAddListener(guiWindow->events().mouseReleased, mainApp.get(), &ofApp::updateEnvelope);
    //changeSlider should be an event trigger that happens as soon the app
    //sees a mousedrag, keypress, or the ofApp update loops starts running.
    //updateEnvelope should be a custom ofApp function, in which I then call my custom
    //gui function
    //  */
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
    
}

