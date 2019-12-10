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

void Arena::desenhar(GLuint texturaCeu, GLuint texturaChao, bool conferir_minimapa)
{

	if(!conferir_minimapa){
	//Desenha o globo do céu
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturaCeu);

		glPushMatrix();			
			glPushAttrib(GL_TEXTURE_BIT);
				GLfloat mat_ambient_w1[] = { 1.0, 1.0, 1.0, 1.0 };
				glColor3fv(mat_ambient_w1);
				GLUquadric* c = gluNewQuadric();
				gluQuadricOrientation(c, GLU_INSIDE);
				gluQuadricDrawStyle(c, GLU_FILL); //GLU_FILL, GLU_LINE, GLU_SILHOUETTE, GLU_POINT
				gluQuadricNormals(c, GLU_SMOOTH); //GLU_NONE, GLU_FLAT, GLU_SMOOTH
				gluQuadricTexture(c, GL_TRUE);// if you want to map a texture to it.	
				//			glBindTexture(GL_TEXTURE_2D, this->textura_ceu);
	//	 		gluCylinder(c, r+78, r+78, 20, 30, 30);
				gluSphere(c,2*r,1000,1000);

			glPopAttrib();
		glPopMatrix();

	
	glDisable(GL_TEXTURE_2D);
/*

	//Desenha o cilindro do céu
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
			gluCylinder(q, r+80, r+80, altura, 30, 30);
		glPopAttrib();
	glPopMatrix();
	

	glDisable(GL_TEXTURE_2D);
*/

	//Desenha o chão
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturaChao);
		
		glPushMatrix();
			glPushAttrib(GL_TEXTURE_BIT);
			
				glColor3fv(mat_ambient_w1);
				glNormal3f(0.0, 0.0, 1.0);

				glBegin(GL_QUADS);			
					glTexCoord2f(0.0, 0.0); glVertex3f(-2*r, -2*r, 0.0);
					glTexCoord2f(0.0, 1.0); glVertex3f(2*r, -2*r, 0.0);
					glTexCoord2f(1.0, 1.0); glVertex3f(2*r, 2*r, 0.0);
					glTexCoord2f(1.0, 0.0); glVertex3f(-2*r, 2*r, 0.0);
				glEnd();
			glPopAttrib();
		glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	
	} else { // se não for pra conferir o minimapa
	
// 	//Desenha o chão redondo
	glPushMatrix();	
		float angulo = 0.0;
		int numVertices = 1000;

//		glColor3f(1.0f, 0.0f, 0.5f);

		GLfloat mat_ambient_w1[] = { 1.0, 1.0, 1.0, 1.0 };
		glColor3fv(mat_ambient_w1);
		glNormal3f(0.0, 0.0, 1.0);

		//Desenha o círculo		
		glBegin(GL_POLYGON);
			for(int i=0; i < numVertices; i++)
			{
				glTexCoord2f((r+80) * cos(angulo), (r+80) * sin(angulo));
				glVertex3f((r+80) * cos(angulo), (r+80) * sin(angulo), 0.0);
				angulo += 2 * M_PI / numVertices;
			}
		glEnd();
	glPopMatrix();
	}
	
}

Arena::~Arena(){
}
