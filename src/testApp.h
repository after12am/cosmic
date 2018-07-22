#pragma once

#include "ofMain.h"
#include "ofxControlPanel.h"
#include <vector.h>
#include "Particle.h"
#include "Planet.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
	
	ofxControlPanel gui;
	ofEasyCam cam;
	ofFbo fbo;
	ofSoundPlayer sound;
	ofImage emitterImage;
	ofImage particleImage;
	ofImage reflectionImage;
	
	vector<Particle*> particles;
	vector<Planet*> planets;
	
	void setupTexture();
};
