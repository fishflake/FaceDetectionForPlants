#include "ofApp.h"
#include "ofAppGlutWindow.h"


//========================================================================
int main( ){
    ofAppGlutWindow window;
    ofSetupOpenGL(800,800, OF_WINDOW);            // <-------- setup the GL context
    ofRunApp(new ofApp());
}
