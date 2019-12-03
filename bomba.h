#include <math.h>
#include <GL/glut.h>
#include <iostream>

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
	
	virtual ~Bomba();
};
