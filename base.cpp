#include <iostream>
#include "base.h"
#include <GL/glut.h>

using namespace std;

#define grausParaRadianos(g) g*(M_PI/180)

Base::Base(){
	this->x = 0;
	this->y = 0;
	this->r = 0;
}

Base::Base(float x, float y, float r){
	this->x = x;
	this->y = y;
	this->r = r;
}

void Base::desenhar()
{
	glPushMatrix();
        glTranslatef(x, y, r);

        glColor3f(0, 1, 0);
	//	glNormal3f(0.0, 0.0, 1.0);

        glutSolidSphere(r,100,100);
    glPopMatrix();

}

Base::~Base(){
}
