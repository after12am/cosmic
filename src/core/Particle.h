//
//  Particle.h
//  emptyExample
//
//  Created by Okami Satoshi on 12/04/26.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef _Particle_h
#define _Particle_h

#include "ofMain.h"
#include "utils.h"
#include <math.h>

class Particle {
	
	
public:
	
	ofVec3f position;
	ofVec3f velocity;
	ofVec3f force;
	
	int gen;
	float radius;
	float age;
	float lifeSpan;
	
	ofVec3f bgColor;
	
	Particle(int _gen, float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		
		position.set(_x, _y, _z);
		gen = _gen;
		radius = ofRandom(10, 50 - (gen - 1) * 10) * 0.2 * 2;
		age = 0;
		lifeSpan = (float)radius;
		bgColor = ofVec3f(ofRandom(0, 1), ofRandom(0, 1), ofRandom(0, 1));
	}
	
	~Particle() {
		
	}
	
	void update() {
		
		velocity += force;
		
		// collide with ground
//		if (position.y < 0) {
//			position.y = 0;
//			velocity.y *= -1;
//			velocity *= 0.6;
//			aging(2.5);
//			position += velocity;
//		} else {
//			velocity *= 0.9;
//			aging(0.2);
//			position += velocity;
//		}
		
		velocity *= 0.9;
		position += velocity;
		
		force.set(0, 0, 0);
	}
	
	void draw() {
		float size = 1.f;
		
		size = radius * remainingLifePer() * 1.2;
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		utils::billboard();
		//glColor4f(remainingLifePer() - 0.5f, remainingLifePer() * 0.25f, 1.5f - remainingLifePer(), 1);
			glColor4f(bgColor.x, bgColor.y, bgColor.z, (1 - remainingLifePer()) * .3f );
		glScalef(size, size, size);
		glCallList(3);
		glPopMatrix();
		
		size = radius * remainingLifePer();
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		utils::billboard();
		glColor4f(remainingLifePer() - 0.5f, remainingLifePer() * 0.25f, 1.5f - remainingLifePer(), .2);
		//glColor4f(remainingLifePer() - 0.5f, remainingLifePer() * 0.25f, 1.5f - remainingLifePer(), 1);
		//glColor4f(remainingLifePer() + 0.5f, remainingLifePer(), 1.5f - remainingLifePer(), 1);
		glScalef(size, size, size);
		glCallList(2);
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
