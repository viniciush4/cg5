#include <iostream>
#include "inimigo.h"
#include <math.h>
#include <GL/glut.h>

using namespace std;

#define grausParaRadianos(g) g*(M_PI/180)

Inimigo::Inimigo(){
	this->x = 0;
	this->y = 0;
    this->z = 0;
	this->r = 0;
    this->angulo_xy = rand() % 360;
    this->angulo_xz = rand() % 360;
    this->angulo_canhao_xy = 0;
    this->angulo_canhao_xz = 0;
    this->angulo_canhao_arena_xy = 0;
    this->angulo_canhao_arena_xz = 0;
    this->angulo_helices = 0;
    this->velocidade = 0;
    this->somatorio_incremento_angulo = 0;
    this->incrementar_angulo = true;
    this->tempo_desde_ultimo_tiro = 0;
}

Inimigo::Inimigo(float x, float y, float z, float r, float velocidade){
	this->x = x;
	this->y = y;
    this->z = z;
	this->r = r;
    this->angulo_xy = rand() % 360;
    this->angulo_xz = rand() % 360;
    this->angulo_canhao_xy = 0;
    this->angulo_canhao_xz = 0;
    this->angulo_canhao_arena_xy = 0;
    this->angulo_canhao_arena_xz = 0;
    this->angulo_helices = 0;
    this->velocidade = velocidade;
    this->somatorio_incremento_angulo = 0;
    this->incrementar_angulo = true;
    this->tempo_desde_ultimo_tiro = 0;
}

void Inimigo::desenhar()
{
    glPushMatrix();
        glTranslatef(x, y, z+r);

        glColor3f(1, 0, 0);
	//	glNormal3f(0.0, 0.0, 1.0);

        glutSolidSphere(r,100,100);
    glPopMatrix();

}

void Inimigo::alterarAngulo(float incremento_xy, float incremento_xz){
    angulo_xy += incremento_xy;
    angulo_xz += incremento_xz;
    angulo_canhao_arena_xy += incremento_xy;
    angulo_canhao_arena_xz += incremento_xz;
}

void Inimigo::andar(float coeficiente_ajuste){
    // y += sin(grausParaRadianos(angulo)) * velocidade * coeficiente_ajuste;
    // x += cos(grausParaRadianos(angulo)) * velocidade * coeficiente_ajuste;
}

void Inimigo::girarHelices(float coeficiente_ajuste){
    // angulo_helices += velocidade * coeficiente_ajuste * 2;
}

Inimigo::~Inimigo(){
}
