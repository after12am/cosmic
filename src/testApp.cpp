#include "testApp.h"
#include <GLUT/GLUT.h>
#include "Particle.h"
#include "utils.h"
#include "Planet.h"

const int size = 512;
float *fftValues;
float normalizedValues[size];
float avgValue = 0;
float maxValue = 0;

bool hideGui = false;

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofEnableSmoothing();
	ofEnableNormalizedTexCoords();
	ofBackground(0, 0, 0);
	//ofHideCursor();
	
	gui.setup("audio", 10, 10, 340, 340);
	gui.addPanel("main", 3);
	gui.selectedPanel = 0;
	gui.addSlider("gravity", "gravity", 0.99, 0, 1, false);
	gui.addSlider("maxThreshold", "maxThreshold", 0.48, 0, 3, false);
	gui.addSlider("particleRandThreshold", "particleRandThreshold", 0.3, 0, 3, false);
	gui.addSlider("maxSpeed", "maxSpeed", 0.6, 0, 10, false);
	gui.addSlider("tooCloseDist", "tooCloseDist", 140, 0, 300, true);
	gui.addSlider("scatterNum", "scatterNum", 40, 0, 100, true);
	
	sound.loadSound("Royksopp - In Space.mp3");
	sound.setLoop(false);
	//sound.play();
	sound.stop();
	//sound.setPosition(0.14673);
	
	setupTexture();
	
	
	cam.setDistance(80);
	
	/* original planet
	 ===================*/
	Planet* p = new Planet(1, 0, 0, 0);
	planets.push_back(p);
	
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	
	maxValue = 0;
	avgValue = 0;
	
	//cout << sound.getPosition() << endl;
	
	/* analyse sound
	 ==============================*/
	fftValues = ofSoundGetSpectrum(size);
	
	if (size > 0)
	{
		for (int i = 0; i < size; i++)
		{
			if (fftValues[i] > maxValue) maxValue = fftValues[i];
		}
		
		for (int i = 0; i < size; i++)
		{
			normalizedValues[i] = fftValues[i] / maxValue;
			avgValue += normalizedValues[i];
		}
		
		avgValue /= size;
	}
	
	/* create planet
	 ===================*/
	if (maxValue > gui.getValueF("maxThreshold")) {
		
		// maximum size is 5
		if (planets.size() <= 3) {
			// copy from original planet
			ofVec3f pos = planets[0]->position;
			Planet* p = new Planet(2, pos.x, pos.y, pos.z);
			p->maxSpeed = gui.getValueF("maxSpeed");
			p->tooCloseDist = gui.getValueI("tooCloseDist");
			planets.push_back(p);
		}
	}
	
	
	/* create particles
	 ===================*/
	for (int j = 0; j < planets.size(); j++) {
		
		for (int i = 0; i < int(maxValue * gui.getValueI("scatterNum")); i++) {
			
			Planet* o = planets[j];
			Particle* p = new Particle(1, o->position.x, o->position.y, o->position.z);
			p->velocity = utils::getRand() * maxValue * 10;
			particles.push_back(p);
		}
	}
	
	
	/* update planets
	 ===================*/
	for (int i = 0; i < planets.size(); i++) {
		Planet* p = planets[i];
		p->aging(maxValue / 2.f);
		p->maxSpeed = gui.getValueF("maxSpeed");
		p->tooCloseDist = gui.getValueI("tooCloseDist");
		
		for (int j = 0; j < planets.size(); j++) {
			
			if (i == j) continue;
			
			Planet* t = planets[j];
			if (p->tooClose(t->position)) {
				p->flee(t->position);
				
				ofVec3f randVec = utils::getRand();
				p->addForce(randVec);
				break;
			}
		}
		p->arrive(ofVec3f(0, 0, 0));
		p->update();
	}
	
	
	/* update particles
	 ===================*/
	ofVec3f crossVec;
	crossVec.set(0, 1, 0);
	crossVec.rotate(ofGetFrameNum(), 0, ofGetFrameNum() * 0.5);
	
	for (int i = 0; i < particles.size(); i++) {
		Particle* p = particles[i];
		
		// rotation
		p->force.set(p->position - ofVec3f(0, 0, 0));
		ofVec3f tangentVec = p->force.crossed(crossVec);
		tangentVec.normalize();
		tangentVec *= maxValue;
		p->force += tangentVec;
		p->force += -p->position * gui.getValueF("gravity");
		
		if (maxValue > gui.getValueF("particleRandThreshold")) {
			ofVec3f l = p->velocity.length();
			
			p->velocity += utils::getRand() * 10;
			p->velocity.normalize();
			p->velocity *= l;
		}
		
		p->aging(maxValue * 2 + 0.3);
		p->update();
	}
	
	
	/* delete dead planets
	 ========================*/
	vector<Planet*> _planets;
	
	for (int i = 0; i < planets.size(); i++) {
		
		// delete only copied planets
		if (planets[i]->gen > 1 && planets[i]->isDead()) {
			delete planets[i];
		} else {
			_planets.push_back(planets[i]);
		}
	}
	planets.swap(_planets);
	
	
	/* delete dead particles
	 ========================*/
	vector<Particle*> _particles;
	
	for (int i = 0; i < particles.size(); i++) {
		
		if (particles[i]->isDead()) {
			delete particles[i];
		} else {
			_particles.push_back(particles[i]);
		}
	}
	particles.swap(_particles);
	
	/* gui
	 =======*/
	gui.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* draw particles
	 =================*/
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	
	cam.begin();
	
