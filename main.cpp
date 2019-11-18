#include "tinyxml2.h"
#include "configuracao.h"
#include "pista.h"
#include "arena.h"
#include "inimigo.h"
#include "base.h"
#include "jogador.h"
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <math.h>

#define grausParaRadianos(g) g*(M_PI/180)

using namespace tinyxml2;
using namespace std;

/*
 * DIMENSÕES DO JOGO
 */
int larguraJanela = 500;
int alturaJanela = 500;

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
Jogador jogador;
Jogador jogador_copia;
Pista pista;
Arena arena;
int estado = 0;

/*
 * VARIÁVEIS PARA AJUSTE DO TEMPO
 */
static GLdouble previousTime = 0;
GLdouble currentTime;
GLdouble timeDiference;

// VARIÁVEIS DA CÂMERA
int camera = 1;
GLfloat angle=45, fAspect;
GLdouble obsX=200, obsY=200, obsZ=200;
GLdouble eyeX=0, eyeY=0, eyeZ=0;
GLdouble upX=0, upY=1, upZ=0;



int anguloCamera = 60;


/*
 * REGRAS DO JOGO
 */
void teletransportarJogador() {
}

void reiniciarJogo() {
}

void atualizarEstadoJogador() {
}

void atualizarEstadoInimigos() {
}

void atualizarEstadoTirosJogador() {
}

void atualizarEstadoTirosInimigos() {
}

void atualizarEstadoBombas() {
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
	// Habilita o modelo de colorizacao de Gouraud
	glShadeModel(GL_SMOOTH);

	
	glShadeModel(GL_FLAT);
	glViewport(0, 0, (GLsizei)larguraJanela, (GLsizei)alturaJanela);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (GLfloat)larguraJanela / (GLfloat)alturaJanela, 1, 15);


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
				Arena a = Arena(x, y, r);
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

		// int fator_correcao_x = (arena.r - arena.x) - arena.r;
		// int fator_correcao_y = (arena.r - arena.y) - arena.r;

		// // Normaliza todos os elementos
		// arena.x += fator_correcao_x;
		// arena.y += fator_correcao_y;
		// arena.y *= -1;

		// jogador.x += fator_correcao_x;
		// jogador.y += fator_correcao_y;
		// jogador.y *= -1;

		// pista.x1 += fator_correcao_x;
		// pista.y1 += fator_correcao_y;
		// pista.y1 *= -1;
		// pista.x2 += fator_correcao_x;
		// pista.y2 += fator_correcao_y;
		// pista.y2 *= -1;

		// for(int i=0; i < inimigos_aereos.size(); i++){
		// 	inimigos_aereos.at(i).x += fator_correcao_x;
		// 	inimigos_aereos.at(i).y += fator_correcao_y;
		// 	inimigos_aereos.at(i).y *= -1;
		// }

		// for(int i=0; i < inimigos_terrestres.size(); i++){
		// 	inimigos_terrestres.at(i).x += fator_correcao_x;
		// 	inimigos_terrestres.at(i).y += fator_correcao_y;
		// 	inimigos_terrestres.at(i).y *= -1;
		// }

		// // Seta valores iniciais para os ângulos
		// float angulo = atan2((pista.y2-pista.y1), (pista.x2-pista.x1));
		// jogador.angulo = radianosParaGraus(angulo);
		// jogador.angulo_canhao_arena = radianosParaGraus(angulo);

		// // Calcula a velocidade dos inimigos
		// float distancia 					= sqrt(pow(pista.y2-pista.y1,2) + pow(pista.x2-pista.x1,2));
		// float velocidade_decolagem_final 	= (distancia / 8) * 4;
		
		// // Guarda instância Jogador
		// jogador_base = jogador;
		// // Guarda instâncias Inimigos aéreos
		// for(int i=0; i < inimigos_aereos.size(); i++){
		// 	inimigos_aereos.at(i).velocidade = velocidade_decolagem_final;
        // 	inimigos_aereos_base.push_back(inimigos_aereos.at(i));
		// }
		// // Guarda instâncias Inimigos terrestres
		// for(int i=0; i < inimigos_terrestres.size(); i++){
		// 	inimigos_terrestres_base.push_back(inimigos_terrestres.at(i));
		// }

		// // Cria o placar
		// placar = Placar(arena.r, inimigos_terrestres.size());

	return true;
}

