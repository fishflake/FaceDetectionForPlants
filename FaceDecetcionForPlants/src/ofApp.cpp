#include "ofApp.h"

/*
 * If you have a servo attached, press the left arrow key to rotate
 * the servo head counterclockwise and press the right arrow key to
 * rotate the servo head clockwise.
 *
 * Clicking the mouse over any part of the application will turn the
 * on-board LED on and off.
 *
 */

using namespace ofxCv;


//--------------------------------------------------------------

void ofApp::setup() {
    cam.initGrabber(20, 20);
    tracker.setup();
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    ofBackground(255);
    
    buttonState = "digital pin:";
    potValue = "analog pin:";
    
    bgImage.load("background.png");
    font.load("franklinGothic.otf", 20);
    smallFont.load("franklinGothic.otf", 14);
    
    // replace the string below with the serial port for your Arduino board
    // you can get this from the Arduino application or via command line
    // for OSX, in your terminal type "ls /dev/tty.*" to get a list of serial devices
    //    ard.connect("/dev/tty.usbmodemfd121", 57600);
    ard.connect("/dev/cu.usbmodem1431", 57600);
    
    // listen for EInitialized notification. this indicates that
    // the arduino is ready to receive commands and it is safe to
    // call setupArduino()
    ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
    bSetupArduino    = false;    // flag so we setup arduino when its ready, you don't need to touch this :)
    
}

//--------------------------------------------------------------
void ofApp::update() {
    cam.update();
    if(cam.isFrameNew()) {
        tracker.update(toCv(cam));
        tracker.getFound();
    }
    
    updateArduino();
    
    
    
    if(tracker.getFound()){
        // rotate servo head to 180 degrees
        ard.sendServo(9, 180, false);
    }
    else{
        // rotate servo head to 0 degrees
        ard.sendServo(9, 0, false);
    }
    
    orientation = tracker.getOrientation();
    


}

//--------------------------------------------------------------
void ofApp::setupArduino(const int & version) {
    
    // remove listener because we don't need it anymore
    ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    // print firmware name and version to the console
    ofLogNotice() << ard.getFirmwareName();
    ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
    
    // Note: pins A0 - A5 can be used as digital input and output.
    // Refer to them as pins 14 - 19 if using StandardFirmata from Arduino 1.0.
    // If using Arduino 0022 or older, then use 16 - 21.
    // Firmata pin numbering changed in version 2.3 (which is included in Arduino 1.0)
    
//    // set pins D2 and A5 to digital input
//    ard.sendDigitalPinMode(2, ARD_INPUT);
//
//    // set pin A0 to analog input
//    ard.sendAnalogPinReporting(0, ARD_ANALOG);
//
//    // set pin D13 as digital output
//    ard.sendDigitalPinMode(13, ARD_OUTPUT);
//    // set pin A4 as digital output
//
//    // set pin D11 as PWM (analog output)
//    ard.sendDigitalPinMode(11, ARD_PWM);
    
    // attach a servo to pin D9
    // servo motors can only be attached to pin D3, D5, D6, D9, D10, or D11
    ard.sendServoAttach(9);
    
    // Listen for changes on the digital and analog pins
    ofAddListener(ard.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
    ofAddListener(ard.EAnalogPinChanged, this, &ofApp::analogPinChanged);
}

void ofApp::updateArduino(){
    
    // update the arduino, get any data or messages.
    // the call to ard.update() is required
    ard.update();
    
    // do not send anything until the arduino has been set up
    if (bSetupArduino) {
        // fade the led connected to pin D11
        ard.sendPwm(11, (int)(128 + 128 * sin(ofGetElapsedTimef())));   // pwm...
    }
    
    
}

// digital pin event handler, called whenever a digital pin value has changed
// note: if an analog pin has been set as a digital pin, it will be handled
// by the digitalPinChanged function rather than the analogPinChanged function.

//--------------------------------------------------------------
void ofApp::digitalPinChanged(const int & pinNum) {
    // do something with the digital input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    buttonState = "digital pin: " + ofToString(pinNum) + " = " + ofToString(ard.getDigital(pinNum));
}

// analog pin event handler, called whenever an analog pin value has changed

//--------------------------------------------------------------
void ofApp::analogPinChanged(const int & pinNum) {
    // do something with the analog input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    potValue = "analog pin: " + ofToString(pinNum) + " = " + ofToString(ard.getAnalog(pinNum));
}

//--------------------------------------------------------------

void ofApp::draw() {
    ofSetColor(255);
    cam.draw(0, 0);
    ofSetLineWidth(2);
    tracker.draw();
//        ofPolyline noseBase = tracker.getImageFeature(ofxFaceTracker::NOSE_BASE);
    //    ofSetColor(ofColor::red);
    //    noseBase.draw();
    //    ofDrawCircle(noseBase.getCentroid2D(), 8 * tracker.getScale());
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);

    
//    Direction faceOrientation = getDirection() const;
    
    ofEnableAlphaBlending();
    ofSetColor(0, 0, 0, 127);
    ofDrawRectangle(510, 15, 275, 150);
    ofDisableAlphaBlending();
    
    ofSetColor(255);

    if(tracker.getFound()){
        ofDrawBitmapString("FACE", 600, 150);
    }
    else{
        ofDrawBitmapString("NO FACE", 600, 150);
    }
    
        ofSetColor(255, 255, 255);
        if (!bSetupArduino){
            font.drawString("arduino not ready...\n", 515, 40);
        } else {
            font.drawString(potValue + "\n" + buttonState +
                            "\nsending pwm: " + ofToString((int)(128 + 128 * sin(ofGetElapsedTimef()))), 515, 40);
            
            ofSetColor(64, 64, 64);
            smallFont.drawString("If a servo is attached, use the left arrow key to rotate "
                                 "\ncounterclockwise and the right arrow key to rotate clockwise.", 200, 550);
            ofSetColor(255, 255, 255);
            
        }
    
    ofDrawBitmapString(orientation, 520, 125);
    
}
//--------------------------------------------------------------

void ofApp::keyPressed(int key) {
    if(key == 'r') {
        tracker.reset();
    }
    switch (key) {
        case OF_KEY_RIGHT:
            // rotate servo head to 180 degrees
            ard.sendServo(9, 180, false);
            ard.sendDigital(18, ARD_HIGH);  // pin 20 if using StandardFirmata from Arduino 0022 or older
            break;
        case OF_KEY_LEFT:
            // rotate servo head to 0 degrees
            ard.sendServo(9, 0, false);
            ard.sendDigital(18, ARD_LOW);  // pin 20 if using StandardFirmata from Arduino 0022 or older
            break;
        default:
            break;
    }
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
    // turn on the onboard LED when the application window is clicked
    ard.sendDigital(13, ARD_HIGH);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    // turn off the onboard LED when the application window is clicked
    ard.sendDigital(13, ARD_LOW);
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
