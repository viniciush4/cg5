#ifndef INIMIGO_H_
#define INIMIGO_H_

#include <iostream>
#include <math.h>
#include <GL/glut.h>

#include "lerOBJ.h"

using namespace std;

#define grausParaRadianos(g) g*(M_PI/180)

class Inimigo {
public:
	float x;
	float y;
	float z;
	float r;
	float angulo_xy;
	float angulo_xz;
	float angulo_canhao_xy;
	float angulo_canhao_xz;
	float angulo_canhao_arena_xy;
	float angulo_canhao_arena_xz;
	float angulo_helices;
	float velocidade;
	float somatorio_incremento_angulo;
	bool incrementar_angulo_xy;
	bool incrementar_altura_z;
	float tempo_desde_ultimo_tiro;

	Inimigo();
	Inimigo(float x, float y, float z, float r, float velocidade);
	void desenhar();
	void alterarAngulo(float incremento_xy, float incremento_xz);
	void andar(float coeficiente_ajuste);
	void girarHelices(float coeficiente_ajuste);

	void desenharModeloAviao(LerOBJ &aviaoJogador, struct obj_model_t &modeloAviaoJogador, LerOBJ &helice, struct obj_model_t &modeloHelice);
	
	virtual ~Inimigo();
};

#endif /* INIMIGO_H_ */