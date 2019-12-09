
#include "inimigo.h"


Inimigo::Inimigo(){
	this->x = 0;
	this->y = 0;
    this->z = 0;
	this->r = 0;
    this->angulo_xy = rand() % 360;
    this->angulo_xz = 0;
    this->angulo_canhao_xy = 0;
    this->angulo_canhao_xz = 0;
    this->angulo_canhao_arena_xy = 0;
    this->angulo_canhao_arena_xz = 0;
    this->angulo_helices = 0;
    this->velocidade = 0;
    this->somatorio_incremento_angulo = 0;
    this->incrementar_angulo_xy = true;
    this->incrementar_altura_z = true;
    this->tempo_desde_ultimo_tiro = 0;
}

Inimigo::Inimigo(float x, float y, float z, float r, float velocidade){
	this->x = x;
	this->y = y;
    this->z = z;
	this->r = r;
    this->angulo_xy = rand() % 360;
    this->angulo_xz = 0;
    this->angulo_canhao_xy = 0;
    this->angulo_canhao_xz = 0;
    this->angulo_canhao_arena_xy = 0;
    this->angulo_canhao_arena_xz = 0;
    this->angulo_helices = 0;
    this->velocidade = velocidade;
    this->somatorio_incremento_angulo = 0;
    this->incrementar_angulo_xy = true;
    this->incrementar_altura_z = true;
    this->tempo_desde_ultimo_tiro = 0;
}


void Inimigo::desenharModeloAviao(LerOBJ &aviaoJogador, struct obj_model_t &modeloAviaoJogador, LerOBJ &helice, struct obj_model_t &modeloHelice, GLuint texturaInimigo)
{
	GLfloat mat_ambient_w[] = { 1.0, 1.0, 1.0, 1.0 };
	glColor3fv(mat_ambient_w);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturaInimigo);


	glPushMatrix();


		//Posiciona o avião de acordo com o ângulo da pista
		glTranslatef(x, y, z);		
		glScalef(4,4,4);
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		
		//Movimento do avião
		glRotatef(angulo_xy, 0, 1, 0);
		glRotatef(angulo_xz, -1, 0, 0);
		
	
		//Desenha o modelo do avião
		aviaoJogador.RenderOBJModel(&modeloAviaoJogador);

		//Desenha as hélices
		glPushMatrix();
			glTranslatef(r/8.5,-r/35,r/9);
			glRotatef(angulo_helices,0,0,1);
			helice.RenderOBJModel(&modeloHelice);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-r/8.5,-r/35,r/9);
			glRotatef(angulo_helices,0,0,1);
			helice.RenderOBJModel(&modeloHelice);
		glPopMatrix();

		//Desenha o canhão
		glPushMatrix();
			
			//Desenha na ponta do avião
			glTranslatef(0, 0.14, r/4.7);
			
			glPushMatrix();
				GLfloat mat_ambient_k[] = { 0.0, 0.0, 0.0, 1.0 };
				glColor3fv(mat_ambient_k);

				//glRotatef(90, 0, 0, 1);
				GLUquadric* q = gluNewQuadric();
				gluQuadricOrientation(q, GLU_INSIDE);
				gluQuadricDrawStyle(q, GLU_FILL); //GLU_FILL, GLU_LINE, GLU_SILHOUETTE, GLU_POINT
				gluQuadricNormals(q, GLU_FLAT); //GLU_NONE, GLU_FLAT, GLU_SMOOTH

				gluCylinder(q, r/100, r/100, r/14, 30, 30);
			glPopMatrix();			
		
		glPopMatrix();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}


void Inimigo::desenhar()
{
    glPushMatrix();
        glTranslatef(x, y, z+r);

        glColor3f(1, 0, 0);
	//	glNormal3f(0.0, 0.0, 1.0);

        glutSolidSphere(r,100,100);
    glPopMatrix();

}

void Inimigo::alterarAngulo(float incremento_xy, float incremento_xz){
    angulo_xy += incremento_xy;
    angulo_xz += incremento_xz;
    angulo_canhao_arena_xy += incremento_xy;
    angulo_canhao_arena_xz += incremento_xz;
}

void Inimigo::andar(float coeficiente_ajuste){
    y += sin(grausParaRadianos(angulo_xy)) * velocidade * coeficiente_ajuste;
    x += cos(grausParaRadianos(angulo_xy)) * velocidade * coeficiente_ajuste;
}

void Inimigo::girarHelices(float coeficiente_ajuste){
    angulo_helices += velocidade * coeficiente_ajuste * 2;
}

Inimigo::~Inimigo(){
}
