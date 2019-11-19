#include <iostream>
#include "jogador.h"
#include <math.h>
#include <GL/glut.h>

using namespace std;

#define grausParaRadianos(g) g*(M_PI/180)

Jogador::Jogador(){
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
}

Jogador::Jogador(float x, float y, float z, float r, float velocidade){
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
}

void Jogador::desenhar(){
    glPushMatrix();
        glColor3f(0.0f, 0.0f, 1.0f);
		glTranslatef(x, y, z);
        glRotatef(90, 1, 0, 0);
		glutSolidTeapot(r);
	glPopMatrix();
}

// void Jogador::alterarAngulo(float coeficiente_ajuste){
//     angulo += velocidade * coeficiente_ajuste;
//     angulo_canhao_arena += velocidade * coeficiente_ajuste;
// }

// void Jogador::alterarAnguloCanhao(float incremento){
//     if(angulo_canhao + incremento <= 45 && angulo_canhao + incremento >= -45){
//         angulo_canhao += incremento;
//         angulo_canhao_arena += incremento;
//     }
// }

// void Jogador::alterarEscala(float incremento){
//     escala += incremento;
//     r += incremento;
// }

// void Jogador::andar(float coeficiente_ajuste){
//     y += sin(grausParaRadianos(angulo)) * velocidade * coeficiente_ajuste;
//     x += cos(grausParaRadianos(angulo)) * velocidade * coeficiente_ajuste;
// }

// void Jogador::girarHelices(float coeficiente_ajuste){
//     angulo_helices += velocidade * coeficiente_ajuste * 2;
// }

Jogador::~Jogador(){
}
