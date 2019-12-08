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

#include "Camera3dPerson.h"
#include "Camera3dBase.h"
#include "CameraCannon.h"
#include "Camera1stPerson.h"
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
Jogador jogador;
Jogador jogador_copia;
Pista pista;
Arena arena;
Placar placar;
Minimapa minimapa;
int estado = 0;

bool curvando = false;
bool rButtonDown = false;
int scrollUp = 0;

/*
 * VARIÁVEIS PARA AJUSTE DO TEMPO
 */
static GLdouble previousTime = 0;
GLdouble currentTime;
GLdouble timeDiference;

/*
 * VARIÁVEIS DA CÂMERA
 */
// int camera = 1;
// GLfloat anguloCamera=45, fAspect;
// GLfloat obsX=0, obsY=-10, obsZ=1000;
// GLfloat eyeX=0, eyeY=0, eyeZ=0;
// GLfloat upX=0, upY=0, upZ=1;
// float anguloCameraJogadorXY=0;
// int ultimaPosicaoMouseCameraX=0, ultimaPosicaoMouseCameraY=0;
// bool movimentarCamera3 = false;
int mouse_ultima_posicao_x = 0;
int mouse_ultima_posicao_y = 0;

//Camera controls
int toggleCam = 0;
int camAngle = 60;
int lastX = 0;
int lastY = 0;

double camZAngle;
double camYAngle;
double camZAngleBase;
double camYAngleBase;

bool cam1 = true;
bool cam2 = false;
bool cam3 = false;
bool cam4 = false;
bool cam5 = false;

double camDist = 0;

Camera3dPerson* camera;
Camera3dBase* camera3dBase;
CameraCannon* cameraCannon;
Camera1stPerson* camera1stPerson;
CameraBomba* cameraBomba;

bool verificarLimiteArena(Jogador jogador) {	
	//Calcula a distância do piloto até a borda da arena
	float distancia = sqrt(pow(jogador.x - arena.x, 2.0) + pow(jogador.y - arena.y, 2.0));

	//Verifica se o piloto está fora da arena
	if(distancia >= arena.r)
	{
		return true;
	}
	else
	{
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
	}else{
		jogador.x = x1;
		jogador.y = y1;
	}

}

//Realiza o teletransporte para o lado oposto da arena
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
	// tiros_inimigos.clear();
	bombas.clear();
	balas.clear();
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

void atualizarEstadoTirosJogador() 
{
	for(auto it = balas.begin(); it != balas.end();)
	{
		Bala &bala = *it;
		
		//Mover as balas em linha reta
		bala.mover(timeDiference/1000);
		
		//Verificar se as balas sairam da arena
//		if(verificarLimiteArtilharia(bala.getX(), bala.getY(), bala.getRaio()/5))
//		{
//			it = balas.erase(it);		
//		}		
		//Verificar se acertou o inimigo
//		else if(derrubarInimigo(bala.getX(), bala.getY(), bala.getRaio()/5))
//		{
//			it = balas.erase(it);			
//		}
//		else
//		{	
			++it;
//		}	
	}
}

void atualizarEstadoTirosInimigos() {
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

	// glDepthFunc(GL_LEQUAL);

	//	carregouAviaoJogador = aviaoJogador.LoadFile("Modelos/piper_pa18.obj");
	
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

		camera = new Camera3dPerson(-3*2*jogador.r,0,0, //Position
                                  0,0,0,        //look point
                                  0,0,1        //up vector
                                );
		camera->setZAngle(jogador.angulo_xy);

		cameraCannon = new CameraCannon(jogador.r*0.9*cos(grausParaRadianos(jogador.angulo_xy)),jogador.r*0.9*sin(grausParaRadianos(jogador.angulo_xy)),7,
										0,0,0,
										0,0,1
									);

		camera1stPerson = new Camera1stPerson(jogador.r,0,11,
											0,0,0,
											0,0,1
											);

		cameraBomba = new CameraBomba(jogador.x,jogador.y,jogador.z,
											0,0,0,
											0,0,1
											);

		camera3dBase = new Camera3dBase(bases[0].x,bases[0].y,0,
										0,0,0,
										0,0,1
										);

		camZAngle = camera->getZAngle();
    	camYAngle = camera->getYAngle();

	return true;
}


/*
 * CÂMERAS, LUZES E DESENHO
 */

