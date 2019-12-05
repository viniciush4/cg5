#ifndef ARENA_H_
#define ARENA_H_

#include <GL/glut.h>
#include <math.h>
#include "imageloader.h"

using namespace std;

class Arena {
public:
	float x;
	float y;
	float r;
	float altura;
	GLuint textura_ceu;
	GLuint textura_chao;

	Arena();
	Arena(float x, float y, float r, float altura);
	void desenhar(GLuint texturaCeu, GLuint texturaChao);
	
	virtual ~Arena();
};

#endif /* ARENA_H_ */