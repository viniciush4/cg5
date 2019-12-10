#ifndef PISTA_H_
#define PISTA_H_

#include <GL/glut.h>
#include <math.h>

using namespace std;

#define grausParaRadianos(g) g*(M_PI/180)
#define radianosParaGraus(r) r*(180/M_PI)

class Pista {
public:
	float x1;
	float y1;
	float x2;
	float y2;
	float largura;
	float comprimento;
	float angulo_xy;

	Pista();
	Pista(float x1, float y1, float x2, float y2);
	void desenhar(GLuint texturaPista);
	virtual ~Pista();
};

#endif /* PISTA_H_ */