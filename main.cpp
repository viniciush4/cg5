#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tinyxml2.h"
#include "configuracao.h"
#include "pista.h"
#include "arena.h"
#include "inimigo.h"
#include "base.h"
#include "bomba.h"
#include "bala.h"
#include "jogador.h"
#include "placar.h"
#include "imageloader.h"
#include "minimapa.h"
#include "lerOBJ.h"
#include "lerTextura.h"

#include "Camera3pJogador.h"
#include "Camera3pInimigo.h"
#include "Camera3pBase.h"
#include "CameraCanhao.h"
#include "Camera1pJogador.h"
#include "CameraBomba.h"

#define grausParaRadianos(g) g*(M_PI/180)
#define radianosParaGraus(r) r*(180/M_PI)

using namespace tinyxml2;
using namespace std;

/*
 * DIMENSÕES DO JOGO
 */
int larguraJanela = 500;
int alturaJanela = 500;

/*
 * MODELOS IMPORTADOS NO JOGO
 */

//Avião do jogador
struct obj_model_t modeloAviaoJogador;
LerOBJ aviaoJogador = LerOBJ();

//Hélice
struct obj_model_t modeloHelice;
LerOBJ helice = LerOBJ();

//Bombas
struct obj_model_t modeloBomba;
LerOBJ bomba = LerOBJ();

//Bases
struct obj_model_t modeloBase;
LerOBJ base = LerOBJ();

/*
* TEXTURAS
*/

//Jogador
GLuint texturaJogador = 0;
LerTextura carregarTexturaJogador = LerTextura();

//Inimigo
GLuint texturaInimigo = 0;
LerTextura carregarTexturaInimigo = LerTextura();

//Céu 
GLuint texturaCeu = 0;
LerTextura carregarTexturaCeu = LerTextura();

//Chão
GLuint texturaChao = 0;
LerTextura carregarTexturaChao = LerTextura();

//Base
GLuint texturaBase = 0;
LerTextura carregarTexturaBase = LerTextura();

//Pista
GLuint texturaPista = 0;
LerTextura carregarTexturaPista = LerTextura();

//Árvore
GLuint texturaArvore = 0;
LerTextura carregarTexturaArvore = LerTextura();

/*
 * MAPEAMENTO DAS TECLAS
 */
int teclas[256];

/*
 * VARIÁVEIS DO JOGO
 */
Configuracao configuracao;
vector<Inimigo> inimigos;
vector<Inimigo> inimigos_copia;
vector<Base> bases;
vector<Base> bases_copia;
vector<Bomba> bombas;
vector<Bala> balas;
vector<Bala> balasInimigas;
Jogador jogador;
Jogador jogador_copia;
Pista pista;
Arena arena;
Placar placar;
Minimapa minimapa;
int estado = 0;

bool curvando = false;
bool botao_direito_status = false;
int incremento_zoom = 0;

/*
 * VARIÁVEIS PARA AJUSTE DO TEMPO
 */
static GLdouble previousTime = 0;
GLdouble currentTime;
GLdouble timeDiference;

/*
 * VARIÁVEIS DA CÂMERA
 */
int mouse_ultima_posicao_x = 0;
int mouse_ultima_posicao_y = 0;
int mouse_ultima_posicao_x_motion = 0;
int mouse_ultima_posicao_y_motion = 0;
float momentoTiro = 0;

double cam3_angulo_xy;
double cam3_angulo_xz;
double cam5_angulo_xy;
double cam5_angulo_xz;
double cam6_angulo_xy;
double cam6_angulo_xz;
double distancia_camera = 0;

bool cam1 = true;
bool cam2 = false;
bool cam3 = false;
bool cam4 = false;
bool cam5 = false;
bool cam6 = false;
bool desenhar_cameras = false;

Camera3pJogador* camera3pJogador;
Camera3pInimigo* camera3pInimigo;
Camera3pBase* camera3pBase;
CameraCanhao* cameraCanhao;
Camera1pJogador* camera1pJogador;
CameraBomba* cameraBomba;

bool verificarLimiteArena(Jogador jogador) {	
	//Calcula a distância do piloto até a borda da arena
	float distancia = sqrt(pow(jogador.x - arena.x, 2.0) + pow(jogador.y - arena.y, 2.0));

	//Verifica se o piloto está fora da arena
	if(distancia >= arena.r){
		return true;
	} else {
		return false;
	}
}

bool verificarColisaoInimigo() {
	bool explodiu = false;
	
	//Percorre todos os inimigos aéreos
	for(size_t i = 0; i < inimigos.size(); i++)	{
		//Calcula a distância do piloto até o inimigo
		float distancia = sqrt(pow(jogador.x - inimigos[i].x, 2.0) + pow(jogador.y - inimigos[i].y, 2.0) + pow(jogador.z - inimigos[i].z, 2.0));

		//Verifica se houve colisão
		if(distancia < inimigos[i].r/2 + jogador.r/2) {
			explodiu = true;	
			break;
		}
	}
	return explodiu;
}

