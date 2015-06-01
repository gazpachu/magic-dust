#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    //ofBackground(0);
    //ofSetFrameRate(60);
    //ofSetVerticalSync(true);
    //ofSetLogLevel(OF_LOG_VERBOSE);
    
    if( ! bg.loadImage("disney_bg.jpg")) printf("Bg loading failed");
    if( ! star.loadImage("star.png")) printf("Star loading failed");
    if( ! tinkerbell.loadImage("tinkerbell.png")) printf("Tinkerbell loading failed");
    
    star.setAnchorPoint(16,17);
    
    font.loadFont("bebasneue-webfont.ttf", 25);
    
    nearThreshold = 400;
    farThreshold = 700;
    strokeOpacity = 230;
    strokeWidth = 8.0f;
    particlerPerFrame = 5;
    blurAmount = 1;
    fadeAmnt = 1;
    index = 0;
    totalParticles = 1000;
    
    started = false;
    blurEnabled = false;
    reseting = false;
    smooth = false;
    
    niContext.setup();
    //niContext.toggleRegisterViewport();
    niContext.toggleMirror();
    
    niDepthGenerator.setup( &niContext );
    niImageGenerator.setup( &niContext );
    
    niDepthGenerator.setDepthThreshold(0, nearThreshold, farThreshold);
    
    niUserGenerator.setup( &niContext );
    niUserGenerator.setSmoothing(filterFactor); // built in openni skeleton smoothing...
    niUserGenerator.setMaxNumberOfUsers(1);
    
    niHandGenerator = new ofxHandGenerator();
    niHandGenerator->setup(&niContext,1);
    niHandGenerator->setMaxNumHands(1);
    niHandGenerator->setSmoothing(0.0f); // built in openni hand track smoothing...
//    niHandGenerator->setFilterFactors(filterFactor); // custom smoothing/filtering (can also set per hand with setFilterFactor)...set them all to 0.1f to begin with
//    niHandGenerator->startTrackHands();
    
    particles.assign( totalParticles, Particle() );
    
    for(int i = 0; i < particles.size(); i++)
    {
        particles[i].init( i );
    }
    
    shapes.push_back(ofPoint(1031,316));
    shapes.push_back(ofPoint(959,292));
    shapes.push_back(ofPoint(874,311));
    shapes.push_back(ofPoint(875,239));
    shapes.push_back(ofPoint(852,183));
    shapes.push_back(ofPoint(794,144));
    shapes.push_back(ofPoint(730,141));
    shapes.push_back(ofPoint(677,158));
    shapes.push_back(ofPoint(637,181));
    shapes.push_back(ofPoint(602,215));
    shapes.push_back(ofPoint(582,264));
    shapes.push_back(ofPoint(579,319));
    shapes.push_back(ofPoint(591,367));
    shapes.push_back(ofPoint(610,403));
    shapes.push_back(ofPoint(648,427));
    shapes.push_back(ofPoint(691,435));
    shapes.push_back(ofPoint(738,437));
    shapes.push_back(ofPoint(717,478));
    shapes.push_back(ofPoint(709,529));
    shapes.push_back(ofPoint(705,576));
    shapes.push_back(ofPoint(720,631));
    shapes.push_back(ofPoint(743,672));
    shapes.push_back(ofPoint(771,709));
    shapes.push_back(ofPoint(805,736));
    shapes.push_back(ofPoint(847,761));
    shapes.push_back(ofPoint(893,773));
    shapes.push_back(ofPoint(948,778));
    shapes.push_back(ofPoint(998,772));
    shapes.push_back(ofPoint(1050,758));
    shapes.push_back(ofPoint(1090,731));
    shapes.push_back(ofPoint(1128,698));
    shapes.push_back(ofPoint(1160,654));
    shapes.push_back(ofPoint(1179,612));
    shapes.push_back(ofPoint(1189,557));
    shapes.push_back(ofPoint(1188,508));
    shapes.push_back(ofPoint(1177,465));
    shapes.push_back(ofPoint(1160,430));
    shapes.push_back(ofPoint(1201,436));
    shapes.push_back(ofPoint(1253,428));
    shapes.push_back(ofPoint(1288,406));
    shapes.push_back(ofPoint(1316,360));
    shapes.push_back(ofPoint(1324,304));
    shapes.push_back(ofPoint(1309,246));
    shapes.push_back(ofPoint(1281,197));
    shapes.push_back(ofPoint(1239,159));
    shapes.push_back(ofPoint(1188,141));
    shapes.push_back(ofPoint(1132,139));
    shapes.push_back(ofPoint(1084,158));
    shapes.push_back(ofPoint(1049,188));
    shapes.push_back(ofPoint(1026,230));
    shapes.push_back(ofPoint(1019,266));
    
    
    reset();
}

