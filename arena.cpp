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

void Arena::desenhar(GLuint texturaCeu, GLuint texturaChao, GLuint texturaArvore)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturaArvore);

	glPushMatrix();			
		glPushAttrib(GL_TEXTURE_BIT);
			GLfloat mat_ambient_w1[] = { 1.0, 1.0, 1.0, 1.0 };
			glColor3fv(mat_ambient_w1);
			GLUquadric* c = gluNewQuadric();
			gluQuadricOrientation(c, GLU_INSIDE);
			gluQuadricDrawStyle(c, GLU_FILL); //GLU_FILL, GLU_LINE, GLU_SILHOUETTE, GLU_POINT
			gluQuadricNormals(c, GLU_FLAT); //GLU_NONE, GLU_FLAT, GLU_SMOOTH
			gluQuadricTexture(c, GL_TRUE);// if you want to map a texture to it.	
			//			glBindTexture(GL_TEXTURE_2D, this->textura_ceu);
			gluCylinder(c, r+48, r+48, 20, 30, 30);
		glPopAttrib();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturaCeu);

	// Desenha um cilindro
	glPushMatrix();			
		glPushAttrib(GL_TEXTURE_BIT);
			GLfloat mat_ambient_w[] = { 1.0, 1.0, 1.0, 1.0 };
			glColor3fv(mat_ambient_w);
			GLUquadric* q = gluNewQuadric();
			gluQuadricOrientation(q, GLU_INSIDE);
			gluQuadricDrawStyle(q, GLU_FILL); //GLU_FILL, GLU_LINE, GLU_SILHOUETTE, GLU_POINT
			gluQuadricNormals(q, GLU_FLAT); //GLU_NONE, GLU_FLAT, GLU_SMOOTH
			gluQuadricTexture(q, GL_TRUE);// if you want to map a texture to it.	
			//			glBindTexture(GL_TEXTURE_2D, this->textura_ceu);
			gluCylinder(q, r+50, r+50, altura, 30, 30);
		glPopAttrib();
	glPopMatrix();
	

	glDisable(GL_TEXTURE_2D);


	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, texturaChao);

	// Desenha o chão
	glPushMatrix();
		glPushAttrib(GL_TEXTURE_BIT);

			
			glColor3fv(mat_ambient_w);
			glNormal3f(0.0, 0.0, 1.0);

//			glBindTexture(GL_TEXTURE_2D, this->textura_chao);
			glBegin(GL_QUADS);
				
				
				glTexCoord2f(0.0, 0.0); glVertex3f(-r-50, -r-50, 0.0);
				glTexCoord2f(0.0, 1.0); glVertex3f(r+50, -r-50, 0.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(r+50, r+50, 0.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(-r-50, r+50, 0.0);
			glEnd();
		glPopAttrib();
	glPopMatrix();

	
	glDisable(GL_TEXTURE_2D);
	
	
// 	//Desenha o chão redondo
// 	glPushMatrix();	
// 		float angulo = 0.0;
// 		int numVertices = 1000;

// //		glColor3f(1.0f, 0.0f, 0.5f);

// 		glColor3fv(mat_ambient_w);
// 		glNormal3f(0.0, 0.0, 1.0);

// 		//Desenha o círculo		
// 		glBegin(GL_POLYGON);
// 			for(int i=0; i < numVertices; i++)
// 			{
// 				glTexCoord2f(r * cos(angulo), r * sin(angulo));
// 				glVertex3f(r * cos(angulo), r * sin(angulo), 0.0);
// 				angulo += 2 * M_PI / numVertices;
// 			}
// 		glEnd();
// 	glPopMatrix();

	
}

Arena::~Arena(){
}
