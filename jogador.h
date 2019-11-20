#include <iostream>
#include <GL/glut.h>
#include <math.h>


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
	float angulo_canhao_xy;
	float angulo_canhao_xz;
	float angulo_canhao_arena_xy;
	float angulo_canhao_arena_xz;
	float angulo_helices;
	float velocidade;

	Jogador();
	Jogador(float x, float y, float z, float r, float velocidade);
	void desenhar();
	// void alterarAngulo(float coeficiente_ajuste);
	// void alterarAnguloCanhao(float incremento);
	// void alterarEscala(float incremento);
	// void andar(float coeficiente_ajuste);
	// void girarHelices(float coeficiente_ajuste);
	
	virtual ~Jogador();
};
