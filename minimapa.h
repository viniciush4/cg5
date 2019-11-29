#ifndef MINIMAPA_H_
#define MINIMAPA_H_

#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <math.h>
#include "arena.h"
#include "inimigo.h"
#include "base.h"
#include "jogador.h"

class Minimapa
{
	public:

        Minimapa();
        void desenhar(Arena &arena, Jogador &jogador, vector<Inimigo> &inimigos, vector<Base> &bases, GLfloat larguraJanela, GLfloat alturaJanela);

};

#endif /* MINIMAPA_H_ */