void PosicionaObservador(GLdouble obsX, GLdouble obsY, GLdouble obsZ, GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble upX, GLdouble upY, GLdouble upZ)
{
	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();
	// Especifica posição do observador e do alvo
    gluLookAt(obsX, obsY, obsZ, eyeX, eyeY, eyeZ, upX, upY, upZ);
}

void EspecificaParametrosVisualizacao(GLfloat angle, GLfloat fAspectW, GLfloat fAspectH, GLfloat zMin, GLfloat zMax) {
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();
	// Especifica a projeção perspectiva(angulo,aspecto,zMin,zMax)
	gluPerspective(angle, (GLfloat)fAspectW / (GLfloat)fAspectH, zMin, zMax);
	
	
	glMatrixMode(GL_MODELVIEW);
}

/*
void mudarCamera(int angle, int w, int h)
{
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluPerspective(angle, (GLfloat)w / (GLfloat)h, 0.1, 500.0);

    glMatrixMode(GL_MODELVIEW);
}
*/


/*
 * CALLBACK'S
 */

void display(void) {

	/* Limpar todos os pixels */
	glClearColor (0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//---------------------------------------------------------------------------------------	
	//Câmera da bomba
	glViewport(0, (GLsizei)alturaJanela, (GLsizei)larguraJanela ,200);	
	glLoadIdentity();
//	mudarCamera(anguloCamera, larguraJanela, 200);
	EspecificaParametrosVisualizacao(anguloCamera, larguraJanela, 200, 0.1, 500.0);

	//Colocar o código da câmera da bomba aqui

	
	PosicionaObservador(0.0, 80.0, 200.0,
					0.0, 0.0, 0.0, 
					0.0, 1.0, 0.0);
	
	glPushMatrix();
		GLfloat mat_ambient_a[] = {0.0, 0.0, 1.0, 1.0};
		glColor3fv(mat_ambient_a);
		glutSolidTeapot(20);
	glPopMatrix();



	

	
//---------------------------------------------------------------------------------------	
	
	//Câmera normal
	glViewport(0, 0, (GLsizei)larguraJanela, (GLsizei)alturaJanela);
	glLoadIdentity();
//	mudarCamera(anguloCamera, larguraJanela, alturaJanela);
	EspecificaParametrosVisualizacao(anguloCamera, larguraJanela, alturaJanela, 0.1, 500.0);

	//Colocar o código do jogo a partir daqui
	
	
	
	
	
	
	PosicionaObservador(0.0, 80.0, 200.0,
					0.0, 0.0, 0.0, 
					0.0, 1.0, 0.0);

	glPushMatrix();		
		GLfloat mat_ambient_y[] = {1.0, 1.0, 0.0, 1.0};
		glColor3fv(mat_ambient_y);
		glutWireTeapot(20);
	glPopMatrix();
	
	
	
	
	
	
	
	
	glutSwapBuffers();
}

void idle(void) {

	// Calcula a diferença de tempo
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	timeDiference = currentTime - previousTime;
	previousTime = currentTime;

	// Ajusta a camera
	if(camera == 1){
		EspecificaParametrosVisualizacao(angle, larguraJanela, alturaJanela, 0.5, 5000);
		PosicionaObservador(obsX, obsY, obsZ, eyeX, eyeY, eyeZ, upX, upY, upZ);
	}
	if(camera == 2){
		// EspecificaParametrosVisualizacao(angle, fAspect, 0.5, 5000);
		// PosicionaObservador(jx-120, jy+50, jz, jx, jy, jz, 0, 1, 0);
	}
	if(camera == 3){
		
	} 

	// Decolando
	if(estado == 1){

	}

	// Jogando
	else if(estado == 2){

		atualizarEstadoJogador();

		atualizarEstadoTirosInimigos();

		atualizarEstadoTirosJogador();

		atualizarEstadoBombas();

		criarTirosInimigos();		

		if(teclas['a'] == 1) {
			
		}
		if(teclas['d'] == 1) {
			
		}
		if(teclas['w'] == 1) {
			
		}
		if(teclas['s'] == 1) {
			
		}
	}

	// Início, decolando ou jogando
	if(estado != 3) {
		atualizarEstadoInimigos();
	}

	if(teclas['r'] == 1) {
		reiniciarJogo();
	}
	
	
	
	
	
	
	
	
	
	

	// Marca a janela atual como precisando ser reexibida
	glutPostRedisplay();
}

void keyPress(unsigned char key, int x, int y) {
	teclas[key] = 1;	
	switch (key)
	{
		case 'u':
			estado = (estado == 0) ? 1 : estado;
			break;
		case '+':
			// jogador.velocidade += (estado == 2) ? 1 : 0;
			break;
		case '-':
			// jogador.velocidade -= (estado == 2) ? ((jogador.velocidade > 50) ? 1 : 0) : 0;
			break;
		case 'I':
		case 'i':
		{	
			int inc = anguloCamera <= 5 ? 0 : 1;
		     anguloCamera -= inc;
		   //  mudarCamera(anguloCamera, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		     EspecificaParametrosVisualizacao(anguloCamera, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0.1, 500.0);
		     			
			break;
		}
		case 'O':
		case 'o':
		{	
			int inc = anguloCamera >= 180 ? 0 : 1;
		     anguloCamera += inc;
		    // mudarCamera(anguloCamera, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		     EspecificaParametrosVisualizacao(anguloCamera, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0.1, 500.0);
			break;
		}
		default:
			break;  	
	}
}

void keyUp(unsigned char key, int x, int y) {
	teclas[key] = 0;
}

void specialKeys(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT :
			break;
		case GLUT_KEY_RIGHT :
			break;
		case GLUT_KEY_UP : 
			break;
		case GLUT_KEY_DOWN : 
			break;
		case GLUT_KEY_HOME : 
			break;
		case GLUT_KEY_END : 
			break;
	}
}