//--------------------------------------------------------------
void ofApp::reset()
{
    rgbaFbo.allocate((float)ofGetWidth(), (float)ofGetHeight(), GL_RGBA, false); // GL_RGBA32F_ARB
    
    if( blur )
        delete blur;
    
    blur = new ofxBlurShader();
    blur->setup((float)ofGetWidth(), (float)ofGetHeight());
    
    rgbaFbo.begin();
    ofClear(255,255,255, 0);
    rgbaFbo.end();
    
    reseting = false;
}

//--------------------------------------------------------------
void ofApp::update()
{
    niContext.update();
    niDepthGenerator.update();
    niImageGenerator.update();
    niUserGenerator.update();
    
    tracked = niHandGenerator->getHand(0);
    
    if( tracked->projectPos.y && !reseting )
        started = true;
    
    if( started && !reseting )
    {
        oldX = xPos;
        oldY = yPos;
        xPos = (float)(tracked->projectPos.x * (ofGetWidth() / 640));
        yPos = (float)(tracked->projectPos.y * (ofGetHeight() / 480));

        
        rgbaFbo.begin();
        drawFbo();
        rgbaFbo.end();
    }
    else
    {
        xPos = 810.0;
        yPos = 370.0;
    }
}

//--------------------------------------------------------------
void ofApp::drawFbo()
{
    ofFill();
    ofSetColor(255.0,255.0,255.0, fadeAmnt);
    ofRect(0.0,0.0,(float)ofGetWidth(),(float)ofGetHeight());
    
    ofSetLineWidth(strokeWidth);
    
    if( blurEnabled && started )
    {
        blur->begin();
        ofSetColor(231.0, 225.0, 213.0, strokeOpacity);
        ofLine(oldX, oldY, xPos, yPos);
        blur->amount = ofMap(xPos,0.0,(float)ofGetWidth(),0.0,blurAmount,true);
        blur->iterations = ofMap(yPos,0.0,(float)ofGetHeight(),1.0,blurAmount,true);
        blur->end();
    }
    
    //drawShape();
}

