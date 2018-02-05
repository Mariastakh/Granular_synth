#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0, 0, 0);
    sampleRate = 44100;
    bufferSize = 512;
    
    paramChange = 1000;
    paramChange2 = 1000;
    
    // BOOLS :
    readyToSendFileSize = false;
    atLeastOneUserConnected = false;
    endOfGrain = true;
    user0ready = false;
    state1 = true;
    swapParams = true;
    newSoundLoaded = false;
    //----- OSC -----
    atak.setup("localhost", 11114);
    mReceiver.setup(12345);
    //---------------
    
    // ----- IMPORT SOUNDFILE: -----
    string mypathway = openDialogBox();
    soundFile = openSoundFile(mypathway.c_str());
    fileSize = soundFile.size()/2;
    cout << "soundFile size : " << soundFile.size() << endl; // 2131564
    
    leftSoundFile = (double*)std::malloc((soundFile.size())*sizeof(double));
    if (leftSoundFile==NULL) {
        printf("Error allocating memory!\n"); //print an error message
        return 1; //return with failure
    }
    
    soundIsLoaded = true;
    readyToSendFileSize = true;
    
   
    playSound1 = false; // dont try to play until sound is loaded

    
    // STEREO TO MONO:
    if(soundIsLoaded == true)
    {
        leftSoundFile = leftChannel(soundFile);     // 1065782
        rightSoundFile = rightChannel(soundFile);   // 1065782
        vector<double>().swap(soundFile);
          int size = sizeof(leftSoundFile);
        cout << "LEFT : " << size << endl;
        cout << "RIGHT : " << rightSoundFile.size() << endl;
        cout << "ORIGINAL : " << soundFile.size() << endl;
        //        playSound1 = true;
    }
    
    
    // ----- EXCEPTION HANDLING FOR SOUND FILE IMPORT ------
    if (psf_init())
    {   //initialize the portSF library
        cout << "unable to initalize port sound file ):" << endl;
        //warn if initialization fails
    }
    
    
    
    // ----- start audio thread: -----
    ofSoundStreamSetup(2, 0, this, sampleRate, bufferSize, 4);
    //                 (2 outputs, 0 inputs, pointer to ofApp, sampleRate, bufferSize, 4 buffers)

    
}

