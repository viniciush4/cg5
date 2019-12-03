#include "bomba.h"

#define grausParaRadianos(g) g*(M_PI/180)
#define radianosParaGraus(r) r*(180/M_PI)

Bomba::Bomba(){
    this->x = 0;
    this->y = 0;
    this->angulo_xy = 0;
    this->angulo_xz = 0;
}

Bomba::Bomba(float x, float y, float z, float r, float angulo_xy, float angulo_xz, float velocidade){
    this->x = x;
    this->y = y;
    this->z = z;
    this->r = r;
    this->angulo_xy = angulo_xy;
    this->angulo_xz = angulo_xz;
    this->velocidade = velocidade;
}

void Bomba::desenhar(){
    glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(0.28, 0.28, 0.28);
        glColor3f(1, 1, 0);
        glutSolidSphere(r,100,100);
    glPopMatrix();

}

void Bomba::mover(float coeficiente_ajuste){
    y += velocidade*coeficiente_ajuste*sin(grausParaRadianos(angulo_xy));
    x += velocidade*coeficiente_ajuste*cos(grausParaRadianos(angulo_xy));
    z -= velocidade*coeficiente_ajuste;
}

Bomba::~Bomba(){
}
