#include "imageloader.h"

class Pista {
public:
	float x1;
	float y1;
	float x2;
	float y2;
	float largura;
	float comprimento;
	float angulo_xy;
	GLuint textura;

	Pista();
	Pista(float x1, float y1, float x2, float y2);
	void desenhar();
	virtual ~Pista();
};
