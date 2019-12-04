#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <math.h>

#include "tinyxml2.h"
#include "configuracao.h"
#include "pista.h"
#include "arena.h"
#include "inimigo.h"
#include "base.h"
#include "bomba.h"
#include "jogador.h"
#include "placar.h"
//#include "OBJ_Loader.h"
#include "imageloader.h"
#include "minimapa.h"
#include "lerOBJ.h"



#define grausParaRadianos(g) g*(M_PI/180)
#define radianosParaGraus(r) r*(180/M_PI)


using namespace tinyxml2;
using namespace std;
//using namespace objl;



/*
 * DIMENSÕES DO JOGO
 */
int larguraJanela = 500;
int alturaJanela = 500;

/*
 * TEXTURAS DO JOGO
 */
GLuint texturaCeu;

/*
 * MINI MAPA
 */
Minimapa minimapa;

/*
 * MODELOS IMPORTADOS NO JOGO
 */
//Loader aviaoJogador;
//bool carregouAviaoJogador = false;

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

//Avião dos inimigos




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
Jogador jogador;
Jogador jogador_copia;
Pista pista;
Arena arena;
Placar placar;
int estado = 0;

int controladorCanhaoX = 0;
int controladorCanhaoZ = 0;

bool curvando = false;

// int basesDestruidas = 0;
// int basesRestantes = 0;

/*
 * VARIÁVEIS PARA AJUSTE DO TEMPO
 */
static GLdouble previousTime = 0;
GLdouble currentTime;
GLdouble timeDiference;

/*
 * VARIÁVEIS DA CÂMERA
 */
int camera = 1;
GLfloat anguloCamera=45, fAspect;
GLfloat obsX=0, obsY=-10, obsZ=1000;
GLfloat eyeX=0, eyeY=0, eyeZ=0;
GLfloat upX=0, upY=0, upZ=1;
float anguloCameraJogadorXY=0;
int ultimaPosicaoMouseCameraX=0, ultimaPosicaoMouseCameraY=0;
bool movimentarCamera3 = false;
int mouse_ultima_posicao_x = 0;
int mouse_ultima_posicao_y = 0;