void teletransportarJogador(){
	double m = tanf(grausParaRadianos(jogador.angulo_xy));
	double E = jogador.y - 0;
	double A = 1+pow(m,2);
	double B = -2*pow(m,2)*jogador.x + 2*E*m;
	double C = pow(E,2) - 2*E*m*jogador.x + pow(m,2)*pow(jogador.x,2) - pow(arena.r,2);

	double x1 = (-B + sqrt(pow(B,2) - 4*A*C))/(2*A);
	double x2 = (-B - sqrt(pow(B,2) - 4*A*C))/(2*A);

	double y1 = jogador.y + m*(x1-jogador.x);
	double y2 = jogador.y + m*(x2-jogador.x);

	if(fabs(jogador.x - x1) < fabs(jogador.x - x2)){
		jogador.x = x2;
		jogador.y = y2;
	} else {
		jogador.x = x1;
		jogador.y = y1;
	}
}

void teletransportarInimigo(Inimigo& inimigo) {
	double m = tanf(grausParaRadianos(inimigo.angulo_xy));
	double E = inimigo.y - 0;
	double A = 1+pow(m,2);
	double B = -2*pow(m,2)*inimigo.x + 2*E*m;
	double C = pow(E,2) - 2*E*m*inimigo.x + pow(m,2)*pow(inimigo.x,2) - pow(arena.r,2);

	double x1 = (-B + sqrt(pow(B,2) - 4*A*C))/(2*A);
	double x2 = (-B - sqrt(pow(B,2) - 4*A*C))/(2*A);

	double y1 = inimigo.y + m*(x1-inimigo.x);
	double y2 = inimigo.y + m*(x2-inimigo.x);

	if(fabs(inimigo.x - x1) < fabs(inimigo.x - x2)){
		inimigo.x = x2;
		inimigo.y = y2;
	}else{
		inimigo.x = x1;
		inimigo.y = y1;
	}
}

/*
 * REGRAS DO JOGO
 */
void reiniciarJogo() {
	jogador = jogador_copia;
	inimigos = inimigos_copia;
	bases = bases_copia;
	bombas.clear();
	balas.clear();
	balasInimigas.clear();
	placar.resetarPlacar();
	estado = 0;
}

void atualizarEstadoJogador() {
	jogador.girarHelices(timeDiference/1000);
	jogador.andar(timeDiference/1000, arena.altura);
}

void atualizarEstadoInimigos() {
	for(auto it=inimigos.begin(); it!=inimigos.end();++it){
		Inimigo &inimigo = *it;
		inimigo.girarHelices(timeDiference/1000);
		inimigo.andar((timeDiference/1000) * configuracao.inimigo_velocidade);
		// Colisão com a arena
		float distancia = sqrt(pow(inimigo.x,2)+pow(inimigo.y,2));
		if(distancia > arena.r) {
			teletransportarInimigo(inimigo);
		}
		// Muda o ângulo xy (180 graus em 5 segundos)
		float incremento_angulo = 180 / (5 / (timeDiference/1000));
		incremento_angulo = inimigo.incrementar_angulo_xy ? incremento_angulo : -incremento_angulo;
		inimigo.somatorio_incremento_angulo += incremento_angulo;
		if(inimigo.incrementar_angulo_xy && inimigo.somatorio_incremento_angulo > 180){
			inimigo.somatorio_incremento_angulo = 0;
			inimigo.incrementar_angulo_xy = !inimigo.incrementar_angulo_xy;
		}
		if(!inimigo.incrementar_angulo_xy && inimigo.somatorio_incremento_angulo < -180){
			inimigo.somatorio_incremento_angulo = 0;
			inimigo.incrementar_angulo_xy = !inimigo.incrementar_angulo_xy;
		}
		inimigo.alterarAngulo(incremento_angulo, 0);
		// Muda a altura z 
		if(arena.altura-inimigo.z < inimigo.r && inimigo.incrementar_altura_z)
			inimigo.incrementar_altura_z = false;
		if(inimigo.z < inimigo.r)
			inimigo.incrementar_altura_z = true;

		if(inimigo.incrementar_altura_z)
			inimigo.z = inimigo.z + 0.5;
		else
			inimigo.z = inimigo.z - 0.5;
	}
}

bool verificarLimiteBalaArena(float x, float y, float r) {
	float distancia = sqrt(pow(x - arena.x, 2.0) + pow(y - arena.y, 2.0));

	if(distancia >= arena.r - r) {
		return true;
	} else {
		return false;
	}
}

bool derrubarInimigo(float x, float y, float z, float r) {
	//Percorre todos os inimigos 
	for(auto it = inimigos.begin(); it != inimigos.end();) {
		Inimigo &inimigo = *it;

		float distancia = sqrt(pow(x - inimigo.x, 2.0) + pow(y - inimigo.y, 2.0)  + pow(z - inimigo.z, 2.0));
		
		//Verificar se as balas sairam da arena
		if(distancia < inimigo.r/2 + r) {
			it = inimigos.erase(it);
			return true;		
		} else {	
			++it;
		}	
	}
	return false;
}

