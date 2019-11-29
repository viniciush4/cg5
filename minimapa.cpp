#include "minimapa.h"

Minimapa::Minimapa()
{

}

void Minimapa::desenhar(GLfloat larguraJanela, GLfloat alturaJanela)
{
     glPushAttrib(GL_ENABLE_BIT);
        
        float r= larguraJanela/8;
        
        glLineWidth(2.0);
        glTranslatef(larguraJanela - r , alturaJanela - (alturaJanela - r),0);

     

        float angulo = 0.0;
		int numVertices = 1000;

		glColor3f(1, 1, 0);
	//	glNormal3f(0.0, 0.0, 1.0);

		//Desenha o círculo		
		glBegin(GL_LINE_LOOP);
			for(int i=0; i < numVertices; i++)
			{
				glVertex3f(r * cos(angulo), r * sin(angulo), 0.0);
				angulo += 2 * M_PI / numVertices;
			}
		glEnd();
    glPopAttrib();
}