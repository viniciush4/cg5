#include "base.h"


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

void Base::desenhar(LerOBJ &base, struct obj_model_t &modeloBase)
{
/*
	glPushMatrix();
        glTranslatef(x, y, r);

        glColor3f(0, 1, 0);
	//	glNormal3f(0.0, 0.0, 1.0);

        glutSolidSphere(r,100,100);
    glPopMatrix();
*/
	glPushMatrix();

        glTranslatef(x, y, r);
		glScalef(0.5, 0.5, 0.5);

        //glColor3f(0, 1, 0);
	//	glNormal3f(0.0, 0.0, 1.0);
		GLfloat mat_ambient_y[] = { 1.0, 1.0, 0.0, 1.0 };
		glColor3fv(mat_ambient_y);

		glRotatef(90, 1, 0, 0);
        base.RenderOBJModel(&modeloBase);
    glPopMatrix();

	

}

Base::~Base(){
}
