#include "minimapa.h"

Minimapa::Minimapa()
{

}

void Minimapa::desenhar(Arena &arena, Jogador &jogador, vector<Inimigo> &inimigos, vector<Base> &bases, GLfloat larguraJanela, GLfloat alturaJanela)
{
	glLoadIdentity();
    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity ();
        glOrtho (0, larguraJanela, 0, alturaJanela, -1, 1);
        
		glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);

			glPushAttrib(GL_ENABLE_BIT);
				
				//Representa 1/4 da largura da janela.
				float centroDesenhoArena= larguraJanela/8;

				glTranslatef(larguraJanela - centroDesenhoArena, alturaJanela - (alturaJanela - centroDesenhoArena),0);
				
				//Raio da arena = 60
				glScalef(0.2, 0.2, 0.2);

				//Espessura da linha
				glLineWidth(3.0);

				//Desenhar a arena
				//-------------------------------------------------------------------------------
				float angulo = 0.0;
				int numVertices = 1000;

				//Cor da borda = amarelo
				glColor3f(1, 1, 0);

				//Desenha o círculo		
				glBegin(GL_LINE_LOOP);
					for(int i=0; i < numVertices; i++)
					{
						glVertex3f(arena.x + arena.r * cos(angulo), arena.y + arena.r * sin(angulo), 0.0);
						angulo += 2 * M_PI / numVertices;
					}
				glEnd();

				//Desenhar o jogador
				//-------------------------------------------------------------------------------
				glPushAttrib(GL_ENABLE_BIT);

					angulo = 0.0;
					numVertices = 1000;

					//Cor do jogador = verde
					glColor3f(0, 1, 0);

					//Desenha o círculo		
					glBegin(GL_POLYGON);
						for(int i=0; i < numVertices; i++)
						{
							glVertex3f(jogador.x + jogador.r * cos(angulo), jogador.y + jogador.r * sin(angulo), 0.0);
							angulo += 2 * M_PI / numVertices;
						}
					glEnd();

				glPopAttrib();

				//Desenhar os inimigos
				//-------------------------------------------------------------------------------
				glPushAttrib(GL_ENABLE_BIT);
					for(Inimigo inimigo: inimigos)
					{	
						angulo = 0.0;
						numVertices = 1000;

						//Cor dos inimigos = vermelho
						glColor3f(1, 0, 0);

						//Desenha o círculo		
						glBegin(GL_POLYGON);
							for(int i=0; i < numVertices; i++)
							{
								glVertex3f(inimigo.x + inimigo.r * cos(angulo), inimigo.y + inimigo.r * sin(angulo), 0.0);
								angulo += 2 * M_PI / numVertices;
							}
						glEnd();
					}
				glPopAttrib();


				//Desenhar as bases
				//-------------------------------------------------------------------------------
				glPushAttrib(GL_ENABLE_BIT);
					for(Base base: bases)
					{	
						angulo = 0.0;
						numVertices = 1000;

						//Cor das bases = amarelo
						glColor3f(1, 1, 0);

						//Desenha o círculo		
						glBegin(GL_POLYGON);
							for(int i=0; i < numVertices; i++)
							{
								glVertex3f(base.x + base.r * cos(angulo), base.y + base.r * sin(angulo), 0.0);
								angulo += 2 * M_PI / numVertices;
							}
						glEnd();
					}
				glPopAttrib();


			glPopAttrib();

		glPopAttrib();

    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
}