bool derrubarJogador(float x, float y, float z, float r) {
	float distancia = sqrt(pow(x - jogador.x, 2.0) + pow(y - jogador.y, 2.0) + pow(z - jogador.z, 2.0));
	
	if(distancia < jogador.r/2 + r)	{
		return true;
	} else {
		return false;
	}
}

void atualizarEstadoTirosJogador() {

	for(auto it = balas.begin(); it != balas.end();) {
		Bala &bala = *it;
		
		//Mover as balas em linha reta
		bala.mover(timeDiference/1000);
		
		//Verificar se as balas sairam da arena
		if(verificarLimiteBalaArena(bala.x, bala.y, (bala.r/100)*4)) {
			it = balas.erase(it);		
		}		
		//Verificar se acertou o inimigo
		else if(derrubarInimigo(bala.x, bala.y, bala.z, bala.r/100)) {
			it = balas.erase(it);			
		} else {	
			++it;
		}	
	}
}

void atualizarEstadoTirosInimigos() {

	for(auto it = balasInimigas.begin(); it != balasInimigas.end();) {
		Bala &balaInimiga = *it;
		
		//Mover as balas em linha reta
		balaInimiga.moverBalaInimiga(timeDiference/1000);
	
		//Verificar se as balas sairam da arena
		if(verificarLimiteBalaArena(balaInimiga.x, balaInimiga.y, (balaInimiga.r/100)*4)) {
			it = balasInimigas.erase(it);		
		}			
		//Verificar se acertou o jogador
		else if(derrubarJogador(balaInimiga.x, balaInimiga.y, balaInimiga.z, balaInimiga.r/100)) {
			it = balasInimigas.erase(it);
			estado = 3;			
		} else {	
			++it;
		}	
	}
}

void atualizarEstadoBombas() {
	for(auto it=bombas.begin(); it!=bombas.end();){
		Bomba &bomba = *it;
		// Posição
		bomba.mover(timeDiference/1000);
		// Condições de remoção da bomba (saiu da arena ou chegou ao raio mínimo)
		float distancia = sqrt(pow(bomba.x,2)+pow(bomba.y,2));
		if(distancia > arena.r || bomba.z <= 0){
			
			// Se a bomba está em cima de alguma base inimiga, deleta a base
			for(auto it2=bases.begin(); it2!=bases.end();){
				Base &base = *it2;
				float distancia_base = sqrt(pow(bomba.x - base.x,2)+pow(bomba.y - base.y,2));
				if(distancia_base < (bomba.r + base.r)){
					placar.incrementarBasesDestruidas();
					it2 = bases.erase(it2);
					if(placar.quantidade_bases_restantes == 0){
						estado = 3;
					}
				} else {
					++it2;
				}
			}
			it = bombas.erase(it);
		} else {
			++it;
		}
	}
}

void criarTirosInimigos() {
	//Frequência do tiro do inimigo
	momentoTiro += timeDiference * configuracao.inimigo_frequencia_tiro;
	if(momentoTiro >= 1000)	{

		for(auto it = inimigos.begin(); it != inimigos.end();) {
			Inimigo &inimigo = *it;
			
			//Criar bala inimiga
			Bala bala = Bala(
				inimigo.x, 
				inimigo.y,
				inimigo.z,
				inimigo.r,
				inimigo.angulo_xy,
				inimigo.angulo_xz,
				inimigo.angulo_canhao_xy,  
				inimigo.angulo_canhao_xz,
				inimigo.angulo_canhao_arena_xy,
				inimigo.angulo_canhao_arena_xz,
				0,
				false,
				configuracao.inimigo_velocidade_tiro*inimigo.velocidade
			);
			balasInimigas.push_back(bala);
			++it;
		}	
		momentoTiro = 0.0;
	}
}

/*
 * CONFIGURAÇÕES
 */

void inicializarOpengl() {
	// Especifica que a cor de fundo da janela será preta
	glClearColor(0.0, 0.0, 0.0, 1.0);
	// Habilita a definicao da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminacao
	glEnable(GL_LIGHTING);
	// Habilita a luz de numero 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);
	//Habilitar texturas
	glEnable(GL_TEXTURE_2D);
	// Habilita o modelo de colorizacao de Gouraud
	glShadeModel(GL_SMOOTH);	
}

