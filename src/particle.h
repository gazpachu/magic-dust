#pragma once
#include "ofMain.h"

class Particle
{
	public:

		Particle();

		void reset(int xPos, int yPos);
		void update();
		void draw();
		void init(int i);

		ofPoint pos;
		ofPoint vel;
		ofPoint frc;

		float drag;
		float uniqueVal;
		float scale;

		float alpha;
		ofColor color;

        bool isAlive;
        int type;
        int id;
		ofImage star;
};
