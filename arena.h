#ifndef ARENA_H_
#define ARENA_H_

#include <GL/glut.h>
#include <math.h>

using namespace std;

class Arena {
public:
	float x;
	float y;
	float r;
	float altura;

	Arena();
	Arena(float x, float y, float r, float altura);
	void desenhar(GLuint textura);
	
	virtual ~Arena();
};

#endif /* ARENA_H_ */