bool inicializarObjetosJogo(char* caminho_arquivo_configuracoes) {

	/*
	 * CONFIG.XML
	 */
	XMLDocument doc_configuracoes;
    bool loaded_conf = doc_configuracoes.LoadFile(strcat(caminho_arquivo_configuracoes,"config.xml"));
	if(loaded_conf){
		cout << "Erro ao carregar configurações" << endl;
		return false;
	}

		XMLElement* xml_arquivo_arena 		= doc_configuracoes.FirstChildElement()->FirstChildElement( "arquivoDaArena" );
		configuracao.arquivo_arena_nome 	= xml_arquivo_arena->FirstChildElement( "nome" )->GetText();
		configuracao.arquivo_arena_tipo 	= xml_arquivo_arena->FirstChildElement( "tipo" )->GetText();
		configuracao.arquivo_arena_caminho 	= xml_arquivo_arena->FirstChildElement( "caminho" )->GetText();

		XMLElement* xml_jogador 			= doc_configuracoes.FirstChildElement()->FirstChildElement( "jogador" );
		xml_jogador->QueryFloatAttribute( "vel", &configuracao.jogador_velocidade );
		xml_jogador->QueryFloatAttribute( "velTiro", &configuracao.jogador_velocidade_tiro );

		XMLElement* xml_inimigo 			= doc_configuracoes.FirstChildElement()->FirstChildElement( "inimigo" );
		xml_inimigo->QueryFloatAttribute( "vel", &configuracao.inimigo_velocidade );
		xml_inimigo->QueryFloatAttribute( "velTiro", &configuracao.inimigo_velocidade_tiro );
		xml_inimigo->QueryFloatAttribute( "freqTiro", &configuracao.inimigo_frequencia_tiro );

	/*
	 * ARENA.SVG
	 */
	XMLDocument doc_arena;
	char* caminho_arena_svg = strcat(strcat(strcat((char*)configuracao.arquivo_arena_caminho, configuracao.arquivo_arena_nome),"."), configuracao.arquivo_arena_tipo);
	bool loaded_arena = doc_arena.LoadFile(caminho_arena_svg);
	if(loaded_arena){
		cout << "Erro ao carregar arena" << endl;
		return false;
	}
		float x1, y1, x2, y2;
		XMLElement* xml_pista = doc_arena.FirstChildElement( "svg" )->FirstChildElement( "line" );
		xml_pista->QueryFloatAttribute("x1", &x1);
		xml_pista->QueryFloatAttribute("y1", &y1);
		xml_pista->QueryFloatAttribute("x2", &x2);
		xml_pista->QueryFloatAttribute("y2", &y2);

		Pista p = Pista(x1, y1, x2, y2);
		pista = p;

		XMLElement* xml_circulo = doc_arena.FirstChildElement( "svg" )->FirstChildElement( "circle" );
		for(XMLElement* e = xml_circulo; e != NULL; e = e->NextSiblingElement("circle")) {
			
			float x, y, r;
			const char* fill;
			e->QueryFloatAttribute("cx", &x);
			e->QueryFloatAttribute("cy", &y);
			e->QueryFloatAttribute("r", &r);
			e->QueryStringAttribute("fill", &fill);

			if(!strcmp(fill,"blue")){
				Arena a = Arena(x, y, r, 0);
				arena = a;
			}
			if(!strcmp(fill,"red")){
				float distancia_pista = sqrt(pow(pista.y2-pista.y1,2) + pow(pista.x2-pista.x1,2));
				float velocidade = (distancia_pista / 8) * 4;
				Inimigo i = Inimigo(x, y, 0, r, velocidade);
				inimigos.push_back(i);
			}
			if(!strcmp(fill,"orange")){
				Base b = Base(x, y, r);
				bases.push_back(b);
			}
			if(!strcmp(fill,"green")){
				Jogador j = Jogador(x, y, 0, r, 0);
				jogador = j;
			}
		}

		// Seta a altura da arena (8x diametro do jogador)
		arena.altura = 8 * 2 * jogador.r;

		// Seta a largura da pista (mesma largura do jogador)
		pista.largura = 2 * jogador.r;

		int fator_correcao_x = (arena.r - arena.x) - arena.r;
		int fator_correcao_y = (arena.r - arena.y) - arena.r;

		// // Normaliza todos os elementos
		arena.x += fator_correcao_x;
		arena.y += fator_correcao_y;
		arena.y *= -1;

		jogador.x += fator_correcao_x;
		jogador.y += fator_correcao_y;
		jogador.y *= -1;

		pista.x1 += fator_correcao_x;
		pista.y1 += fator_correcao_y;
		pista.y1 *= -1;
		pista.x2 += fator_correcao_x;
		pista.y2 += fator_correcao_y;
		pista.y2 *= -1;

		for(size_t i=0; i < inimigos.size(); i++){
			inimigos.at(i).x += fator_correcao_x;
			inimigos.at(i).y += fator_correcao_y;
			inimigos.at(i).y *= -1;
		}

		for(size_t i=0; i < bases.size(); i++){
			bases.at(i).x += fator_correcao_x;
			bases.at(i).y += fator_correcao_y;
			bases.at(i).y *= -1;
		}

		// Seta valores iniciais para os ângulos do jogador
		float angulo = atan2((pista.y2-pista.y1), (pista.x2-pista.x1));
		jogador.angulo_xy = radianosParaGraus(angulo);
		jogador.angulo_canhao_arena_xy = radianosParaGraus(angulo);
		// anguloCameraJogadorXY = radianosParaGraus(angulo);

		// Seta valores iniciais para altura (z)
		for(size_t i=0; i < inimigos.size(); i++){
        	float z = rand() % (int)arena.altura;
			inimigos.at(i).z = z;
		}

		// Guarda instância Jogador
		jogador_copia = jogador;
		// Guarda instâncias Inimigos aéreos
		for(size_t i=0; i < inimigos.size(); i++){
        	inimigos_copia.push_back(inimigos.at(i));
		}
		// Guarda instâncias Inimigos terrestres
		for(size_t i=0; i < bases.size(); i++){
			bases_copia.push_back(bases.at(i));
		}

		// Inicia o placar
		placar = Placar(bases.size());

		//Cria o mini mapa
		minimapa = Minimapa();
		
		//Carrega os modelos 3d

		//Avião jogador
		if(!aviaoJogador.ReadOBJModel("Modelos/modeloAviao.obj", &modeloAviaoJogador))
		{
			cout << "Erro ao carregar o modelo avião do jogador!" << endl;
			exit(EXIT_FAILURE);
		}

		//Hélice
		if(!helice.ReadOBJModel("Modelos/modeloHelice.obj", &modeloHelice))
		{
			cout << "Erro ao carregar o modelo da hélice!" << endl;
			exit(EXIT_FAILURE);
		}

		//Bomba
		if(!bomba.ReadOBJModel("Modelos/modeloBomba.obj", &modeloBomba))
		{
			cout << "Erro ao carregar o modelo da bomba!" << endl;
			exit(EXIT_FAILURE);
		}

		//Bases
		if(!base.ReadOBJModel("Modelos/modeloBase.obj", &modeloBase))
		{
			cout << "Erro ao carregar o modelo da base!" << endl;
			exit(EXIT_FAILURE);
		}

		//Carregar as texturas

		//Jogador
		texturaJogador = carregarTexturaJogador.loadTGATexture("Texturas/jogador.tga", 0);
  		if(!texturaJogador)
		{
			cout << "Erro ao carregar a textura do Jogador!" << endl;
   	 		exit (EXIT_FAILURE);
		}

		//Inimigo
		texturaInimigo = carregarTexturaInimigo.loadTGATexture("Texturas/inimigo.tga", 0);
  		if(!texturaInimigo)
		{
			cout << "Erro ao carregar a textura do Inimigo!" << endl;
   	 		exit (EXIT_FAILURE);
		}

		//Céu da arena
		texturaCeu = carregarTexturaCeu.loadTGATexture("Texturas/ceu.tga", 0);
  		if(!texturaCeu)
		{
			cout << "Erro ao carregar a textura do céu da arena!" << endl;
   	 		exit (EXIT_FAILURE);
		}

		//Chão da arena
		texturaChao = carregarTexturaChao.loadTGATexture("Texturas/areia.tga", 0);
  		if(!texturaChao)
		{
			cout << "Erro ao carregar a textura do chão da arena!" << endl;
   	 		exit (EXIT_FAILURE);
		}

		//Base
		texturaBase = carregarTexturaBase.loadTGATexture("Texturas/base.tga", 0);
  		if(!texturaBase)
		{
			cout << "Erro ao carregar a textura da base!" << endl;
   	 		exit (EXIT_FAILURE);
		}

		//Pista
		texturaPista = carregarTexturaPista.loadTGATexture("Texturas/pista.tga", 0);
  		if(!texturaPista)
		{
			cout << "Erro ao carregar a textura da pista!" << endl;
   	 		exit (EXIT_FAILURE);
		}

		//Árvore
		texturaArvore = carregarTexturaArvore.loadTGATexture("Texturas/arvore.tga", 0);
  		if(!texturaArvore)
		{
			cout << "Erro ao carregar a textura da árvore!" << endl;
   	 		exit (EXIT_FAILURE);
		}

		camera3pJogador = new Camera3pJogador(
			-3*2*jogador.r,0,0,
            0,0,0,
            0,0,1
		);

		camera3pInimigo = new Camera3pInimigo(
			-3*2*inimigos[0].r,0,0,
            0,0,0,
            0,0,1
		);

		cameraCanhao = new CameraCanhao(
			jogador.r*0.9*cos(grausParaRadianos(jogador.angulo_xy)),jogador.r*0.9*sin(grausParaRadianos(jogador.angulo_xy)),7.3,
			0,0,0,
			0,0,1
		);

		camera1pJogador = new Camera1pJogador(
			jogador.r,0,11,
			0,0,0,
			0,0,1
		);

		cameraBomba = new CameraBomba(
			jogador.x,jogador.y,jogador.z,
			0,0,0,
			0,0,1
		);

		camera3pBase = new Camera3pBase(
			-3*2*bases[0].r,0,0,
			0,0,0,
			0,0,1
		);

		camera3pJogador->setZAngle(jogador.angulo_xy);
		cam3_angulo_xy = camera3pJogador->getZAngle();
    	cam3_angulo_xz = camera3pJogador->getYAngle();
		cam5_angulo_xy = 0;
		cam5_angulo_xz = 30;
		cam6_angulo_xy = 0;
		cam6_angulo_xz = 30;

	return true;
}


