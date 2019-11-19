#include "arena.h"
#include <GL/glut.h>

using namespace std;

Arena::Arena(){
	this->x = 0;
	this->y = 0;
	this->r = 0;
	this->altura = 0;
}

Arena::Arena(float x, float y, float r, float altura){
	this->x = x;
	this->y = y;
	this->r = r;
	this->altura = altura;
}

void Arena::desenhar(){
	
	// Desenha um cilindro
	glPushMatrix();
		// glRotatef(-90,1,0,0);
		GLUquadric* q = gluNewQuadric();
		gluQuadricOrientation(q, GLU_INSIDE);
		gluCylinder(q, r, r, altura, 30, 30);
	glPopMatrix();

	// Desenha o chão
	glPushMatrix();
		glBegin(GL_QUADS);
			glColor3f(1.0f, 0.0f, 0.5f);
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(-r, -r, 0.0);
			glVertex3f(r, -r, 0.0);
			glVertex3f(r, r, 0.0);
			glVertex3f(-r, r, 0.0);
		glEnd();
	glPopMatrix();
}

Arena::~Arena(){
}
