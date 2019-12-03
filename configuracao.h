#ifndef CONFIGURACAO_H_
#define CONFIGURACAO_H_

#include <string>

class Configuracao {
public:
	/*
	 * ARQUIVO ARENA
	 */
	const char* arquivo_arena_nome;
	const char* arquivo_arena_tipo;
	const char* arquivo_arena_caminho;
	/*
	 * JOGADOR
	 */
	float jogador_velocidade;
	float jogador_velocidade_tiro;
	/*
	 * INIMIGO
	 */
	float inimigo_velocidade;
	float inimigo_velocidade_tiro;
	float inimigo_frequencia_tiro;

	Configuracao();
	virtual ~Configuracao();
};

#endif /* CONFIGURACAO_H_ */