/*
 * CÂMERAS, LUZES E DESENHO
 */
void especificarIluminacao(void) {

	GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0}; 
	GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};	   // "cor" 
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho" 
	GLfloat posicaoLuz[4]={0.0, 0.0, 50.0, 1.0};

	GLfloat posicaoLuzSpot[4]={jogador.x+20, jogador.y, jogador.z, 1.0};
	GLfloat direcaoLuzSpot[4]={jogador.x+50, jogador.y, jogador.z, 1.0};

	// Capacidade de brilho do material
	GLfloat especularidade[4]={1.0,1.0,1.0,1.0}; 
	GLint especMaterial = 60;

	// Define a reflet�ncia do material 
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	// Define os parametros da luz de numero 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz ); 

	// Luz spot (farol)
	glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente); 
	glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT1, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuzSpot );   
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direcaoLuzSpot ); 
}

void desenharMundo() {

	arena.desenhar(texturaCeu, texturaChao, texturaArvore);

	pista.desenhar(texturaPista);

	for(Base b: bases) {
		b.desenharModeloBase(base, modeloBase, texturaBase);			
	}

	for(Bomba bomb: bombas) {
		bomb.desenharModeloBomba(bomba, modeloBomba);			
	}

	for(Bala bala: balas) {
		bala.desenhar();		
	}

	for(Bala balaInimiga: balasInimigas)
	{
		balaInimiga.desenharBalaInimiga();		
	}

	jogador.desenharModeloAviao(aviaoJogador, modeloAviaoJogador, helice, modeloHelice, texturaJogador);

	for(Inimigo inimigo: inimigos) {
		inimigo.desenharModeloAviao(aviaoJogador, modeloAviaoJogador, helice, modeloHelice, texturaInimigo);			
	}
}

