#include "jogador.h"


Jogador::Jogador(){
	this->x = 0;
	this->y = 0;
    this->z = 0;
	this->r = 0;
    this->angulo_xy = 0;
    this->angulo_xz = 0;
    this->angulo_canhao_xy = 0;
    this->angulo_canhao_xz = 0;
    this->angulo_canhao_arena_xy = 0;
    this->angulo_canhao_arena_xz = 0;
    this->angulo_helices = 0;
    this->velocidade = 0;
}

Jogador::Jogador(float x, float y, float z, float r, float velocidade){
	this->x = x;
	this->y = y;
    this->z = z;
	this->r = r;
    this->angulo_xy = 0;
    this->angulo_xz = 0;
    this->angulo_canhao_xy = 0;
    this->angulo_canhao_xz = 0;
    this->angulo_canhao_arena_xy = 0;
    this->angulo_canhao_arena_xz = 0;
    this->angulo_helices = 0;
    this->velocidade = velocidade;
}

void Jogador::desenhar() {
/*
    glPushMatrix();
        glColor3f(0.0f, 0.0f, 1.0f);
		glTranslatef(x, y, z+r);
        glRotatef(90, 1, 0, 0);
        glRotatef(angulo_xy, 0, 1, 0);
        glRotatef(angulo_xz, 0, 0, 1);
		glutSolidTeapot(r);
	glPopMatrix();
*/
    glPushMatrix();
		
		//Posiciona o avião de acordo com o ângulo da pista
		glTranslatef(x, y, z);
		glRotatef(-90, 0, 0, 1);
		glRotatef(angulo_xy, 0, 0, 1);
		
		
		//Desenha a fuselagem
		//posiçãoX = 0.0, posiçãoY = 0.0, raioX = raioPiloto/3, raioY = raioPiloto
		desenharFuselagem(r);

		//Desenha a cabine
		//posiçãoX = 0.0, posiçãoY = raioPiloto/2, raioX = raioPiloto/7, raioY = raioPiloto/3
		desenharCabine(r/3);

		//Desenha a cauda
		desenharCauda();
		
		//Desenha a asa direita
		desenharAsaDireita();
				
		//Desenha a asa esquerda
		desenharAsaEsquerda();
			
		//Desenha a hélice direita
		desenhaHeliceDireita();
		
		//Desenha a hélice esquerda
		desenhaHeliceEsquerda();
/*		
		//Desenha o canhão
		glPushMatrix();
			glTranslatef(0, getR(), 0);
			
			glRotatef(anguloCanhao, 0, 0, 1);
			desenharRetangulo(getR()/8, 2*getR()/5, 0.0, 0.0, 0.0);			
		glPopMatrix();	
		





*/


	glPopMatrix();	
    




}


void Jogador::desenharFuselagem(float raio)
{
	//GLfloat mat_ambient_r[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient_g[] = { 0.0, 1.0, 0.0, 1.0 };
    //GLfloat mat_ambient_b[] = { 0.0, 0.0, 1.0, 1.0 };


	glPushMatrix();
		glScalef(0.3, 1, 0.3);

		glColor3fv(mat_ambient_g);
		

		glutSolidSphere(raio,100,100);
	glPopMatrix();	
}

void Jogador::desenharCabine(float raio)
{
	GLfloat mat_ambient_k[] = { 0.0, 0.0, 0.0, 1.0 };
    //GLfloat mat_ambient_g[] = { 0.0, 1.0, 0.0, 1.0 };
    //GLfloat mat_ambient_b[] = { 0.0, 0.0, 1.0, 1.0 };


	glPushMatrix();
		

		glScalef(0.3, 1, 0.3);
		glTranslatef(0, r/2, r);

		glColor3fv(mat_ambient_k);
		

		glutSolidSphere(raio,100,100);
	glPopMatrix();	
}

void Jogador::desenharCauda()
{
	GLfloat mat_ambient_k[] = { 0.0, 0.0, 0.0, 1.0 };
	

	glPushMatrix();
		glScalef(0.1, 0.5, 0.5);
		glTranslatef(0, -2*r, r/2);

		glColor3fv(mat_ambient_k);
		glutSolidCube(r);
	glPopMatrix();


	glPushMatrix();
		glScalef(0.8, 0.5, 0.1);
		glTranslatef(0, -2*r, 0);

		glColor3fv(mat_ambient_k);
		glutSolidCube(r);
	glPopMatrix();

	
}
void Jogador::desenharAsaDireita()
{
	GLfloat mat_ambient_k[] = { 0.0, 0.0, 0.0, 1.0 };
	

	glPushMatrix();
		glTranslatef(r/3, 0, 0);	
//		desenharAsaDireita(2*getR()/3, getR()/2, 0.0, 0.0, 0.0);
		glRotatef(-10, 0, 0, 1);
		glPushMatrix();
			glScalef(1, 0.4, 0.1);

			glColor3fv(mat_ambient_k);
			glutSolidCube(r);
		glPopMatrix();
	glPopMatrix();
}
void Jogador::desenharAsaEsquerda()
{
	GLfloat mat_ambient_k[] = { 0.0, 0.0, 0.0, 1.0 };

	glPushMatrix();
		glTranslatef(-(r/3), 0, 0);	
		//desenharAsaEsquerda(2*getR()/3, getR()/2, 0.0, 0.0, 0.0);
		glRotatef(10, 0, 0, 1);
		glPushMatrix();
			glScalef(1, 0.4, 0.1);

			glColor3fv(mat_ambient_k);
			glutSolidCube(r);
		glPopMatrix();
	glPopMatrix();

}

