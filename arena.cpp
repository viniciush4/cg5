#include "arena.h"
#include <GL/glut.h>

using namespace std;

Arena::Arena(){
	this->x = 0;
	this->y = 0;
	this->r = 0;
}

Arena::Arena(float x, float y, float r){
	this->x = x;
	this->y = y;
	this->r = r;
}

void Arena::desenhar(){
	glPushMatrix();
		glColor3f(1, 1, 0);
		glBegin(GL_POLYGON);
			glTranslatef(0, 0, 0);
			glVertex3f(-r, -r, 0);
			glVertex3f(-r,  r, 0);
			glVertex3f(r,  r, 0);
			glVertex3f(r, -r, 0);
		glEnd();
	glPopMatrix();
}

Arena::~Arena(){
}