void desenharViewport1() {
	// Se tiver bomba ativa
	if(bombas.size() != 0) {
		glViewport(0, (GLsizei)alturaJanela, (GLsizei)larguraJanela ,200);
		glLoadIdentity();
		cameraBomba->changeCamera(45,larguraJanela,200);	
		cameraBomba->record();
		desenharMundo();
	}
}

void desenharViewport2() {

	glViewport(0, 0, (GLsizei)larguraJanela, (GLsizei)alturaJanela);
	glLoadIdentity();

	if (cam6) {
		camera3pInimigo->changeCamera(45,larguraJanela,alturaJanela);
		camera3pInimigo->record();
	}
	if (cam5) {
		camera3pBase->changeCamera(45,larguraJanela,alturaJanela);
		camera3pBase->record();
	}
	if (cam3) {
		camera3pJogador->changeCamera(45,larguraJanela,alturaJanela);
		camera3pJogador->record();
	}
	if (cam2) {
		cameraCanhao->changeCamera(45,larguraJanela,alturaJanela);
		cameraCanhao->record();
	}
	if (cam1) {
		camera1pJogador->changeCamera(45,larguraJanela,alturaJanela);
		camera1pJogador->record();
	}
	if (desenhar_cameras) {
		cameraCanhao->draw();
		camera1pJogador->draw();
	}

	desenharMundo();

	minimapa.desenhar(arena, jogador, inimigos, bases, larguraJanela, alturaJanela);

	placar.desenhar(alturaJanela, larguraJanela);

	if(estado == 3)
		placar.desenharMensagemFinal(alturaJanela, larguraJanela);
}

/*
 * CALLBACK'S
 */

