#include "jogador.h"


Jogador::Jogador(){
	this->x = 0;
	this->y = 0;
    this->z = 0;
	this->r = 0;
    this->angulo_xy = 0;
    this->angulo_xz = 0;
	this->angulo_inclinacao = 0;
    this->angulo_canhao_xy = 0;
    this->angulo_canhao_xz = 0;
    this->angulo_canhao_arena_xy = 0;
    this->angulo_canhao_arena_xz = 0;
    this->angulo_helices = 0;
    this->velocidade = 0;
	this->curvando = false;
}

Jogador::Jogador(float x, float y, float z, float r, float velocidade){
	this->x = x;
	this->y = y;
    this->z = z;
	this->r = r;
    this->angulo_xy = 0;
    this->angulo_xz = 0;
	this->angulo_inclinacao = 0;
    this->angulo_canhao_xy = 0;
    this->angulo_canhao_xz = 0;
    this->angulo_canhao_arena_xy = 0;
    this->angulo_canhao_arena_xz = 0;
    this->angulo_helices = 0;
    this->velocidade = velocidade;
	this->curvando = false;
}

void Jogador::desenharModeloAviao(LerOBJ &aviaoJogador, struct obj_model_t &modeloAviaoJogador, LerOBJ &helice, struct obj_model_t &modeloHelice, GLuint texturaJogador)
{
	GLfloat mat_ambient_w[] = { 1.0, 1.0, 1.0, 1.0 };
	glColor3fv(mat_ambient_w);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturaJogador);
	
	 glPushMatrix();

		

		//Posiciona o avião de acordo com o ângulo da pista
		glTranslatef(x, y, z);		
		glScalef(4,4,4);
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		
		//Movimento do avião
		glRotatef(angulo_xy, 0, 1, 0);
		glRotatef(angulo_xz, -1, 0, 0);
		glRotatef(angulo_inclinacao, 0, 0, -1);

		if(!curvando)
		{
			glRotatef(angulo_inclinacao, 0, 0, 1);
		}
	
		//Desenha o modelo do avião
		aviaoJogador.RenderOBJModel(&modeloAviaoJogador);

		//Desenha as hélices
		glPushMatrix();
			glTranslatef(r/8.5,r/25,r/7);
			glRotatef(angulo_helices,0,0,1);
			helice.RenderOBJModel(&modeloHelice);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-r/8.5,r/25,r/7);
			glRotatef(angulo_helices,0,0,1);
			helice.RenderOBJModel(&modeloHelice);
		glPopMatrix();

		//Desenha o canhão
		glPushMatrix();
			
			//Desenha na ponta do avião
			glTranslatef(0,r/13,r/4);
			
			//Movimentos do canhão
			glRotatef(angulo_canhao_xy, 0, 1, 0);
			glRotatef(angulo_canhao_xz, -1, 0, 0);

			glPushMatrix();
				GLfloat mat_ambient_k[] = { 0.0, 0.0, 0.0, 1.0 };
				glColor3fv(mat_ambient_k);

				//glRotatef(90, 0, 0, 1);
				GLUquadric* q = gluNewQuadric();
				gluQuadricOrientation(q, GLU_INSIDE);
				gluQuadricDrawStyle(q, GLU_FILL); //GLU_FILL, GLU_LINE, GLU_SILHOUETTE, GLU_POINT
				gluQuadricNormals(q, GLU_FLAT); //GLU_NONE, GLU_FLAT, GLU_SMOOTH

				gluCylinder(q, r/80, r/80, r/14, 30, 30);
			glPopMatrix();			
		
		glPopMatrix();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
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
		desenharHeliceDireita();
		
		//Desenha a hélice esquerda
		desenharHeliceEsquerda();

		//Desenha o canhão
		desenharCanhao();



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
		glTranslatef(0, r/3, r);

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

void Jogador::desenharPiramide()
{
	glPushMatrix();
		glBegin(GL_QUADS);
			glVertex3f(0.5,-1,-0.5);  
			glVertex3f(0.5,-1,0.5);  
			glVertex3f(-0.5,-1,0.5);  
			glVertex3f(-0.5,-1,-0.5);
		glEnd();

		glBegin(GL_TRIANGLES);
			glVertex3f(0,0,0);
			glVertex3f(0.5,-1,-0.5);
			glVertex3f(0.5,-1,0.5);

			glVertex3f(0,0,0);
			glVertex3f(-0.5,-1,-0.5);
			glVertex3f(-0.5,-1,0.5);

			glVertex3f(0,0,0);
			glVertex3f(-0.5,-1,0.5);
			glVertex3f(0.5,-1,0.5);

			glVertex3f(0,0,0);
			glVertex3f(0.5,-1,0.5);
			glVertex3f(-0.5,-1,0.5);
		glEnd();

	glPopMatrix();
}