//--------------------------------------------------------------
void ofApp::drawShape()
{
    for( int i = 0; i < shapes.size(); i++ )
    {
        ofSetColor(255, 255, 255, ofRandom(255));
        ofCircle( shapes[i].x, shapes[i].y, 2);
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofSetColor(255,255,255);
    bg.draw(0,0);
    
    ofEnableAlphaBlending();
    
    if( !started )
    {
        tinkerbell.draw(200,200);
        font.drawString("SHOW ME YOUR HAND TO START PLAYING", ofGetWidth()/2 - 50, ofGetHeight()/2);
        font.drawString("(press spacebar to restart)", ofGetWidth()/2 - 50, ofGetHeight()/2 + 50);
    }
    else
    {
        ofDrawBitmapString("Position: " + ofToString(xPos) + ", " + ofToString(yPos), 20,20);
        ofDrawBitmapString("FPS: " + ofToString( ofGetFrameRate(), 2 ), 20, 40);
        //ofDrawBitmapString("Smoothing: " + ofToString( smooth ), 20, 60);
        ofDrawBitmapString("Blur status: " + ofToString( blurEnabled ), 20, 60);
        ofDrawBitmapString("Blur amount: " + ofToString( blurAmount ), 20, 80);
        ofDrawBitmapString("Trail opacity: " + ofToString( strokeOpacity ), 20, 100);
        ofDrawBitmapString("Trail width: " + ofToString( strokeWidth ), 20, 120);
        ofDrawBitmapString("Fade amount: " + ofToString( fadeAmnt ), 20, 140);
        ofDrawBitmapString("Particles per frame: " + ofToString( particlerPerFrame ), 20, 160);
        
        ofDrawBitmapString("Toggle Fullscreen: 'F'", ofGetWidth() - 200,20);
        //ofDrawBitmapString("Toggle Smoothing: 'S'", ofGetWidth() - 200,40);
        ofDrawBitmapString("Toggle Blur: 'B'", ofGetWidth() - 200,40);
        ofDrawBitmapString("Set Blur Amount: 'Q/W'", ofGetWidth() - 200,60);
        ofDrawBitmapString("Set Particles: '+/-'", ofGetWidth() - 200,80);
        ofDrawBitmapString("Set Trail Opacity: 'O/P'", ofGetWidth() - 200,100);
        ofDrawBitmapString("Set Trail Width: 'U/I'", ofGetWidth() - 200,120);
        ofDrawBitmapString("Set Fade Amount: 'A/S'", ofGetWidth() - 200,140);
        ofDrawBitmapString("Restart: 'Spacebar'", ofGetWidth() - 200,160);
        
        star.draw(xPos, yPos);
        rgbaFbo.draw(0.0,0.0);
    }
    
    if( blurEnabled && started )
    {
        blur->begin();
        blur->amount = ofMap(xPos,0.0,(float)ofGetWidth(),0.0,1,true);
        blur->iterations = ofMap(yPos,0.0,(float)ofGetHeight(),1.0,1,true);
    }
    
    int sparksCreated = 0;
    
    for(int i = 0; i < totalParticles; i++)
    {
        if( particles[i].isAlive )
        {
            particles[i].update();
        }
        else if( sparksCreated < particlerPerFrame )
        {
            particles[i].reset(xPos, yPos);
            sparksCreated++;
        }
    }
    
    if( blurEnabled && started )
        blur->end();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if(key == 'f')
        ofToggleFullscreen();
    
    /*if(key == 's')
     {
	    if( smooth )
	    {
     niHandGenerator->setSmoothing(0.0f);
     smooth = false;
	    }
     else
     {
     niHandGenerator->setSmoothing(0.1f);
     smooth = true;
     }
     }*/
    
    if(key == 'b')
    {
        if( blurEnabled )
            blurEnabled = false;
        else
            blurEnabled = true;
    }
    
    if(key == '+')
    {
        if( particlerPerFrame < 20 )
            particlerPerFrame++;
    }
    
    if(key == '-')
    {
        if( particlerPerFrame > 0 )
            particlerPerFrame--;
    }
    
    if(key == 'p')
    {
        if( strokeOpacity < 255 )
            strokeOpacity++;
    }
    
    if(key == 'o')
    {
        if( strokeOpacity > 0 )
            strokeOpacity--;
    }
    
    if(key == 'i')
    {
        if( strokeWidth < 40.0f )
            strokeWidth++;
    }
    
    if(key == 'u')
    {
        if( strokeWidth > 0.0f )
            strokeWidth--;
    }
    
    if(key == 'w')
    {
        if( blurAmount < 10 )
            blurAmount++;
    }
    
    if(key == 'q')
    {
        if( blurAmount > 0 )
            blurAmount--;
    }
    
    if(key == 'a')
    {
        if( fadeAmnt < 255 )
            fadeAmnt++;
    }
    
    if(key == 's')
    {
        if( fadeAmnt > 1 )
            fadeAmnt--;
    }
    
    if(key == 32) //spacebar
    {
        started = false;
        reseting = true;
        niHandGenerator->dropHands();
        
        delete tracked;
        delete niHandGenerator;
        niHandGenerator = new ofxHandGenerator();
        niHandGenerator->setup(&niContext,1);
        niHandGenerator->setMaxNumHands(1);
        
        reset();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    
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
