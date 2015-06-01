#include "particle.h"

ofColor colors[1];

//------------------------------------------------------------------
Particle::Particle()
{
    isAlive = false;
    type = 1;
    /*colors[2] = ofColor(205, 190, 112);
    colors[3] = ofColor(205, 173, 0);
    colors[4] = ofColor(205, 149, 12);
    colors[5] = ofColor(250, 250, 210);
    colors[6] = ofColor(184, 134, 11);
    colors[7] = ofColor(139, 129, 76);
    colors[8] = ofColor(139, 117, 0);
    colors[9] = ofColor(139, 105, 20);
    colors[10] = ofColor(139, 101, 8);
    colors[11] = ofColor(255, 215, 0);
    colors[12] = ofColor(255, 236, 139);*/


}

void Particle::init(int i)
{
    id = i;

    int lpb = ofRandom(100); // LPB homage ^_^

    if( lpb > 90 )
    {
        if( !star.loadImage("star2.png")) printf("Star loading failed");
        star.setAnchorPoint(21,22);
        type = 2;
    }
    else if( lpb > 80 && lpb < 90 )
    {
        if( !star.loadImage("star3.png")) printf("Star loading failed");
        star.setAnchorPoint(21,22);
        type = 3;
    }
    else if( lpb > 60 && lpb < 80 )
    {
        if( !star.loadImage("star4.png")) printf("Star loading failed");
        star.setAnchorPoint(10,10);
        type = 4;
    }
    else if( lpb > 45 && lpb < 60 )
    {
        if( !star.loadImage("star5.png")) printf("Star loading failed");
        star.setAnchorPoint(7,7);
        type = 5;
    }
}

//------------------------------------------------------------------
void Particle::reset( int xPos, int yPos )
{


	//the unique val allows us to set properties slightly differently for each particle
	uniqueVal = ofRandom(-10000, 10000);

	vel.x = ofRandom(-1.9, 1.9);
	vel.y = ofRandom(-0.01, 0.01);

	frc   = ofPoint(0,0,0);

	scale = ofRandom(0.1, 1.2);
    drag  = ofRandom(0.97, 0.99);
    vel.y = fabs(vel.y) * 0.01; //make the particles all be going down

    alpha = 80.0f;

    pos.x = xPos;
    pos.y = yPos;

    isAlive = true;

    /*colors[0] = ofColor(250, 251, 180);
    colors[1] = ofColor(231, 225, 213);

    color = colors[(int)ofRandom(sizeof(colors))];*/
}

//------------------------------------------------------------------
void Particle::update()
{
    //lets simulate falling snow
    //the fake wind is meant to add a shift to the particles based on where in x they are
    //we add pos.y as an arg so to prevent obvious vertical banding around x values - try removing the pos.y * 0.006 to see the banding
    float fakeWindX = ofSignedNoise(pos.x * 0.003, ofGetElapsedTimef() * 0.6);

    frc.x = fakeWindX * 0.25 + ofSignedNoise(uniqueVal, pos.y * 0.04) * 0.6;
    frc.y = ofSignedNoise(uniqueVal, pos.x * 0.006, ofGetElapsedTimef()*0.2) * 0.09 + 0.18;

    vel *= drag;

    if( type != 5 )
        vel += frc * 0.3;
    else
        vel += frc * 0.01;

	// Update position and alpha
	pos += vel;

    if( type == 2 || type == 3 )
        alpha -= 0.2f;
    else
        alpha -= 0.5f;

    // Reset the particle
	if( alpha < 6.0f || pos.y > ofGetHeight() )
	{
	    isAlive = false;
	}

    ofEnableAlphaBlending();

	if( star.isAllocated() )
	{
	    ofSetColor(255,255,255, (int)alpha);
        star.draw(pos.x, pos.y);
	}
	else
	{
        ofSetColor(250, 251, 180, ofRandom(255));
        ofCircle(pos.x, pos.y, scale * 3.0);
	}

    //ofSetColor(255,255,255);
	//ofDrawBitmapString(ofToString(id), pos.x, pos.y);
}

