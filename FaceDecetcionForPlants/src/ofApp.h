#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"


#include "ofxFaceTracker.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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
    
        ofVideoGrabber cam;
        ofxFaceTracker tracker;
        ofVec3f orientation;

    
    ofImage                bgImage;
    ofTrueTypeFont        font;
    ofTrueTypeFont      smallFont;
    ofArduino    ard;
    bool        bSetupArduino;            // flag variable for setting up arduino once
    
private:
    
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
    void updateArduino();
    
    string buttonState;
    string potValue;
    
    //timer stuff
    float startTime, timer;
    bool timerEnd;

    
    ofxPanel gui;
    ofParameter<float> endTime;
    
		
};
