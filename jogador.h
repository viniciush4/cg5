#ifndef JOGADOR_H_
#define JOGADOR_H_

#include <iostream>
#include <GL/glut.h>
#include <math.h>

#include "lerOBJ.h"


#define grausParaRadianos(g) g*(M_PI/180)

using namespace std;


class Jogador {
public:
	float x;
	float y;
	float z;
	float r;
	float angulo_xy;
	float angulo_xz;
	float angulo_inclinacao;
	float angulo_canhao_xy;
	float angulo_canhao_xz;
	float angulo_canhao_arena_xy;
	float angulo_canhao_arena_xz;
	float angulo_helices;
	float velocidade;
	bool curvando;

	Jogador();
	Jogador(float x, float y, float z, float r, float velocidade);
	void desenhar();
	void alterarAnguloXY(float coeficiente_ajuste);
	void alterarAnguloXZ(float coeficiente_ajuste);
	void alterarAnguloInclinacao(float coeficiente_ajuste);
	void alterarAnguloCanhaoXY(float incremento);
	void alterarAnguloCanhaoXZ(float incremento);
	void andar(float coeficiente_ajuste, float alturaArena);
	void girarHelices(float coeficiente_ajuste);




	void desenharFuselagem(float raio);
	void desenharCabine(float raio);
	void desenharCauda();
	void desenharAsaDireita();
	void desenharAsaEsquerda();
	
	void desenharPiramide();
	void desenharHeliceDireita();
	void desenharHeliceEsquerda();

	void desenharCanhao();
	void desenharModeloAviao(LerOBJ &aviaoJogador, struct obj_model_t &objfile, LerOBJ &helice, struct obj_model_t &modeloHelice, GLuint texturaJogador);











	
	virtual ~Jogador();
};

#endif /* JOGADOR_H_ */