void display(void) {

	glClearColor (0.0, 0.0, 0.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	especificarIluminacao();
	
	desenharViewport1();

	desenharViewport2();

	glutSwapBuffers();
}

void idle(void) {

	// Calcula a diferença de tempo
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	timeDiference = currentTime - previousTime;
	previousTime = currentTime;

	distancia_camera = incremento_zoom*timeDiference*0.1;
  	incremento_zoom = 0;

	// cam6
	if(inimigos.size() != 0){ 
		camera3pInimigo->setDist(distancia_camera);
		camera3pInimigo->update(inimigos.at(0).x,inimigos.at(0).y,inimigos.at(0).z,cam6_angulo_xz,cam6_angulo_xy);
	}
	// cam5
	if(bases.size() != 0){ 
		camera3pBase->setDist(distancia_camera);
		camera3pBase->update(bases.at(0).x,bases.at(0).y,0,cam5_angulo_xz,cam5_angulo_xy);
	}
	// cam4
	if(bombas.size() != 0){ 
		cameraBomba->update(
			bombas.at(0).x, bombas.at(0).y, bombas.at(0).z,
	 		bombas.at(0).x, bombas.at(0).y, bombas.at(0).z-10, 
	 		cos(grausParaRadianos(bombas.at(0).angulo_xy)), sin(grausParaRadianos(bombas.at(0).angulo_xy)), 0.0
		);
	}
	// cam3
	camera3pJogador->setDist(distancia_camera);
	camera3pJogador->update(jogador.x,jogador.y,jogador.z,cam3_angulo_xz,cam3_angulo_xy);
	// cam2
	cameraCanhao->update(
		jogador.x, 
		jogador.y, 
		jogador.z,
		0.14*jogador.r, 
		-jogador.angulo_canhao_xz, 
		jogador.angulo_canhao_xy, 
		jogador.angulo_xy
	);
	// cam1
	camera1pJogador->update(jogador.x,jogador.y,jogador.z,jogador.angulo_xy,jogador.angulo_xz);

	// Decolando
	if(estado == 1){

		float distancia_jogador_fim_pista = sqrt(pow(jogador.y-pista.y1,2) + pow(jogador.x-pista.x1,2));
		
		jogador.velocidade += (pista.comprimento/8) * (timeDiference/1000);

		jogador.girarHelices(timeDiference/1000);
		jogador.andar(timeDiference/1000, arena.altura);

		if(distancia_jogador_fim_pista >= pista.comprimento/2)
			jogador.alterarAnguloXZ((timeDiference/1000) * 0.5);			

		if(distancia_jogador_fim_pista >= pista.comprimento)
			estado = 2;
	}

	// Jogando
	else if(estado == 2){

		atualizarEstadoJogador();

		atualizarEstadoTirosInimigos();

		atualizarEstadoTirosJogador();

		atualizarEstadoBombas();

		criarTirosInimigos();		

		jogador.curvando = false;

		if(teclas['a'] == 1 || teclas['A'] == 1) {
			jogador.alterarAnguloXY(+1 * timeDiference/1000);

			jogador.curvando = true;
			jogador.alterarAnguloInclinacao(1 * timeDiference/1000);
		}
		if(teclas['d'] == 1 || teclas['D'] == 1) {
			jogador.alterarAnguloXY(-1 * timeDiference/1000);

			jogador.curvando = true;
			jogador.alterarAnguloInclinacao(-1 * timeDiference/1000);
		}
		if(teclas['w'] == 1 || teclas['W'] == 1) 
		{
			jogador.alterarAnguloXZ(+1 * timeDiference/1000);
		}
		if(teclas['s'] == 1 || teclas['S'] == 1) 
		{
			jogador.alterarAnguloXZ(-1 * timeDiference/1000);
		}

		//Verificar se o piloto chegou ao final da arena
		if(verificarLimiteArena(jogador)) {
			teletransportarJogador();
		}

		if(verificarColisaoInimigo()) {
			estado = 3;
		}
	}

	// Início, decolando ou jogando
	if(estado != 3) {
		atualizarEstadoInimigos();
	}
	
	// Marca a janela atual como precisando ser reexibida
	glutPostRedisplay();
}

void keyPress(unsigned char key, int x, int y) {
	teclas[key] = 1;
	switch (key)
	{
		case 'U':
		case 'u':
			estado = (estado == 0) ? 1 : estado;
			break;
		case '+':
			jogador.velocidade += (estado == 2) ? 1 : 0;
			break;
		case '-':
			jogador.velocidade -= (estado == 2) ? ((jogador.velocidade > 1) ? 1 : 0) : 0;
			break;
		case 'R':
		case 'r':
		{
			reiniciarJogo();
			break;
		}
		case '9':
			desenhar_cameras = !desenhar_cameras;
			break;
		case 'N':
		case 'n':
			if(glIsEnabled(GL_LIGHT0)){ glDisable(GL_LIGHT0); }else{ glEnable(GL_LIGHT0); }
			break;
		case 'M':
		case 'm':
			if(glIsEnabled(GL_LIGHT1)){ glDisable(GL_LIGHT1); }else{ glEnable(GL_LIGHT1); }
			break;
		default:
			break;  	
	}

	if(key == '1'){
        cam1 = true;
        cam2 = false;
        cam3 = false;
		cam5 = false;
		cam6 = false;
    }
    if(key == '2'){
        cam1 = false;
		cam2 = true;
        cam3 = false;
		cam5 = false;
		cam6 = false;
    }
    if(key == '3'){
        cam1 = false;
        cam2 = false;
		cam3 = true;
		cam5 = false;
		cam6 = false;
    }
	if(key == '5'){
		if(bases.size() != 0) {
			cam1 = false;
			cam2 = false;
			cam3 = false;
			cam5 = true;
			cam6 = false;
		} else {
			cam1 = true;
			cam2 = false;
			cam3 = false;
			cam5 = false;
			cam6 = false;
		}
    }
	if(key == '6'){
		if(inimigos.size() != 0) {
			cam1 = false;
			cam2 = false;
			cam3 = false;
			cam5 = false;
			cam6 = true;
		} else {
			cam1 = true;
			cam2 = false;
			cam3 = false;
			cam5 = false;
			cam6 = false;
		}
    }
}

void keyUp(unsigned char key, int x, int y) {
	teclas[key] = 0;
}

void passiveMotion(int x, int y) {	
	if (estado != 3){
		if(x > mouse_ultima_posicao_x)
			jogador.alterarAnguloCanhaoXY(-3);
		if(x < mouse_ultima_posicao_x)
			jogador.alterarAnguloCanhaoXY(3);

		mouse_ultima_posicao_x = x;

		if(y > mouse_ultima_posicao_y)
			jogador.alterarAnguloCanhaoXZ(-3);
		if(y < mouse_ultima_posicao_y)
			jogador.alterarAnguloCanhaoXZ(3);

		mouse_ultima_posicao_y = y;
	}
}

void motion(int x, int y) {
	
	if(botao_direito_status){

      if(cam3 && (teclas['E'] == 1 || teclas['e'] == 1)){
          cam3_angulo_xy -= x - mouse_ultima_posicao_x_motion;
          cam3_angulo_xz += y - mouse_ultima_posicao_y_motion;

          cam3_angulo_xy = (int)cam3_angulo_xy % 360;
          cam3_angulo_xz = (int)cam3_angulo_xz % 360;

          mouse_ultima_posicao_x_motion = x;
          mouse_ultima_posicao_y_motion = y;
      }

	  if(cam5 && (teclas['E'] == 1 || teclas['e'] == 1)){
          cam5_angulo_xy -= x - mouse_ultima_posicao_x_motion;
          cam5_angulo_xz += y - mouse_ultima_posicao_y_motion;

          cam5_angulo_xy = (int)cam5_angulo_xy % 360;
          cam5_angulo_xz = (int)cam5_angulo_xz % 360;

          mouse_ultima_posicao_x_motion = x;
          mouse_ultima_posicao_y_motion = y;
      }

	  if(cam6 && (teclas['E'] == 1 || teclas['e'] == 1)){
          cam6_angulo_xy -= x - mouse_ultima_posicao_x_motion;
          cam6_angulo_xz += y - mouse_ultima_posicao_y_motion;

          cam6_angulo_xy = (int)cam6_angulo_xy % 360;
          cam6_angulo_xz = (int)cam6_angulo_xz % 360;

          mouse_ultima_posicao_x_motion = x;
          mouse_ultima_posicao_y_motion = y;
      }
  }
}

void mouse(int button, int state, int x, int y) {

	//Verifica se o botão esquerdo está pressionado
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && estado == 2)	{
		//Atirar
		Bala bala = Bala(
			jogador.x, 
			jogador.y,
			jogador.z,
			jogador.r,
			jogador.angulo_xy,
			jogador.angulo_xz,
			jogador.angulo_canhao_xy,  
			jogador.angulo_canhao_xz,
			jogador.angulo_canhao_arena_xy,
			jogador.angulo_canhao_arena_xz,
			jogador.angulo_inclinacao,
			jogador.curvando,
			configuracao.jogador_velocidade_tiro*jogador.velocidade
		);

		balas.push_back(bala);
		
	}
	if(button == 2 && state == 0 && estado == 2 && !(teclas['E'] == 1 || teclas['e'] == 1)) {
		if(bombas.size() == 0) {
			Bomba bomba = Bomba(
				jogador.x, 
				jogador.y,
				jogador.z,
				jogador.r,
				jogador.angulo_xy,
				90,
				jogador.velocidade
			);
			bombas.push_back(bomba);
		}
	}

	if (button == GLUT_RIGHT_BUTTON) {
        if(state == GLUT_DOWN){
            mouse_ultima_posicao_x_motion = x;
            mouse_ultima_posicao_y_motion = y;
			botao_direito_status = true;
        } else {
            botao_direito_status = false;
        }
    }

	incremento_zoom = 0;

	if(button == 4){
		incremento_zoom = -1;
	}
	if(button == 3){
		incremento_zoom = 1;
	}
}

void reshape(GLsizei w, GLsizei h) {
	// Para previnir uma divis�o por zero
	if ( h == 0 ) h = 1;

	camera3pJogador->changeCamera(60, w, h);

	// Especifica o tamanho da viewport
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);

	larguraJanela = w;
	alturaJanela = h - 200;	
}

/*
 * FUNÇÃO PRINCIPAL
 */

int main(int argc, char** argv) {

	glutInit (&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(larguraJanela, alturaJanela + 200);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-larguraJanela)/2,(glutGet(GLUT_SCREEN_HEIGHT)-alturaJanela)/2);
	glutCreateWindow("TRABALHO FINAL");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutPassiveMotionFunc(passiveMotion);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);

	if (!inicializarObjetosJogo(argv[1]))
		return EXIT_FAILURE;
	
	inicializarOpengl();
	glutMainLoop();

	return EXIT_SUCCESS;
}
