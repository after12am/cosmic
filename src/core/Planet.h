//
//  Planet.h
//  emptyExample
//
//  Created by Okami Satoshi on 12/04/26.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef _Planet_h
#define _Planet_h

#include "ofxBoids.h"

class Planet : public ofxSteeredVehicle {
	
	
public:
	
	int gen;
	float age;
	float lifeSpan;
	float radius;
	
	Planet(int _gen, float _x = 0.f, float _y = 0.f, float _z = 0.f) : ofxSteeredVehicle(_x, _y, _z) {
		
		radius = 10;
		gen = _gen;
		age = 0;
		lifeSpan = (float)ofRandom(10, 50 - (gen - 1) * 10) * 0.2 * 2;
	}
	
	~Planet() {
		
	}
	
	void update() {
		
		ofxSteeredVehicle::update();
		
	}
	
	void draw() {
		
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		utils::billboard();
		glColor4f(remainingLifePer() - 0.5f, remainingLifePer() * 0.25f, 1.5f - remainingLifePer(), 1);
		//glColor4f(remainingLifePer() + 0.5f, remainingLifePer(), 1.5f - remainingLifePer(), 1);
		
		glScalef(radius, radius, radius);
		glCallList(3);
		glPopMatrix();
	}
	
	void aging(float v) {
		age += v;
		age = min(max(age, 0.f), lifeSpan);
	}
	
	float remainingLifePer() {
		return 1.0f - (float)age / lifeSpan;
	}
	
	bool isDead() {
		return lifeSpan <= age;
	}
};


#endif
