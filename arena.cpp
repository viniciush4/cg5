#include "arena.h"

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

void Arena::desenhar(GLuint textura){
	
	// Desenha um cilindro
	glPushMatrix();
		
		GLUquadric* q = gluNewQuadric();
		gluQuadricOrientation(q, GLU_INSIDE);
		gluQuadricDrawStyle(q, GLU_FILL); //GLU_FILL, GLU_LINE, GLU_SILHOUETTE, GLU_POINT
		gluQuadricNormals(q, GLU_FLAT); //GLU_NONE, GLU_FLAT, GLU_SMOOTH
		// gluQuadricTexture(q, GL_TRUE);// if you want to map a texture to it.	
		glBindTexture(GL_TEXTURE_2D, textura);
		gluCylinder(q, r, r, altura, 30, 30);
	glPopMatrix();
	

/*
	// Desenha o chão
	glPushMatrix();
		glBegin(GL_QUADS);
			glColor3f(1.0f, 0.0f, 0.5f);
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3f(-r, -r, 0.0);
			glVertex3f(r, -r, 0.0);
			glVertex3f(r, r, 0.0);
			glVertex3f(-r, r, 0.0);
		glEnd();
	glPopMatrix();
*/
	
	//Desenha o chão redondo
	glPushMatrix();	
		float angulo = 0.0;
		int numVertices = 1000;

		glColor3f(1.0f, 0.0f, 0.5f);
		glNormal3f(0.0, 0.0, 1.0);

		//Desenha o círculo		
		glBegin(GL_POLYGON);
			for(int i=0; i < numVertices; i++)
			{
				glVertex3f(r * cos(angulo), r * sin(angulo), 0.0);
				angulo += 2 * M_PI / numVertices;
			}
		glEnd();
	glPopMatrix();
}

Arena::~Arena(){
}
