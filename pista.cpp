#include "pista.h"
#include <GL/glut.h>

using namespace std;

Pista::Pista(){
	this->x1 = 0;
	this->y1 = 0;
    this->x2 = 0;
	this->y2 = 0;
}

Pista::Pista(float x1, float y1, float x2, float y2){
	this->x1 = x1;
	this->y1 = y1;
    this->x2 = x2;
	this->y2 = y2;
}

void Pista::desenhar(){
    glColor3f(0, 0, 0);
	glBegin(GL_LINES);
        glVertex3f(x1, y1, 0.0);
        glVertex3f(x2, y2, 0.0);
    glEnd();
}

Pista::~Pista(){
}