void ofApp::setupGui(){
    
    parameters.add(vol1.set("VOL 1", 0.2, 0.0, 10));
    
    gui.setup(parameters);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(newSoundLoaded == true)
    {
        newSoundLoaded = false;
        vector<double> tempRightChannel;
        tempRightChannel = rightChannel(soundFile);
        user0ready = false;
        tempRightChannel.swap(rightSoundFile);
        fileSize = soundFile.size()/2;
        cout << "filesize: " << fileSize << endl;
        readyToSendFileSize == true;
        ofxOscMessage m;
        m.setAddress("/NumSamples");
        m.addIntArg(fileSize);
        atak.sendMessage(m);
        user0ready = true;
    }
    
    // OSC STUFF : 
    while(mReceiver.hasWaitingMessages())
    {
        ofxOscMessage n;
        
        // LISTEN FOR MESSAGES:
        mReceiver.getNextMessage(n);
        
        // IF RECEIVER DETECTS NEW USER MESSAGE:
        if(n.getAddress() == "/New-User-Joined")
        {
            
            // SEND THE FILE SIZE TO APP AS OSC MESSAGE:
            sendFileSize(atak, fileSize);
  
            // ---- 
            string theId = n.getArgAsString(0);
            userIDs.push_back(theId);
            cout << "NEW USER : " << theId << endl;

            userPackage* temp = new userPackage;
            temp->userID = theId;
            temp->startPoint = 0;
            temp->speed = 1.0;
            temp->RampValue = 1;
            temp->initialLoc = 0;
            userPackages.push_back(temp);
            atLeastOneUserConnected = true;
            cout << userPackages.size() << "packages" << endl;
            
            //VISUAL FEEDBACK:
            box.push_back(new Particle(ofVec2f(ofRandom(0, 100), ofRandom(0, 200))));
        }
        
        if(n.getAddress() == "/Grain-Size")
        {
            string param = n.getArgAsString(0);
            string username = n.getArgAsString(1);
            int value = atoi(param.c_str());
            cout << "Grain Size :" << value << endl;
            cout << "WHICH USER :" << username << endl;
            int paramChange = value;
            
            for(int i = 0; i < userPackages.size(); i++)
            {
                if(username == userPackages[i]->userID)
                {
                    userPackages[i]->grainSize = paramChange;
                }
            }
        }
        
        if(n.getAddress() == "/Location")
        {
            state1 = true;
            string param = n.getArgAsString(0);
            string username = n.getArgAsString(1);
            float value = atof(param.c_str());
            cout << "Location :" << value << endl;
            cout << "WHICH USER :" << username << endl;
            //int paramChange = value;
            StartparamChange = value;
            
//            for(int i = 0; i < userPackages.size(); i++)
//            {
//                if(username == userPackages[i]->userID)
//                {
//
//                    if(endOfGrain == true)
//                    {
//                        state1 = true;
//                        userPackages[i]->startPoint = StartparamChange;
//                        initialLoc = StartparamChange;
//                    }
//                }
//            }
        }
        
        if(n.getAddress() == "/Speed")
        {
            string param = n.getArgAsString(0);
            string username = n.getArgAsString(1);
            float value = atof(param.c_str());
            cout << "SPEED :" << value << endl;
            cout << "WHICH USER :" << username << endl;
            float speedChange = value;
            
            for(int i = 0; i < userPackages.size(); i++)
            {
                if(username == userPackages[i]->userID)
                {
                    userPackages[i]->speed = speedChange;
                }
            }
        }
        
        
        if(n.getAddress() == "/Ramp")
        {
            state1 = false;
            string param = n.getArgAsString(0);
            string username = n.getArgAsString(1);
            float value = atof(param.c_str());
            cout << "RAMP :" << value << endl;
            cout << "WHICH USER :" << username << endl;
            //float RampChange = value;
            rampParamChange = value;
            
//            for(int i = 0; i < userPackages.size(); i++)
//            {
//                if(username == userPackages[i]->userID)
//                {
//
//                    if(endOfGrain == true)
//                    {//state1 = false;
//                    userPackages[i]->RampValue = rampParamChange;
//                    }
//                }
//            }
        }
    }
    
    if(atLeastOneUserConnected == true)
    {
        for(int i = 0; i < userPackages.size(); i++)
        {

                                if(state1 == true)
                                {
                                    if(swapParams == true)
                                    {
                                        userPackages[i]->startPoint = StartparamChange;
                                        userPackages[i]->initialLoc =  StartparamChange;
                             
                                    }
                                    swapParams = false;
                                    
                                }
                                else if(state1 == false)
                                {
                                    userPackages[0]->RampValue = rampParamChange;
                                }
                                user0ready = true;

        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(readyToSendFileSize == true)
    {
            ofxOscMessage m;
            m.setAddress("/NumSamples");
            m.addIntArg(fileSize);
            atak.sendMessage(m);
            readyToSendFileSize = false;
    }
    
    
    for(int i = 0; i < box.size(); i++)
    {
            box[i]->display();
    }
    
} //close draw


//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs & args){
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::updateEnvelope(ofMouseEventArgs & args){
    if(soundIsLoaded == true)
    {
        
    }
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels)
{
    // ------ START CYCLING THRU AUDIO BUFFER: ------
    for (int i = 0; i < bufferSize; i++)
    {

        if(atLeastOneUserConnected == true && user0ready == true)
           {

               for(int i = 0; i < userPackages.size(); i++)
              {

                  //SET UP RAMP
                     userPackages[i]->phasorValue = userPackages[i]->phasor.phasor(userPackages[i]->RampValue, 0, 1);
               
                  // MAP RAMP VALUE TO FILE LOCATION
                  userPackages[i]->phasorLocation = ofMap(userPackages[i]->phasorValue, 0, 1, 0, fileSize);
                  userPackages[i]->absPhasorLoc = abs(userPackages[i]->phasorLocation);

                  // CALCULATE ENVELOPE:
                  userPackages[i]->envelope = userPackages[i]->windowFunction.cosineHann(0, userPackages[i]->grainSize, userPackages[i]->speed);
                  //cout << "envelope value : " << userPackages[0]->envelope << endl;
                  
                 if(userPackages[i]->envelope == 0)
                 {
                     endOfGrain = true;
                     
                 }
                 else {
                     endOfGrain = false;
                     
                 }
                  
                  // WORK OUT SAMPLE INTERPOLATION:
                  userPackages[i]->interps1 = interpolator(rightSoundFile, userPackages[i]->startPoint, userPackages[i]->grainSize);
                 // cout << "interp" << userPackages[0]->interps1 << endl;
                  //MULTIPLY INTERPOLATION RESULT BY ENVELOPE:
                  userPackages[i]->interps2 = multiplyInterpSamples(userPackages[i]->envelope, userPackages[i]->interps1);
                //  cout << "interp2" << userPackages[0]->interps2 << endl;
                  
              }
               
                double rightMix = 0;
               
               for(int i = 0; i < userPackages.size(); i++)
               {
                   userPackages[i]->startPoint += userPackages[i]->speed;
                
              
                if(userPackages[i]->startPoint >= userPackages[i]->initialLoc + userPackages[i]->grainSize)
                {
                
                    swapParams = true;
                    if(state1 == true )
                    {
                    userPackages[i]->startPoint = userPackages[i]->initialLoc;
                    }
                    
                    else if(state1 == false)
                    {
                    userPackages[i]->startPoint = userPackages[i]->absPhasorLoc;
                    userPackages[i]->initialLoc = userPackages[i]->absPhasorLoc;
                    }
                    
                }
                   
                    rightMix += userPackages[i]->interps2;
               }
        
               float maxLatenxy = (1000 * 4) * (bufferSize/sampleRate); //46.43 ms
               double leftMix;
              
               
                //   cout << "mix" << rightMix << endl;
                
               
         
               
               //LEFT CHANNEL:
               output[i*nChannels] = 0;
            
               //RIGHT
               output[i*nChannels + 1] = rightMix * vol1;


           } // close atLeastOneUserConnected bool
        
    }//end of audio buffer loop

} //CLOSE AUDIO LOOP


//-----------
//double ofApp::multiplySamples(double &num, int index, const vector<double> &array)
//{
//   return num * array[playIndex];
//}

//---------
double ofApp::interpolator(vector<double> &arr, double index, int grainsize)
{
    int x0 = (int)index;
    int x1 = (int)(index+1.0);
    double f0, f1;
    
    
    if(x1 >= index+ grainsize)
    {
        x1 = index - grainsize;
    }
    
    f0 = arr[x0];
    f1 = arr[x1];
    float result;
    

    result =f0 + ((f1 - f0) * (index - x0));

    return result;//f0 + ((f1 - f0) * (index - x0));
}
//--------

double ofApp::multiplyInterpSamples(double &num, double &envy)
{
    return num * envy;
}
//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    // samples are "interleaved"
    for(int i = 0; i < bufferSize; i++){
        // you can also grab the data out of the arrays
        
    }
    
}

//--------------------------------------------------------------
void ofApp::convertToMono(){
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == ' ')
    {
        string mypathway = openDialogBox();
        soundFile = openSoundFile(mypathway.c_str());
        //   cout << "soundFile size : " << soundFile.size() << endl; // 2131564
        //soundIsLoaded = true;
        newSoundLoaded = true;
    }
    
    //for (std::vector<float>::const_iterator i = soundFile.begin(); i != soundFile.end(); ++i)
    //std::cout << *i << ' ';
    
    //for (int i=0;i<soundFile.size();i++){
    //     std::cout << "SOUNDFILE"<<soundFile.at(i) << std::endl;
    //   }
    
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

uint64_t ofApp::getDurationMS()
{
    return uint64_t(ofGetFrameNum()) * uint64_t(1000) / uint64_t(sampleRate);
}

//-----------------------------
string ofApp::openDialogBox()
{
    //open the load dialog
    ofFileDialogResult loadFileResult = ofSystemLoadDialog();
    
    //store the string
    string path = loadFileResult.getPath();
    
    return path;;
    
}


//----------------------------
vector<double> ofApp::openSoundFile(const char *filePath)
{
    
    //identification for the sound file, used by the library:
    int ifd = -1;
    
    //will be auto-populated when file loaded:
    PSF_PROPS soundFileProperties;
    
    ifd = psf_sndOpen(filePath, &soundFileProperties, 0);//open the sound file
    // cout << "song" << mySong.ifd << endl;
    if (ifd < 0)
    {   //this can happen if the file does not exist or the format is not supported
        ofSystemAlertDialog("Error loading file");
    }
    
    //framesread used to check if there are still more frames in file
    long framesRead, totalFramesRead;
    vector<double> soundFile;//actual stored sound file

    //location of currently read frame
    //pointer of a float says 'go to the address of a float'
    //this is because we do not yet know how big a frame is:
    double* frame = NULL;
    
    frame = (double*)malloc(soundFileProperties.chans * sizeof(double));
    //set the size of frame to be the nSamples per Frame
    //malloc is memory allocate. so we allocate memory for that frame
    //sizeof(float) is likely to be 32 bits.
    //as we have two channels (stereo file), it'll be 32 bits x 2 = 64 bits.
    
    framesRead = psf_sndReadDoubleFrames(ifd, frame, 1);//grab the first frame
    //which sound file location to read, where to put it, and how many frames to read
    //this function returns a 1 if there are more frames left
    
    //we start with:
    totalFramesRead = 0;
    soundFile.clear();//clear the vector that the soundFile will be stored in
    
    
    while (framesRead == 1)
    {   //while there are still frames to be read
        totalFramesRead++;      //keep track of number of frames;
        for (int i = 0; i < soundFileProperties.chans; i++)
        {   //for every sample in the frame
            //add frame to the soundFile vector
            soundFile.push_back(frame[i]);
            
        }
        
        //framesRead returns 1 if there are still more frames:
        framesRead = psf_sndReadDoubleFrames(ifd, frame, 1);
        //  cout << totalFramesRead << "framesre " << endl;
        
    }
    return soundFile;
    
}

//--------------------------------------------------------------
double* ofApp::leftChannel(vector<double> soundFile)
{
    //vector<double> leftTemp;
    double *leftTemp;
    leftTemp = (double*)std::malloc((soundFile.size()/2) * sizeof(double));
    
    for(int i = 0; i < soundFile.size(); i+=2) //left channel beings on the first index
    {
        leftTemp[i] = soundFile[i];
    }
   int size = sizeof(&leftTemp);
    cout << "TEMP" << size << endl;
    cout << "TEMP" << leftTemp[1065790] << endl;
    return leftTemp;
}

//--------------------------------------------------------------
vector<double> ofApp::rightChannel(vector<double> soundFile)
{
    vector<double> rightTemp;
    rightTemp.clear();
    for(int i = 1; i < soundFile.size(); i+=2)
    {
        rightTemp.push_back(soundFile[i]);
    }
    return rightTemp;
}

//--------------------------------------------------------------
void ofApp::sendFileSize(ofxOscSender &sender, int fileSize)
{
    ofxOscMessage m;
    m.setAddress("/NumSamples");
    m.addIntArg(fileSize);
    sender.sendMessage(m);
}

//--------------------------------------------------------------
void ofApp::exit() {
    ofSoundStreamClose();
    psf_finish();//safety. will close sound files on exit
}

//-----------------------------

