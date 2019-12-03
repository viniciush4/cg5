#include <math.h>
#include <GL/glut.h>
#include <iostream>

using namespace std;

class Placar {
public:
	int quantidade_bases;
	int quantidade_bases_destruidas;
	int quantidade_bases_restantes;

	Placar();
	Placar(int quantidade_bases);
	void mensagem(void* font, string s, float x, float y, float z);
	void desenhar(int altura_janela, int largura_janela);
	void desenharMensagemFinal(int altura_janela, int largura_janela);
	void incrementarBasesDestruidas();
	void resetarPlacar();

	virtual ~Placar();
};