void passiveMotion(int x, int y){
}

void mouse(int button, int state, int x, int y) {

	if(button == 0 && state == 0 && estado == 2) {
		// Cria tiro
	}
	if(button == 2 && state == 0 && estado == 2) {
		// Cria bomba
	}
}

void reshape(GLsizei w, GLsizei h) {
	// Para previnir uma divis�o por zero
	if ( h == 0 ) h = 1;

	// Calcula a correcao de aspecto
//	fAspect = (GLfloat)w/(GLfloat)h;

	// Especifica o tamanho da viewport
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);

//	mudarCamera(anguloCamera, w, h);
	EspecificaParametrosVisualizacao(anguloCamera, w, h, 0.1, 500.0);

	larguraJanela = w;
	alturaJanela = h - 200;
 
	
}

/*
 * FUNÇÃO PRINCIPAL
 */

int main(int argc, char** argv) {

	if (!inicializarObjetosJogo(argv[1]))
		return EXIT_FAILURE;

	glutInit (&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(larguraJanela, alturaJanela + 200);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-larguraJanela)/2,(glutGet(GLUT_SCREEN_HEIGHT)-alturaJanela)/2);
	glutCreateWindow("TRABALHO FINAL");
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutPassiveMotionFunc(passiveMotion);
	glutMouseFunc(mouse);
	inicializarOpengl();
	glutMainLoop();

	return EXIT_SUCCESS;
}
