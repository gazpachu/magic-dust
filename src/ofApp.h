#pragma once
#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofTrueTypeFont.h"
#include "ofxBlurShader.h"
#include "Particle.h"

class ofApp : public ofBaseApp
{
public:
    
    void setup();
    void update();
    void draw();
    
    void reset();
    void drawFbo();
    void drawShape();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    int particlerPerFrame, nearThreshold, farThreshold, score, frame, fadeAmnt, index, totalParticles, strokeOpacity, blurAmount;
    float filterFactor, strokeWidth, oldY, oldX, xPos, yPos;
    bool started, blurEnabled, reseting, smooth;
    
    ofxOpenNIContext niContext;
    ofxDepthGenerator niDepthGenerator;
    ofxImageGenerator niImageGenerator;
    ofxUserGenerator niUserGenerator;
    ofxHandGenerator* niHandGenerator;
    ofxTrackedHand* tracked;
    
    ofImage bg, star, tinkerbell;
    
    ofTrueTypeFont font;
    
    ofFbo rgbaFbo;
    ofxBlurShader* blur;
    ofShader shader;
    
    vector <Particle> particles;
    vector <ofPoint> shapes;
};









