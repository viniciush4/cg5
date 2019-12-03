#include "placar.h"

using namespace std;

Placar::Placar(){
    this->quantidade_bases = 0;
    this->quantidade_bases_destruidas = 0;
    this->quantidade_bases_restantes = 0;
}

Placar::Placar(int quantidade_bases){
    this->quantidade_bases = quantidade_bases;
    this->quantidade_bases_destruidas = 0;
    this->quantidade_bases_restantes = quantidade_bases;
}

//Mensagem na tela
void Placar::mensagem(void * font, string s, float x, float y, float z)
{
    glRasterPos3f(x, y, z);
	

    for(size_t i = 0; i < s.length(); i++)
    {
        glutBitmapCharacter(font, s[i]);
    }
}

void Placar::desenhar(int altura_janela, int largura_janela){

    glLoadIdentity();
    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity ();
        glOrtho (0, largura_janela, 0, altura_janela, -1, 1);
        
		glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);

                glColor3f(0.0, 1.0, 0.0);
                mensagem(GLUT_BITMAP_HELVETICA_12, "BASES", largura_janela - 80, altura_janela - 20 , 0);
                glColor3f(1.0, 1.0, 1.0);
                mensagem(GLUT_BITMAP_HELVETICA_10, "DESTRUIDAS = ", largura_janela - 110, altura_janela - 40, 0);
                glColor3f(0.0, 1.0, 0.0);
                mensagem(GLUT_BITMAP_HELVETICA_10, to_string(quantidade_bases_destruidas), largura_janela - 25, altura_janela - 40, 0);
                glColor3f(1.0, 1.0, 1.0);
                mensagem(GLUT_BITMAP_HELVETICA_10, "FALTAM = ", largura_janela - 110, altura_janela - 55, 0);
                glColor3f(0.0, 1.0, 0.0);
                mensagem(GLUT_BITMAP_HELVETICA_10, to_string(quantidade_bases_restantes), largura_janela - 25, altura_janela - 55, 0);

        glPopAttrib();

    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
}

void Placar::desenharMensagemFinal(int altura_janela, int largura_janela){

    glLoadIdentity();
    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity ();
        glOrtho (0, largura_janela, 0, altura_janela, -1, 1);
        
		glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
    
                if(quantidade_bases_restantes == 0){

                    char buffer [23];
                    sprintf (buffer, "PARABENS, VOCE GANHOU!");

                    // Quadrado
                    // glPushMatrix();
                    //     glTranslatef(largura_janela/2, altura_janela/2, 0);
                    //     glColor3f(0, 1, 0);
                    //     glBegin(GL_POLYGON);
                    //         glVertex3f(-90, -5, 0);
                    //         glVertex3f(88, -5, 0);
                    //         glVertex3f(88, 13, 0);
                    //         glVertex3f(-90, 13, 0);
                    //     glEnd();
                    // glPopMatrix();

                    glPushMatrix();
                        glTranslatef(largura_janela/2-130, altura_janela/2, 0);
                        glPushMatrix();
                            glColor3f(1, 1, 1);
                            glRasterPos3f(0, 0, 0);
                            for (size_t i = 0; i < 22; i++) {
                                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);
                            }
                        glPopMatrix();
                    glPopMatrix();

                } else {

                    char buffer [23];
                    sprintf (buffer, "QUE PENA, VOCE PERDEU!");

                    // Quadrado
                    // glPushMatrix();
                    //     glTranslatef(largura_janela/2, altura_janela/2, 0);
                    //     glColor3f(1, 0, 0);
                    //     glBegin(GL_POLYGON);
                    //         glVertex3f(-90, -5, 0);
                    //         glVertex3f(88, -5, 0);
                    //         glVertex3f(88, 13, 0);
                    //         glVertex3f(-90, 13, 0);
                    //     glEnd();
                    // glPopMatrix();

                    glPushMatrix();
                        glTranslatef(largura_janela/2-120, altura_janela/2, 0);
                        glPushMatrix();
                            glColor3f(1, 1, 1);
                            glRasterPos3f(0, 0, 0);
                            for (size_t i = 0; i < 22; i++) {
                                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);
                            }
                        glPopMatrix();
                    glPopMatrix();
                }
        glPopAttrib();

    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
}

void Placar::incrementarBasesDestruidas(){
    this->quantidade_bases_destruidas++;
    this->quantidade_bases_restantes--;
}

void Placar::resetarPlacar(){
    this->quantidade_bases_destruidas = 0;
    this->quantidade_bases_restantes = this->quantidade_bases;
}

Placar::~Placar(){
}