bool verificarLimiteArena(Jogador jogador)
{	//Calcula a distância do piloto até a borda da arena
	float distancia = sqrt(pow(jogador.x - arena.x, 2.0) + pow(jogador.y - arena.y, 2.0));

//	cout << "Distãncia: " << distancia << endl;

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

bool verificarColisaoInimigo()
{
	bool explodiu = false;
	
	//Percorre todos os inimigos aéreos
	for(size_t i = 0; i < inimigos.size(); i++)
	{
		//Calcula a distância do piloto até o inimigo
		float distancia = sqrt(pow(jogador.x - inimigos[i].x, 2.0) + pow(jogador.y - inimigos[i].y, 2.0));

		//Verifica se houve colisão
		if(distancia < inimigos[i].r + jogador.r)
		{
			explodiu = true;
			//indiceInimigo = i;
			
			//Finaliza o jogo para poder ser reiniciado
//			decolou = false;
//			motorLigado = false;
//			jogando = false;
			
			break;
		}
	}

	return explodiu;
}

//Realiza o teletransporte para o lado oposto da arena
void teletransportarJogador(float angulo)
{
	angulo = angulo * M_PI / 180.0;

	//Coeficiente angular
	float m = 0.0;
	
	//Ajuste caso o ângulo do avião seja 90 graus
	if(angulo >= 1.57 && angulo <= 1.58)
	{
		m = tanf(90.1 * M_PI / 180.0);
	}
	else if(angulo <= -1.57 && angulo >= -1.58)
	{
		m = tanf(-90.1 * M_PI / 180.0);
	}
	else
	{ 
		m = tanf(angulo);
	}

	//pilotoY - arenaY
	float E = jogador.y - arena.y;	
	
	float coeficienteA = 1 + m * m;
	float coeficienteB = -2 * arena.x -2 * m * m * jogador.x + 2 * E * m;
	float coeficienteC = E * E - 2 * E * m * jogador.x + m * m * jogador.x * jogador.x + arena.x * arena.x - arena.r * arena.r;

	float x1 = (-coeficienteB + sqrt(coeficienteB * coeficienteB - 4 * coeficienteA * coeficienteC))/(2 * coeficienteA);
	float x2 = (-coeficienteB - sqrt(coeficienteB * coeficienteB - 4 * coeficienteA * coeficienteC))/(2 * coeficienteA);

	float y1 = jogador.y + m * (x1 - jogador.x);
	float y2 = jogador.y + m * (x2 - jogador.x);

	float dist1 = sqrt( powf(x1 - jogador.x, 2) + powf(y1 - jogador.y, 2) );
	float dist2 = sqrt( powf(x2 - jogador.x, 2) + powf(y2 - jogador.y, 2) ); 

	if(dist1 < dist2)
	{
		jogador.x = x2;
		jogador.y = y2;
	}
	else
	{
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
	// tiros.clear();
	// tiros_inimigos.clear();
	bombas.clear();
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

void atualizarEstadoTirosJogador() {
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


//Carregar texturas
GLuint LoadTextureRAW( const char * filename )
{
	GLuint texture;

	Image* image = loadBMP(filename);

	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
		                   0,                            //0 for now
		                   GL_RGB,                       //Format OpenGL uses for image
		                   image->width, image->height,  //Width and height
		                   0,                            //The border of the image
		                   GL_RGB, //GL_RGB, because pixels are stored in RGB format
		                   GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
		                                     //as unsigned numbers
		                   image->pixels);               //The actual pixel data
	delete image;

	return texture;
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

	//Carregar
	texturaCeu = LoadTextureRAW("Texturas/ceu.bmp");
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
				inimigos_copia.push_back(i);
			}
			if(!strcmp(fill,"orange")){
				Base b = Base(x, y, r);
				bases.push_back(b);
				bases_copia.push_back(b);
			}
			if(!strcmp(fill,"green")){
				Jogador j = Jogador(x, y, 0, r, 0);
				jogador = j;
				jogador_copia = j;
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
		anguloCameraJogadorXY = radianosParaGraus(angulo);

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

	return true;
}


/*
 * CÂMERAS, LUZES E DESENHO
 */

void posicionarObservador(GLfloat obsX, GLfloat obsY, GLfloat obsZ, GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, GLfloat upX, GLfloat upY, GLfloat upZ) {
	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();
	// Especifica posição do observador e do alvo
    gluLookAt(obsX, obsY, obsZ, eyeX, eyeY, eyeZ, upX, upY, upZ);
}

void especificarParametrosVisualizacao(GLfloat angle, GLfloat fAspectW, GLfloat fAspectH, GLfloat zMin, GLfloat zMax) {
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();
	// Especifica a projeção perspectiva(angulo,aspecto,zMin,zMax)
	gluPerspective(angle, (GLfloat)fAspectW / (GLfloat)fAspectH, zMin, zMax);
}

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

/*
void desenharAeromodelo()
{
	if(carregouAviaoJogador)
	{
		glPushMatrix();
			//glTranslatef(0,-0.5,0);
			//glRotatef(rodar, 0, 1, 0);
			

			for(size_t i = 0; i < aviaoJogador.LoadedMeshes.size(); i++)
			{
				Mesh curMesh = aviaoJogador.LoadedMeshes[i];
			
				GLfloat materialEmission[] = { 0.1, 0.1, 0.1, 1};
				GLfloat materialColorA[] = { curMesh.MeshMaterial.Ka.X, curMesh.MeshMaterial.Ka.Y, curMesh.MeshMaterial.Ka.Z, 1};
				GLfloat materialColorD[] = { curMesh.MeshMaterial.Kd.X, curMesh.MeshMaterial.Kd.Y, curMesh.MeshMaterial.Kd.Z, 1};
				GLfloat mat_specular[] = { curMesh.MeshMaterial.Ks.X, curMesh.MeshMaterial.Ks.Y, curMesh.MeshMaterial.Ks.Z, 1};
				GLfloat mat_shininess[] = { curMesh.MeshMaterial.Ns };
				glColor3f(1,1,1);

				glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
				glMaterialfv(GL_FRONT, GL_AMBIENT, materialColorA);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);
				glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
				glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
			
				
				glBindTexture (GL_TEXTURE_2D, texturaCeu);
				glBegin (GL_QUADS);
				for(size_t j = 0; j < curMesh.Vertices.size(); j++)
				{
				   glNormal3f(curMesh.Vertices[j].Normal.X, curMesh.Vertices[j].Normal.Y, curMesh.Vertices[j].Normal.Z);
				   glTexCoord2f (curMesh.Vertices[j].TextureCoordinate.X, curMesh.Vertices[j].TextureCoordinate.Y);
				   glVertex3f (curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z);
				}
				glEnd();
		    
		    }
		glPopMatrix();
	}
	else
	{
		cout << "ERRO: Aeromodelo não encontrado na pasta específica!" << endl;
		exit(EXIT_FAILURE);
	}

}
*/

// //Mensagem na tela
// void mensagem(void * font, string s, float x, float y, float z)
// {
//     glRasterPos3f(x, y, z);
	

//     for(size_t i = 0; i < s.length(); i++)
//     {
//         glutBitmapCharacter(font, s[i]);
//     }
// }


// //Exibe o placar das bombas
// void exibirPlacar()
// {
// 	glColor3f(0.0, 1.0, 0.0);
// 	mensagem(GLUT_BITMAP_HELVETICA_12, "BASES", larguraJanela - 80, alturaJanela - 20 , 0);
// 	glColor3f(1.0, 1.0, 1.0);
// 	mensagem(GLUT_BITMAP_HELVETICA_10, "DESTRUIDAS = ", larguraJanela - 110, alturaJanela - 40, 0);
// 	glColor3f(0.0, 1.0, 0.0);
// 	mensagem(GLUT_BITMAP_HELVETICA_10, to_string(basesDestruidas), larguraJanela - 25, alturaJanela - 40, 0);
// 	glColor3f(1.0, 1.0, 1.0);
// 	mensagem(GLUT_BITMAP_HELVETICA_10, "FALTAM = ", larguraJanela - 110, alturaJanela - 55, 0);
// 	glColor3f(0.0, 1.0, 0.0);
// 	mensagem(GLUT_BITMAP_HELVETICA_10, to_string(basesRestantes), larguraJanela - 25, alturaJanela - 55, 0);
// }

// void desenharMiniMapa()
// {
//  	glLoadIdentity();
	
// 	//Draw text considering a 2D space (disable all 3d features)
//     glMatrixMode (GL_PROJECTION);
//     //Push to recover original PROJECTION MATRIX
//     glPushMatrix();
//         glLoadIdentity ();
//         glOrtho (0, larguraJanela, 0, alturaJanela, -1, 1);
        
// 		 //Push to recover original attributes
// 		glPushAttrib(GL_ENABLE_BIT);
// 			glDisable(GL_LIGHTING);
// 			glDisable(GL_TEXTURE_2D);
			
// 			/*						
// 			/*						
// 			/*						
// 			/*						
// 			/*						
// 			/*						
// 			/*						
// 			//Draw text in the x, y, z position
// 			glColor3f(0,1,0);
// 			glRasterPos3f(arena.x, arena.y, 0);
// 			const char* tmpStr;
// 			tmpStr = "TESTE";
// 			while( *tmpStr ){
// 				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *tmpStr);
// 				tmpStr++;
// 			}

// 			glColor3f(0.0,1.0,0.0);
// 		  	glBegin(GL_TRIANGLES);                                          // início triângulo
// 				glVertex3f(70, 40, 0.0);                         // Topo
// 				glVertex3f(30, 40, 0.0);                          // Esquerda embaixo
// 				glVertex3f(70, 20, 0.0);                          // Direita embaixo
// 			glEnd();
// 			*/
// 			// exibirPlacar();

// 			// placar.desenhar(alturaJanela, larguraJanela);

// 			// if(estado == 3)
// 			// 	placar.desenharMensagemFinal(alturaJanela, larguraJanela);

// 			minimapa.desenhar(arena, jogador, inimigos, bases, larguraJanela, alturaJanela);


// 		glPopAttrib();

//     glPopMatrix();
//     glMatrixMode (GL_MODELVIEW);

	

// }

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

	arena.desenhar(texturaCeu);

	pista.desenhar();

	for(Base b: bases)
	{
		b.desenharModeloBase(base, modeloBase);			
	}

	for(Bomba bomb: bombas)
	{
		bomb.desenharModeloBomba(bomba, modeloBomba);			
	}


//	jogador.desenhar();

	jogador.desenharModeloAviao(aviaoJogador, modeloAviaoJogador, helice, modeloHelice);

	//Desenha os inimigos aereos
	for(Inimigo inimigo: inimigos)
	{
		inimigo.desenharModeloAviao(aviaoJogador, modeloAviaoJogador, helice, modeloHelice);			
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
		especificarParametrosVisualizacao(anguloCamera, larguraJanela, 200, 40, 5000.0);
		posicionarObservador(
			bombas.at(0).x, bombas.at(0).y, bombas.at(0).z,
		 	bombas.at(0).x, bombas.at(0).y, bombas.at(0).z-10, 
		 	cos(grausParaRadianos(bombas.at(0).angulo_xy)), sin(grausParaRadianos(bombas.at(0).angulo_xy)), 0.0
		);
		desenharMundo();
	}
}

void desenharViewport2() {

	glViewport(0, 0, (GLsizei)larguraJanela, (GLsizei)alturaJanela);
	
	// desenharMiniMapa();
	

	if(camera == 0){
		// Padrão
		especificarParametrosVisualizacao(anguloCamera, larguraJanela, alturaJanela, 0.1, 5000.0);
		posicionarObservador(obsX, obsY, obsZ, eyeX, eyeY, eyeZ, upX, upY, upZ);
	}
	if(camera == 1){
		// Cokpit
		especificarParametrosVisualizacao(anguloCamera, larguraJanela, alturaJanela, 5, 5000.0);
		posicionarObservador(
			jogador.x + jogador.r * cos(grausParaRadianos(jogador.angulo_xy)), 
			jogador.y + jogador.r * sin(grausParaRadianos(jogador.angulo_xy)), 
			jogador.z,
			jogador.x + (jogador.r + 1) * cos(grausParaRadianos(jogador.angulo_xy)), 
			jogador.y + (jogador.r + 1) * sin(grausParaRadianos(jogador.angulo_xy)), 
			jogador.z, 
			0, 0, 1);
	}
	if(camera == 2){
		// Canhão
		especificarParametrosVisualizacao(anguloCamera, larguraJanela, alturaJanela, 0.1, 5000.0);
		posicionarObservador(
			jogador.x + jogador.r * cos(grausParaRadianos(jogador.angulo_xy)), 
			jogador.y + jogador.r * sin(grausParaRadianos(jogador.angulo_xy)), 
			jogador.z+2,
			jogador.x + jogador.r * cos(grausParaRadianos(jogador.angulo_xy)) + (0.5*jogador.r) * cos(grausParaRadianos(jogador.angulo_canhao_arena_xy)), 
			jogador.y + jogador.r * sin(grausParaRadianos(jogador.angulo_xy)) + (0.5*jogador.r) * sin(grausParaRadianos(jogador.angulo_canhao_arena_xy)), 
			jogador.z + (0.5*jogador.r) * sin(grausParaRadianos(jogador.angulo_canhao_arena_xz)), 
			0, 0, 1);
	}
	if(camera == 3){
		// 3a pessoa
		especificarParametrosVisualizacao(anguloCamera, larguraJanela, alturaJanela, 0.1, 5000.0);
		posicionarObservador(
			jogador.x - 50*cos(grausParaRadianos(anguloCameraJogadorXY)), 
			jogador.y - 50*sin(grausParaRadianos(anguloCameraJogadorXY)), 
			jogador.z + 40,
			jogador.x, jogador.y, jogador.z+jogador.r, 0, 0, 1);
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
			if(jogador.z <= arena.altura)
			{
				jogador.alterarAnguloXZ(+1 * timeDiference/1000);
			}
		}
		if(teclas['s'] == 1 || teclas['S'] == 1) 
		{
			if(jogador.z >= 0)
			{
				jogador.alterarAnguloXZ(-1 * timeDiference/1000);
			}
		}

		//Verificar se o piloto chegou ao final da arena
		if(verificarLimiteArena(jogador))
		{
			teletransportarJogador(jogador.angulo_xy);
		}

		if(verificarColisaoInimigo())
		{
			cout << "Explodiu!" << endl;
		}

	}

	// Início, decolando ou jogando
	if(estado != 3) {
		atualizarEstadoInimigos();
	}

	if(teclas['r'] == 1 || teclas['R'] == 1) {
		reiniciarJogo();
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
		case 'I':
		case 'i':
		{
			int inc = anguloCamera <= 5 ? 0 : 1;
			anguloCamera -= inc;
			break;
		}
		case 'O':
		case 'o':
		{
			int inc = anguloCamera >= 180 ? 0 : 1;
			anguloCamera += inc;
			break;
		}
		case '0':
			camera = 0;
			break;
		case '1':
			camera = 1;
			break;
		case '2':
			camera = 2;
			break;
		case '3':
			camera = 3;
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
}

void keyUp(unsigned char key, int x, int y) {
	teclas[key] = 0;
}

void specialKeys(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT:
			obsX -=10;
			break;
		case GLUT_KEY_RIGHT: 
			obsX +=10;
			break;
		case GLUT_KEY_UP: 
			obsY +=10;
			break;
		case GLUT_KEY_DOWN: 
			obsY -=10;
			break;
		case GLUT_KEY_HOME : 
			obsZ +=10;
			break;
		case GLUT_KEY_END : 
			obsZ -=10;
			break;
	}
}

//Controla o canhão
void passiveMotion(int x, int y)
{	
		//	cout << "X= " << x << endl;

		//	if(jogando)
		//	{
		// if(x <= controladorCanhaoX)
		// {
		// 	if(jogador.angulo_canhao_xy >= -30 && jogador.angulo_canhao_xy <= 29)
		// 	{
		// 		jogador.angulo_canhao_xy += 1;
		// 	}
			
		// 	controladorCanhaoX = x;
		// }
		// else
		// {
		// 	if(jogador.angulo_canhao_xy >= -29 && jogador.angulo_canhao_xy <= 30)
		// 	{
		// 		jogador.angulo_canhao_xy -= 1;
		// 	}
			
		// 	controladorCanhaoX = x;
		// }

		// if(y <= controladorCanhaoZ)
		// {
		// 	if(jogador.angulo_canhao_xz >= -30 && jogador.angulo_canhao_xz <= 29)
		// 	{
		// 		jogador.angulo_canhao_xz += 1;
		// 	}
			
		// 	controladorCanhaoZ = y;
		// }
		// else
		// {
		// 	if(jogador.angulo_canhao_xz >= -29 && jogador.angulo_canhao_xz <= 30)
		// 	{
		// 		jogador.angulo_canhao_xz -= 1;
		// 	}
			
		// 	controladorCanhaoZ = y;
		// }
	//	}

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

void motion(int x, int y){
	if (movimentarCamera3) {
		if(x > ultimaPosicaoMouseCameraX)
			anguloCameraJogadorXY -= 3;
		if(x < ultimaPosicaoMouseCameraX)
			anguloCameraJogadorXY += 3;

		ultimaPosicaoMouseCameraX = x;
	}
}

void mouse(int button, int state, int x, int y) {

	if(button == 0 && state == 0 && estado == 2) {
		// Cria tiro
	}
	if(button == 2 && state == 0 && estado == 2) {
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
	if(button == 0 && state == 0 && (teclas['b'] == 1 || teclas['B'] == 1)) {
		movimentarCamera3 = true;
	}
	if(button == 0 && state == 1) {
		movimentarCamera3 = false;
	}
}

void reshape(GLsizei w, GLsizei h) {
	// Para previnir uma divis�o por zero
	if ( h == 0 ) h = 1;

	// Calcula a correcao de aspecto
	//	fAspect = (GLfloat)w/(GLfloat)h;

	// Especifica o tamanho da viewport
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);

	especificarParametrosVisualizacao(anguloCamera, w, h, 0.1, 500.0);

	larguraJanela = w;
	alturaJanela = h - 200;	
}

/*
 * FUNÇÃO PRINCIPAL
 */

int main(int argc, char** argv) {

	if (!inicializarObjetosJogo(argv[1]))
		return EXIT_FAILURE;

	cout << "Angulo xy: " << jogador.angulo_xy << endl;
	cout << "Angulo xz: " << jogador.angulo_xz << endl;

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
	inicializarOpengl();
	glutMainLoop();

	return EXIT_SUCCESS;
}
