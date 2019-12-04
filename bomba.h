#ifndef BOMBA_H_
#define BOMBA_H_

#include <math.h>
#include <GL/glut.h>
#include <iostream>

#include "lerOBJ.h"

class Bomba {
public:
	float x;
	float y;
	float z;
	float r;
	float angulo_xy;
	float angulo_xz;
	float velocidade;

	Bomba();
	Bomba(float x, float y, float z, float r, float angulo_xy, float angulo_xz, float velocidade);

	void desenhar();	
	void mover(float coeficiente_ajuste);
	void desenharModeloBomba(LerOBJ &bomba, struct obj_model_t &modeloBomba);
	
	virtual ~Bomba();
};

#endif /* BOMBA_H_ */