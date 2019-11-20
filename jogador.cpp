#include "jogador.h"


Jogador::Jogador(){
	this->x = 0;
	this->y = 0;
    this->z = 0;
	this->r = 0;
    this->angulo_xy = 0;
    this->angulo_xz = 0;
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
    this->angulo_xy = 0;
    this->angulo_xz = 0;
    this->angulo_canhao_xy = 0;
    this->angulo_canhao_xz = 0;
    this->angulo_canhao_arena_xy = 0;
    this->angulo_canhao_arena_xz = 0;
    this->angulo_helices = 0;
    this->velocidade = velocidade;
}

void Jogador::desenhar() {
    glPushMatrix();
        glColor3f(0.0f, 0.0f, 1.0f);
		glTranslatef(x, y, z+r);
        glRotatef(90, 1, 0, 0);
        glRotatef(angulo_xy, 0, 1, 0);
        glRotatef(angulo_xz, 0, 0, 1);
		glutSolidTeapot(r);
	glPopMatrix();
}

void Jogador::alterarAnguloXY(float coeficiente_ajuste){
    angulo_xy += velocidade * coeficiente_ajuste;
    angulo_canhao_arena_xy += velocidade * coeficiente_ajuste;
}

void Jogador::alterarAnguloXZ(float coeficiente_ajuste){
    angulo_xz += velocidade * coeficiente_ajuste;
    angulo_canhao_arena_xz += velocidade * coeficiente_ajuste;
}

void Jogador::alterarAnguloCanhaoXY(float incremento){
    if(angulo_canhao_xy + incremento <= 30 && angulo_canhao_xy + incremento >= -30){
        angulo_canhao_xy += incremento;
        angulo_canhao_arena_xy += incremento;
    }
}

void Jogador::alterarAnguloCanhaoXZ(float incremento){
    if(angulo_canhao_xz + incremento <= 30 && angulo_canhao_xz + incremento >= -30){
        angulo_canhao_xz += incremento;
        angulo_canhao_arena_xz += incremento;
    }
}

void Jogador::andar(float coeficiente_ajuste){
    // Caminha em xy
    y += sin(grausParaRadianos(angulo_xy)) * velocidade * coeficiente_ajuste;
    x += cos(grausParaRadianos(angulo_xy)) * velocidade * coeficiente_ajuste;
    // Caminha em xz
    z += sin(grausParaRadianos(angulo_xz)) * velocidade * coeficiente_ajuste;
    // x += cos(grausParaRadianos(angulo_xz)) * velocidade * coeficiente_ajuste;
}

void Jogador::girarHelices(float coeficiente_ajuste){
    angulo_helices += velocidade * coeficiente_ajuste * 2;
}

Jogador::~Jogador(){
}
