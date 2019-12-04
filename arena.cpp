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
	// this->textura_ceu = LoadTextureRAW("Texturas/ceu.bmp");
	// this->textura_chao = LoadTextureRAW("Texturas/areia.bmp");
}

void Arena::desenhar(){
	
	// Desenha um cilindro
	glPushMatrix();			
		glPushAttrib(GL_TEXTURE_BIT);
			glColor3f(1.0f, 1.0f, 1.0f);
			GLUquadric* q = gluNewQuadric();
			gluQuadricOrientation(q, GLU_INSIDE);
			gluQuadricDrawStyle(q, GLU_FILL); //GLU_FILL, GLU_LINE, GLU_SILHOUETTE, GLU_POINT
			gluQuadricNormals(q, GLU_FLAT); //GLU_NONE, GLU_FLAT, GLU_SMOOTH
			gluQuadricTexture(q, GL_TRUE);// if you want to map a texture to it.	
			glBindTexture(GL_TEXTURE_2D, this->textura_ceu);
			gluCylinder(q, r, r, altura, 30, 30);
		glPopAttrib();
	glPopMatrix();
	

	
	// Desenha o chão
	glPushMatrix();
		glPushAttrib(GL_TEXTURE_BIT);

			glColor3f(1.0f, 1.0f, 1.0f);
			glNormal3f(0.0, 0.0, 1.0);

			glBindTexture(GL_TEXTURE_2D, this->textura_chao);
			glBegin(GL_QUADS);
				
				
				glTexCoord2f(0.0, 0.0); glVertex3f(-r, -r, 0.0);
				glTexCoord2f(0.0, 1.0); glVertex3f(r, -r, 0.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(r, r, 0.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(-r, r, 0.0);
			glEnd();
		glPopAttrib();
	glPopMatrix();
	
	
	//Desenha o chão redondo
	// glPushMatrix();
	// 	glPushAttrib(GL_TEXTURE_BIT);	
	// 		glColor3f(1.0f, 1.0f, 1.0f);
	// 		glNormal3f(0.0, 0.0, 1.0);
	// 		gluQuadricTexture(q, GL_TRUE);
	// 		glBindTexture(GL_TEXTURE_2D, this->textura_chao);
	// 		//Desenha o círculo		
	// 		glBegin(GL_POLYGON);
	// 			float angulo = 0.0;
	// 			int numVertices = 1000;
	// 			for(int i=0; i < numVertices; i++) {
	// 				glVertex3f(r * cos(angulo), r * sin(angulo), 0.0);
	// 				angulo += 2 * M_PI / numVertices;
	// 			}
	// 		glEnd();
	// 	glPopAttrib();
	// glPopMatrix();
}

Arena::~Arena(){
}