void Jogador::desenhaHeliceDireita()
{
	//Desenha o suporte da hélice direita
	glPushMatrix();
		glTranslatef(2*(r/3), 0, 0);
		//desenharRetangulo(getR()/8, getR()/3, 0.0, 0.0, 0.0);
		glPushMatrix();
			GLfloat mat_ambient_k[] = { 0.0, 0.0, 0.0, 1.0 };

			glScalef(0.1, 0.5, 0.1);
			glColor3fv(mat_ambient_k);
			glutSolidCube(r);
		glPopMatrix();
		
	/*	
		//Desenha a hélice direita
		if(motorLigado)
		{
			glPushMatrix();
				glTranslatef(0, (getR()/3), 0);
				glRotatef(velocidadeHelice,0,1,0);		
				desenhaHelice(1.0, 1.0, 0.0);
				glRotatef(120,0,1,0);
				desenhaHelice(1.0, 1.0, 0.0);
				glRotatef(120,0,1,0);
				desenhaHelice(1.0, 1.0, 0.0);				
			glPopMatrix();
			
		}
		else
		{
			glPushMatrix();
				glTranslatef(0, (getR()/3), 0);
				desenhaHelice(1.0, 1.0, 0.0);
			glPopMatrix();
		}		

	*/	
	glPopMatrix();

}

void Jogador::desenhaHeliceEsquerda()
{
	//Desenha o suporte da hélice esquerda
	glPushMatrix();
		glTranslatef(-2*(r/3), 0, 0);
	//	desenharRetangulo(getR()/8, getR()/3, 0.0, 0.0, 0.0);
		glPushMatrix();
			GLfloat mat_ambient_k[] = { 0.0, 0.0, 0.0, 1.0 };

			glScalef(0.1, 0.5, 0.1);
			glColor3fv(mat_ambient_k);
			glutSolidCube(r);
		glPopMatrix();
/*	
		//Desenha a hélice esquerda
		if(motorLigado)
		{
			glPushMatrix();
				glTranslatef(0, (getR()/3), 0);
				glRotatef(velocidadeHelice,0,1,0);		
				desenhaHelice(1.0, 1.0, 0.0);
				glRotatef(120,0,1,0);
				desenhaHelice(1.0, 1.0, 0.0);
				glRotatef(120,0,1,0);
				desenhaHelice(1.0, 1.0, 0.0);				
			glPopMatrix();
			
		}
		else
		{
			glPushMatrix();
				glTranslatef(0, (getR()/3), 0);
				desenhaHelice(1.0, 1.0, 0.0);
			glPopMatrix();
		}	
*/

	glPopMatrix();	

}


















void Jogador::alterarAnguloXY(float coeficiente_ajuste){
    angulo_xy += velocidade * coeficiente_ajuste;
    angulo_canhao_arena_xy += velocidade * coeficiente_ajuste;
}

void Jogador::alterarAnguloXZ(float coeficiente_ajuste){
    if(angulo_xz + velocidade * coeficiente_ajuste <= 15 && angulo_xz + velocidade * coeficiente_ajuste >= -15){
        angulo_xz += velocidade * coeficiente_ajuste;
        angulo_canhao_arena_xz += velocidade * coeficiente_ajuste;
    }
}

void Jogador::alterarAnguloCanhaoXY(float incremento){
    if(angulo_canhao_xy + incremento <= 30 && angulo_canhao_xy + incremento >= -30){
        angulo_canhao_xy += incremento;
        angulo_canhao_arena_xy += incremento;
    }
}

void Jogador::alterarAnguloCanhaoXZ(float incremento){
    if(angulo_canhao_xz + incremento <= 30 && angulo_canhao_xz + incremento >= -30){
        angulo_canhao_xz += incremento;
        angulo_canhao_arena_xz += incremento;
    }
}

void Jogador::andar(float coeficiente_ajuste){
    // Caminha em xy
    y += sin(grausParaRadianos(angulo_xy)) * velocidade * coeficiente_ajuste;
    x += cos(grausParaRadianos(angulo_xy)) * velocidade * coeficiente_ajuste;
    // Caminha em xz
    z += sin(grausParaRadianos(angulo_xz)) * velocidade * coeficiente_ajuste;
    // x += cos(grausParaRadianos(angulo_xz)) * velocidade * coeficiente_ajuste;
}

void Jogador::girarHelices(float coeficiente_ajuste){
    angulo_helices += velocidade * coeficiente_ajuste * 2;
}

Jogador::~Jogador(){
}
