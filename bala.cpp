#include "bala.h"



Bala::Bala()
{

}

Bala::Bala(float x, float y, float z, float r, float angulo_xy, float angulo_xz, float angulo_canhao_xy, float angulo_canhao_xz, float angulo_canhao_arena_xy, float angulo_canhao_arena_xz, float angulo_inclinacao, bool curvando, float velocidade)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->r = r;
    this->angulo_xy = angulo_xy;
    this->angulo_xz = angulo_xz;
    this->angulo_canhao_xy = angulo_canhao_xy;
    this->angulo_canhao_xz = angulo_canhao_xz;
    this->angulo_canhao_arena_xy = angulo_canhao_arena_xy;
    this->angulo_canhao_arena_xz = angulo_canhao_arena_xz;
    this->angulo_inclinacao = angulo_inclinacao;
    this->curvando = curvando;
    this->velocidade = velocidade;

//Tentativa 1
//    this->x += this->r*cos(grausParaRadianos(angulo_xy))+r/14*cos(grausParaRadianos(angulo_canhao_xy*4));
 //   this->y += this->r*sin(grausParaRadianos(angulo_xy))+r/14*sin(grausParaRadianos(angulo_canhao_xy*4));
 //   this->z += r/14*sin(grausParaRadianos(angulo_canhao_arena_xz)) + 8.56;

//Tentativa 2
/*
    double dx1 = ((r/4.7)*4*cos(grausParaRadianos(angulo_xz))+8.56*sin(grausParaRadianos(angulo_xz)));
    double dz1 = ((r/4.7)*4*sin(grausParaRadianos(angulo_xz))+8.56*cos(grausParaRadianos(angulo_xz)));

    this->x += dx1;
    this->z += dz1;

    this->y += dx1*sin(grausParaRadianos(angulo_xy));
    this->x += dx1*cos(grausParaRadianos(angulo_xy));
*/
//Tentativa 3

 //   this->x += this->r*cos(grausParaRadianos(angulo_xy))+r/14*cos(grausParaRadianos(angulo_canhao_xy*4));
 //   this->y += this->r*sin(grausParaRadianos(angulo_xy))+r/14*sin(grausParaRadianos(angulo_canhao_xy*4));

}

void Bala::desenhar()
{
    GLfloat mat_ambient_k[] = { 0.0, 0.0, 0.0, 1.0 };
	glColor3fv(mat_ambient_k);

    glPushMatrix();		

		//Posiciona o avião de acordo com o ângulo da pista
		glTranslatef(x, y, z);	

		// glutSolidSphere(r,100,100);

		glScalef(4,4,4);
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		glTranslatef(0, 2, 0);
		
		//Movimento do avião
		glRotatef(angulo_xy, 0, 1, 0);
		glRotatef(angulo_xz, -1, 0, 0);
		glRotatef(angulo_inclinacao, 0, 0, -1);

		if(!curvando)
		{
    		glRotatef(angulo_inclinacao, 0, 0, 1);
		}
	
		//Desenha o canhão
		glPushMatrix();
			
			//Desenha na ponta do avião
			glTranslatef(0, 0.14, r/4.7);
			
			//Movimentos do canhão
			glRotatef(angulo_canhao_xy, 0, 1, 0);
			glRotatef(angulo_canhao_xz, -1, 0, 0);

			glPushMatrix();
				glTranslated(0,0,r/14);
				
				glutSolidSphere(r/100,100,100);
			glPopMatrix();			
		
		glPopMatrix();

	glPopMatrix();

    glPushMatrix();
       
        
        
		
    glPopMatrix();
}

void Bala::mover(float coeficiente_ajuste)
{
//  bala.moverBalaEixoX(velocidadeFinalTiro * (tempo/1000) * cos(bala.getAnguloAviao() + bala.getAnguloCanhao() * M_PI / 180.0));
//	bala.moverBalaEixoY(velocidadeFinalTiro * (tempo/1000) * sin(bala.getAnguloAviao() + bala.getAnguloCanhao() * M_PI / 180.0));

   
 //   x += velocidade*coeficiente_ajuste*cos(grausParaRadianos(angulo_canhao_arena_xy));
 //   y += velocidade*coeficiente_ajuste*sin(grausParaRadianos(angulo_canhao_arena_xy));
 //   z += velocidade*coeficiente_ajuste*sin(grausParaRadianos(angulo_canhao_arena_xz));

    y += sin(grausParaRadianos(angulo_canhao_arena_xy)) * velocidade * coeficiente_ajuste;
    x += cos(grausParaRadianos(angulo_canhao_arena_xy)) * velocidade * coeficiente_ajuste;
    z += sin(grausParaRadianos(angulo_canhao_arena_xz)) * velocidade * coeficiente_ajuste;


}

Bala::~Bala(){
}