// void posicionarObservador(GLfloat obsX, GLfloat obsY, GLfloat obsZ, GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, GLfloat upX, GLfloat upY, GLfloat upZ) {
// 	// Especifica sistema de coordenadas do modelo
// 	glMatrixMode(GL_MODELVIEW);
// 	// Inicializa sistema de coordenadas do modelo
// 	glLoadIdentity();
// 	// Especifica posição do observador e do alvo
//     gluLookAt(obsX, obsY, obsZ, eyeX, eyeY, eyeZ, upX, upY, upZ);
// }

// void especificarParametrosVisualizacao(GLfloat angle, GLfloat fAspectW, GLfloat fAspectH, GLfloat zMin, GLfloat zMax) {
// 	// Especifica sistema de coordenadas de projeção
// 	glMatrixMode(GL_PROJECTION);
// 	// Inicializa sistema de coordenadas de projeção
// 	glLoadIdentity();
// 	// Especifica a projeção perspectiva(angulo,aspecto,zMin,zMax)
// 	gluPerspective(angle, (GLfloat)fAspectW / (GLfloat)fAspectH, zMin, zMax);
// }

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
	// Define a concentra��o do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	// Ativa o uso da luz ambiente 
	// glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os par�metros da luz de n�mero 0
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

// Vamos retirar depois
void DrawAxes() {
    GLfloat mat_ambient_r[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient_g[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat mat_ambient_b[] = { 0.0, 0.0, 1.0, 1.0 };

    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
 
        //x axis
        glPushMatrix();
            glColor3fv(mat_ambient_r);
            glScalef(5, 0.1, 0.1);
            glTranslatef(0, 0, 0); // put in one end
            glutSolidCube(20.0);
        glPopMatrix();

        //y axis
        glPushMatrix();
            glColor3fv(mat_ambient_g);
            glScalef(0.1, 5, 0.1);
            glTranslatef(0, 0, 0); // put in one end
            glutSolidCube(20.0);
        glPopMatrix();

        //z axis
        glPushMatrix();
            glColor3fv(mat_ambient_b);
            glScalef(0.1, 0.1, 5);
            glTranslatef(0, 0, 0); // put in one end
            glutSolidCube(20.0);
        glPopMatrix();
    glPopAttrib();
    
}


void desenharMundo() {
	
	

	// Desenha uma esfera na posição da luz
	// Vamos retirar isso depois
	glPushMatrix();
		glTranslatef(0, 0, 50);
		glColor3f(1.0f, 1.0f, 0.0f);
		glutSolidSphere (10.0, 50, 50);
		glColor3f(0.0f, 0.0f, 1.0f);
	glPopMatrix();

	
	DrawAxes();

	arena.desenhar(texturaCeu, texturaChao, texturaArvore);

	pista.desenhar(texturaPista);

	for(Base b: bases)
	{
		b.desenharModeloBase(base, modeloBase, texturaBase);			
	}

	for(Bomba bomb: bombas)
	{
		bomb.desenharModeloBomba(bomba, modeloBomba);			
	}

	for(Bala bala: balas)
	{
		bala.desenhar();		
	}


	//	jogador.desenhar();

	jogador.desenharModeloAviao(aviaoJogador, modeloAviaoJogador, helice, modeloHelice, texturaJogador);

	//Desenha os inimigos aereos
	for(Inimigo inimigo: inimigos)
	{
		inimigo.desenharModeloAviao(aviaoJogador, modeloAviaoJogador, helice, modeloHelice, texturaInimigo);			
	}

	//	 glPushMatrix();
		// 	glColor3f(0.0f, 0.0f, 1.0f);
	//	 	glTranslatef(jogador.x, jogador.y, jogador.z);
			
	//		glRotatef(-90, 0, 0, 1);
		
	//		glRotatef(jogador.angulo_xy, 0, 0, 1);

	//	 	desenharAeromodelo();
	//	 glPopMatrix();
	
	
}

void desenharViewport1() {
	// Se tiver bomba ativa
	if(bombas.size() != 0) {
		glViewport(0, (GLsizei)alturaJanela, (GLsizei)larguraJanela ,200);
		glLoadIdentity();
		camera->changeCamera(45,larguraJanela,200);	
		// especificarParametrosVisualizacao(anguloCamera, larguraJanela, 200, 40, 5000.0);
		// posicionarObservador(
		// 	bombas.at(0).x, bombas.at(0).y, bombas.at(0).z,
		//  	bombas.at(0).x, bombas.at(0).y, bombas.at(0).z-10, 
		//  	cos(grausParaRadianos(bombas.at(0).angulo_xy)), sin(grausParaRadianos(bombas.at(0).angulo_xy)), 0.0
		// );
		cameraBomba->record();
		desenharMundo();
	}
}

void desenharViewport2() {

	glViewport(0, 0, (GLsizei)larguraJanela, (GLsizei)alturaJanela);
	glLoadIdentity();
  	camera->changeCamera(45,larguraJanela,alturaJanela);

	// desenharMiniMapa();
	

	// if(camera == 0){
	// 	// Padrão
	// 	especificarParametrosVisualizacao(anguloCamera, larguraJanela, alturaJanela, 0.1, 5000.0);
	// 	posicionarObservador(obsX, obsY, obsZ, eyeX, eyeY, eyeZ, upX, upY, upZ);
	// }
	// if(camera == 1){
	// 	// Cokpit
	// 	especificarParametrosVisualizacao(anguloCamera, larguraJanela, alturaJanela, 5, 5000.0);
	// 	posicionarObservador(
	// 		jogador.x + (jogador.r/2 - 2) * cos(grausParaRadianos(jogador.angulo_xy)), 
	// 		jogador.y + (jogador.r/2 - 2) * sin(grausParaRadianos(jogador.angulo_xy)), 
	// 		jogador.z + jogador.r/2,
	// 		jogador.x + (jogador.r + 1) * cos(grausParaRadianos(jogador.angulo_xy)), 
	// 		jogador.y + (jogador.r + 1) * sin(grausParaRadianos(jogador.angulo_xy)), 
	// 		jogador.z + jogador.r/2, 
	// 		0, 0, 1);
	// }
	// if(camera == 2){
	// 	// Canhão
	// 	especificarParametrosVisualizacao(anguloCamera, larguraJanela, alturaJanela, 0.1, 5000.0);
	// 	posicionarObservador(
	// 		jogador.x + jogador.r * cos(grausParaRadianos(jogador.angulo_xy)), 
	// 		jogador.y + jogador.r * sin(grausParaRadianos(jogador.angulo_xy)), 
	// 		jogador.z+2,
	// 		jogador.x + jogador.r * cos(grausParaRadianos(jogador.angulo_xy)) + (0.5*jogador.r) * cos(grausParaRadianos(jogador.angulo_canhao_arena_xy)), 
	// 		jogador.y + jogador.r * sin(grausParaRadianos(jogador.angulo_xy)) + (0.5*jogador.r) * sin(grausParaRadianos(jogador.angulo_canhao_arena_xy)), 
	// 		jogador.z + (0.5*jogador.r) * sin(grausParaRadianos(jogador.angulo_canhao_arena_xz)), 
	// 		0, 0, 1);
	// }
	// if(camera == 3){
	// 	// 3a pessoa
	// 	especificarParametrosVisualizacao(anguloCamera, larguraJanela, alturaJanela, 0.1, 5000.0);
	// 	posicionarObservador(
	// 		jogador.x - 50*cos(grausParaRadianos(anguloCameraJogadorXY)), 
	// 		jogador.y - 50*sin(grausParaRadianos(anguloCameraJogadorXY)), 
	// 		jogador.z + 40,
	// 		jogador.x, jogador.y, jogador.z+jogador.r, 0, 0, 1);
	// } 

	if(cam5){
		camera3dBase->record();
	}
	if(cam3){
		camera->record();
	}
	if(cam2){
		cameraCannon->record();
	}
	if(cam1){
		camera1stPerson->record();
	}

	cameraCannon->draw();
	camera1stPerson->draw();


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

	camDist = scrollUp*timeDiference*0.1;
  	scrollUp = 0;

	// cam5
	if(bases.size() != 0){ 
		camera3dBase->setDist(camDist);
		camera3dBase->update(bases.at(0).x,bases.at(0).y,0,camYAngleBase,camZAngleBase);
	}
	// cam4
	if(bombas.size() != 0){ 
		cameraBomba->update(
			bombas.at(0).x, bombas.at(0).y, bombas.at(0).z,
	 		bombas.at(0).x, bombas.at(0).y, bombas.at(0).z-10, 
	 		cos(grausParaRadianos(bombas.at(0).angulo_xy)), sin(grausParaRadianos(bombas.at(0).angulo_xy)), 0.0
		);
	}
	// if(cam3){
		camera->setDist(camDist);
		camera->update(jogador.x,jogador.y,jogador.z,camYAngle,camZAngle);
	// }
	// if(cam2){
		cameraCannon->update(jogador.x,jogador.y,jogador.z,jogador.z+0.14*jogador.r, -jogador.angulo_canhao_xz,jogador.angulo_canhao_xy, jogador.angulo_xy);
	// }
	// if(cam1){
		camera1stPerson->update(jogador.x,jogador.y,jogador.z,jogador.angulo_xy,jogador.angulo_xz);
	// }

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
		if(verificarLimiteArena(jogador))
		{
			teletransportarJogador();
		}

		if(verificarColisaoInimigo())
		{
			cout << "Colidiu com inimigo" << endl;
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
		// case 'I':
		// case 'i':
		// {
		// 	int inc = anguloCamera <= 5 ? 0 : 1;
		// 	anguloCamera -= inc;
		// 	break;
		// }
		// case 'O':
		// case 'o':
		// {
		// 	int inc = anguloCamera >= 180 ? 0 : 1;
		// 	anguloCamera += inc;
		// 	break;
		// }
		case 'R':
		case 'r':
		{
			reiniciarJogo();
		}
		// case '0':
		// 	camera = 0;
		// 	break;
		// case '1':
		// 	camera = 1;
		// 	break;
		// case '2':
		// 	camera = 2;
		// 	break;
		// case '3':
		// 	camera = 3;
		// 	break;
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
    }
    if(key == '2'){
        cam1 = false;
		cam2 = true;
        cam3 = false;
		cam5 = false;
    }
    if(key == '3'){
        cam1 = false;
        cam2 = false;
		cam3 = true;
		cam5 = false;
    }
	if(key == '5'){
		if(bases.size() != 0) {
			cam1 = false;
			cam2 = false;
			cam3 = false;
			cam5 = true;
		} else {
			cam1 = true;
			cam2 = false;
			cam3 = false;
			cam5 = false;
		}
    }
}

void keyUp(unsigned char key, int x, int y) {
	teclas[key] = 0;
}

void specialKeys(int key, int x, int y) {
	// switch (key) {
	// 	case GLUT_KEY_LEFT:
	// 		obsX -=10;
	// 		break;
	// 	case GLUT_KEY_RIGHT: 
	// 		obsX +=10;
	// 		break;
	// 	case GLUT_KEY_UP: 
	// 		obsY +=10;
	// 		break;
	// 	case GLUT_KEY_DOWN: 
	// 		obsY -=10;
	// 		break;
	// 	case GLUT_KEY_HOME : 
	// 		obsZ +=10;
	// 		break;
	// 	case GLUT_KEY_END : 
	// 		obsZ -=10;
	// 		break;
	// }
}

//Controla o canhão
void passiveMotion(int x, int y) {	
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

void motion(int x, int y) {
	// if (movimentarCamera3) {
	// 	if(x > ultimaPosicaoMouseCameraX)
	// 		anguloCameraJogadorXY -= 3;
	// 	if(x < ultimaPosicaoMouseCameraX)
	// 		anguloCameraJogadorXY += 3;

	// 	ultimaPosicaoMouseCameraX = x;
	// }

	if(rButtonDown){
      if(cam3 && (teclas['E'] == 1 || teclas['e'] == 1)){
          camZAngle -= x - lastX;
          camYAngle += y - lastY;

          camZAngle = (int)camZAngle % 360;
          camYAngle = (int)camYAngle % 360;

          lastX = x;
          lastY = y;
      }
  }
}

void mouse(int button, int state, int x, int y) {
	//Verifica se o botão esquerdo está pressionado
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && estado == 2)
	{
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

	// Seta flag que permite movimento da câmera 3
	// if(button == 0 && state == 0 && (teclas['b'] == 1 || teclas['B'] == 1)) {
	// 	movimentarCamera3 = true;
	// }
	// if(button == 0 && state == 1) {
	// 	movimentarCamera3 = false;
	// }

	if (button == GLUT_RIGHT_BUTTON) {
        if(state == GLUT_DOWN){
            lastX = x;
            lastY = y;
			rButtonDown = true;
        }else{
            rButtonDown = false;
        }

    }

	scrollUp = 0;

	if(button == 4){
		scrollUp = -1;
	}
	if(button == 3){
		scrollUp = 1;
	}
}

void reshape(GLsizei w, GLsizei h) {
	// Para previnir uma divis�o por zero
	if ( h == 0 ) h = 1;

	// Calcula a correcao de aspecto
	//	fAspect = (GLfloat)w/(GLfloat)h;

	camera->changeCamera(camAngle, w, h);

	// Especifica o tamanho da viewport
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);

	// especificarParametrosVisualizacao(anguloCamera, w, h, 0.1, 500.0);

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
	glutInitWindowPosition(0, 0); // Deixar assim por enquanto... para a janela não abrir em cma do código
	// glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-larguraJanela)/2,(glutGet(GLUT_SCREEN_HEIGHT)-alturaJanela)/2);
	glutCreateWindow("TRABALHO FINAL");
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
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
