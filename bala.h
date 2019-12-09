#ifndef BALA_H_
#define BALA_H_

#include <math.h>
#include <GL/glut.h>
#include <iostream>

#define grausParaRadianos(g) g*(M_PI/180)
#define radianosParaGraus(r) r*(180/M_PI)

using namespace std;

class Bala
{
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
        float angulo_inclinacao;
        bool curvando;
        float velocidade;



        Bala();
        Bala(float x, float y, float z, float r, float angulo_xy, float angulo_xz, float angulo_canhao_xy, float angulo_canhao_xz, float angulo_canhao_arena_xy, float angulo_canhao_arena_xz, float angulo_inclinacao, bool curvando, float velocidade);
        void desenhar();
        void mover(float coeficiente_ajuste);

        virtual ~Bala();
};

#endif /* BALA_H_ */