//	for (int i = 0; i < planets.size(); i++) {
//		planets[i]->draw();
//	}
	
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->draw();
	}
	
	cam.end();
	
	/* draw gui
	 ===========*/
	if (!hideGui) {
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_ALPHA_TEST);
		gui.draw();
		ofSetColor(ofColor(255));
		ofDrawBitmapString("max: " + ofToString(maxValue), 10, ofGetHeight() - 80);
		ofDrawBitmapString("planet num: " + ofToString(planets.size()), 10, ofGetHeight() - 60);
		ofDrawBitmapString("particle num: " + ofToString(particles.size()), 10, ofGetHeight() - 40);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if (key == 'f') {
		ofToggleFullscreen();
	}
	if (key == 32) {
		bool playing = sound.getIsPlaying();
		
		if (playing) {
			sound.stop();
		} else {
			sound.play();
		}
	}
	
	if (key == 'h') {
		hideGui = !hideGui;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	gui.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	gui.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	gui.mouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::setupTexture()
{
	emitterImage.loadImage("img/emitter.png");
	particleImage.loadImage("img/particle.png");
	reflectionImage.loadImage("img/reflection.png");
	ofTexture emitterTexture = emitterImage.getTextureReference();
	ofTexture particleTexture = particleImage.getTextureReference();
	ofTexture reflectionTexture = reflectionImage.getTextureReference();
	
	
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_CULL_FACE);
	
	glNewList(1, GL_COMPILE);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glPushMatrix();
	emitterTexture.bind();
	glNormal3d(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 1.0);
	glVertex3d(-1.0, -1.0,  0.0);
	glTexCoord2d(1.0, 1.0);
	glVertex3d( 1.0, -1.0,  0.0);
	glTexCoord2d(1.0, 0.0);
	glVertex3d( 1.0,  1.0,  0.0);
	glTexCoord2d(0.0, 0.0);
	glVertex3d(-1.0,  1.0,  0.0);
	glEnd();
	emitterTexture.unbind();
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glEndList();
	
	glNewList(2, GL_COMPILE);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glPushMatrix();
	particleTexture.bind();
	glNormal3d(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 1.0);
	glVertex3d(-1.0, -1.0,  0.0);
	glTexCoord2d(1.0, 1.0);
	glVertex3d( 1.0, -1.0,  0.0);
	glTexCoord2d(1.0, 0.0);
	glVertex3d( 1.0,  1.0,  0.0);
	glTexCoord2d(0.0, 0.0);
	glVertex3d(-1.0,  1.0,  0.0);
	glEnd();
	particleTexture.unbind();
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glEndList();
	
	glNewList(3, GL_COMPILE);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glPushMatrix();
	reflectionTexture.bind();
	glNormal3d(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 1.0);
	glVertex3d(-1.0, -1.0,  0.0);
	glTexCoord2d(1.0, 1.0);
	glVertex3d( 1.0, -1.0,  0.0);
	glTexCoord2d(1.0, 0.0);
	glVertex3d( 1.0,  1.0,  0.0);
	glTexCoord2d(0.0, 0.0);
	glVertex3d(-1.0,  1.0,  0.0);
	glEnd();
	reflectionTexture.unbind();
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glEndList();
	
	float sphereColor = 0;
	glNewList(4, GL_COMPILE);
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	glColor4f(sphereColor, sphereColor, sphereColor, 0.8f);
	glutSolidSphere(1, 40, 40);
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	glEndList();
}

