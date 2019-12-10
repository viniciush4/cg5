#include "pista.h"


Pista::Pista(){
	this->x1 = 0;
	this->y1 = 0;
    this->x2 = 0;
	this->y2 = 0;
	this->largura = 0;
	this->comprimento = 0;
	this->angulo_xy = 0;
}

Pista::Pista(float x1, float y1, float x2, float y2){
	this->x1 = x1;
	this->y1 = y1;
    this->x2 = x2;
	this->y2 = y2;
	this->largura = 0;
	this->comprimento = sqrt(pow(y2-y1,2) + pow(x2-x1,2));
	this->angulo_xy = -radianosParaGraus(atan2((y2-y1), (x2-x1)));

}

void Pista::desenhar(GLuint texturaPista){
    // glColor3f(0, 0, 0);
	// glBegin(GL_LINES);
    //     glVertex3f(x1, y1, 2.0);
    //     glVertex3f(x2, y2, 2.0);
    // glEnd();

	// Desenha um plano que representa a pista
	// glPushMatrix();
	// 	glPushAttrib(GL_TEXTURE_BIT);
	// 		glColor3f(1, 1, 1);
	// 		glNormal3f(0.0, 0.0, 1.0);

	// 		glTranslatef(x1, y1, 1);
	// 		glRotatef(angulo_xy, 0, 0, 1);

	// 		glBindTexture(GL_TEXTURE_2D, this->textura);
	// 		glBegin(GL_QUADS);
	// 			glTexCoord2f(0.0, 0.0); glVertex3f(0, -largura/2, 0.5);
	// 			glTexCoord2f(0.0, 1.0); glVertex3f(comprimento, -largura/2, 0.5);
	// 			glTexCoord2f(1.0, 1.0); glVertex3f(comprimento, largura/2, 0.5);
	// 			glTexCoord2f(1.0, 0.0); glVertex3f(0, largura/2, 0.5);
	// 		glEnd();
	// 	glPopAttrib();
	// glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturaPista);

	glPushMatrix();
		// glPushAttrib(GL_TEXTURE_BIT);

			glTranslatef(x1, y1, 1);
			glRotatef(angulo_xy, 0, 0, 1);

			// glColor3f(1.0f, 1.0f, 1.0f);
			// glNormal3f(0.0, 0.0, 1.0);

//			glBindTexture(GL_TEXTURE_2D, this->textura);
			glBegin(GL_QUADS);
				
				
				glTexCoord2f(0.0, 0.0); glVertex3f(0, -largura/2, 0);
				glTexCoord2f(0.0, 1.0); glVertex3f(comprimento, -largura/2, 0);
				glTexCoord2f(1.0, 1.0); glVertex3f(comprimento, largura/2, 0);
				glTexCoord2f(1.0, 0.0); glVertex3f(0, largura/2, 0);
			glEnd();
		// glPopAttrib();
	glPopMatrix();


	glDisable(GL_TEXTURE_2D);
}

Pista::~Pista(){
}