void Jogador::desenharHeliceDireita()
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

		//Desenha a hélice direita
		glPushMatrix();
			GLfloat mat_ambient_y[] = { 1.0, 1.0, 0.0, 1.0 };

			glScalef(0.5, 0.5, 0.5);
			glTranslatef(0, (r/2), 0);
			glRotatef(90,0,0,1);
			glRotatef(angulo_helices,1,0,0);

			glColor3fv(mat_ambient_y);
			for(int i=0;i<3;i++){
				glPushMatrix();
					glScalef(0.15*r,0.6*r,1);
					desenharPiramide();
				glPopMatrix();

				glRotatef(120,1,0,0);
			}


		glPopMatrix();



	glPopMatrix();

}

void Jogador::desenharHeliceEsquerda()
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
		//Desenha a hélice esquerda
		glPushMatrix();
			GLfloat mat_ambient_y[] = { 1.0, 1.0, 0.0, 1.0 };

			glScalef(0.5, 0.5, 0.5);
			glTranslatef(0, (r/2), 0);
			glRotatef(90,0,0,1);
			glRotatef(angulo_helices,1,0,0);

			glColor3fv(mat_ambient_y);
			for(int i=0;i<3;i++){
				glPushMatrix();
					glScalef(0.15*r,0.6*r,1);
					desenharPiramide();
				glPopMatrix();

				glRotatef(120,1,0,0);
			}


		glPopMatrix();




	glPopMatrix();	

}

void Jogador::desenharCanhao()
{
	//Desenha o canhão
	glPushMatrix();
		glTranslatef(0, r - (r/10), 0);
		
//		glRotatef(anguloCanhao, 0, 0, 1);
//		desenharRetangulo(getR()/8, 2*getR()/5, 0.0, 0.0, 0.0);
		
		glRotatef(angulo_canhao_xy, 0, 0, 1);
		glRotatef(angulo_canhao_xz, 1, 0, 0);

		glPushMatrix();
			GLfloat mat_ambient_k[] = { 0.0, 0.0, 0.0, 1.0 };

			glRotatef(-90, 1, 0, 0);
			GLUquadric* q = gluNewQuadric();
			gluQuadricOrientation(q, GLU_INSIDE);
			gluQuadricDrawStyle(q, GLU_FILL); //GLU_FILL, GLU_LINE, GLU_SILHOUETTE, GLU_POINT
			gluQuadricNormals(q, GLU_FLAT); //GLU_NONE, GLU_FLAT, GLU_SMOOTH

			glColor3fv(mat_ambient_k);
			gluCylinder(q, r/12, r/12, 2*r/4, 30, 30);
		glPopMatrix();			
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

void Jogador::alterarAnguloInclinacao(float coeficiente_ajuste){
    if(angulo_inclinacao + velocidade * coeficiente_ajuste <= 30 && angulo_inclinacao + velocidade * coeficiente_ajuste >= -30){
        angulo_inclinacao += velocidade * coeficiente_ajuste;
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

void Jogador::andar(float coeficiente_ajuste, float alturaArena){
    // Caminha em xy
    y += sin(grausParaRadianos(angulo_xy)) * velocidade * coeficiente_ajuste;
    x += cos(grausParaRadianos(angulo_xy)) * velocidade * coeficiente_ajuste;
    // Caminha em xz
	
	if(z >= 0 && z <= (alturaArena - (sin(grausParaRadianos(angulo_xz)) * velocidade * coeficiente_ajuste)))
    {
		if(z + sin(grausParaRadianos(angulo_xz)) * velocidade * coeficiente_ajuste >= 0)
		{
			z += sin(grausParaRadianos(angulo_xz)) * velocidade * coeficiente_ajuste;
		}
		
	}
	// x += cos(grausParaRadianos(angulo_xz)) * velocidade * coeficiente_ajuste;
}

void Jogador::girarHelices(float coeficiente_ajuste){
    angulo_helices += velocidade * coeficiente_ajuste * 2;
}

Jogador::~Jogador(){
}
