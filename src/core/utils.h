//
//  utils.h
//  emptyExample
//
//  Created by Okami Satoshi on 12/04/25.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef emptyExample_utils_h
#define emptyExample_utils_h

#include "ofMain.h"

class utils {
	
	
public:
	
	//--------------------------------------------------------------
	static void billboard()
	{
		// モデルビュー変換行列の操作用
		GLdouble m[17], l;
		
		// 現在のモデルビュー変換行列を取り出す
		glGetDoublev(GL_MODELVIEW_MATRIX, m);
		
		// Z 軸
		m[8] = -m[12];
		m[9] = -m[13];
		m[10] = -m[14];
		l = sqrt(m[8] * m[8] + m[9] * m[9] + m[10] * m[10]);
		m[8] /= l;
		m[9] /= l;
		m[10] /= l;
		
		// X 軸 = (0, 1, 0) × Z 軸
		m[0] = -m[14];
		m[1] = 0.0;
		m[2] = m[12];
		l = sqrt(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
		m[0] /= l;
		m[1] /= l;
		m[2] /= l;
		
		// Y 軸 = Z 軸 × X 軸
		m[4] = m[9] * m[2] - m[10] * m[1];
		m[5] = m[10] * m[0] - m[8] * m[2];
		m[6] = m[8] * m[1] - m[9] * m[0];
		
		// 書き換えた行列を書き戻す
		glLoadMatrixd(m);
	}
	
	//--------------------------------------------------------------
	static ofVec3f getRand()
	{
		ofVec3f v;
		float a1 = (float)(ofRandom(0, 360) * PI / 180);
		float a2 = (float)(ofRandomuf() * 2 * PI);
		
		v.x = (float)(sin(a1) * cos(a2));
		v.y = (float)(sin(a1) * sin(a2));
		v.z = (float)(cos(a1));
		
		return v;
	}
};


#endif
