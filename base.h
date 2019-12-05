#ifndef BASE_H_
#define BASE_H_

#include <iostream>
#include <GL/glut.h>
#include "lerOBJ.h"


#define grausParaRadianos(g) g*(M_PI/180)



class Base {
public:
	float x;
	float y;
	float r;

	Base();
	Base(float x, float y, float r);
	void desenharModeloBase(LerOBJ &base, struct obj_model_t &modeloBase, GLuint texturaBase);
	
	virtual ~Base();
};

#endif /* BASE_H_ */