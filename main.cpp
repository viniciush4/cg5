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

/*
 * VARIÁVEIS DA CÂMERA
 */
int camera = 1;
GLfloat anguloCamera=45, fAspect;
GLdouble obsX=0, obsY=-10, obsZ=1000;
GLdouble eyeX=0, eyeY=0, eyeZ=0;
GLdouble upX=0, upY=0, upZ=1;

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

/*
 * CÂMERAS, LUZES E DESENHO
 */

void posicionarObservador(GLdouble obsX, GLdouble obsY, GLdouble obsZ, GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble upX, GLdouble upY, GLdouble upZ) {
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
            glScalef(5, 0.3, 0.3);
            glTranslatef(0, 0, 0); // put in one end
            glutSolidCube(20.0);
        glPopMatrix();

        //y axis
        glPushMatrix();
            glColor3fv(mat_ambient_g);
            glScalef(0.3, 5, 0.3);
            glTranslatef(0, 0, 0); // put in one end
            glutSolidCube(20.0);
        glPopMatrix();

        //z axis
        glPushMatrix();
            glColor3fv(mat_ambient_b);
            glScalef(0.3, 0.3, 5);
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

	arena.desenhar();

	pista.desenhar();

	jogador.desenhar();
}

void desenharViewport1() {
	
	glViewport(0, (GLsizei)alturaJanela, (GLsizei)larguraJanela ,200);	
	
	especificarParametrosVisualizacao(anguloCamera, larguraJanela, 200, 0.1, 5000.0);
	
	posicionarObservador(200.0, -200.0, 200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

	desenharMundo();
}

void desenharViewport2() {

	glViewport(0, 0, (GLsizei)larguraJanela, (GLsizei)alturaJanela);

	if(camera == 1){
		especificarParametrosVisualizacao(anguloCamera, larguraJanela, alturaJanela, 0.1, 5000.0);
		posicionarObservador(obsX, obsY, obsZ, eyeX, eyeY, eyeZ, upX, upY, upZ);
	}
	if(camera == 2){
		especificarParametrosVisualizacao(anguloCamera, larguraJanela, alturaJanela, 0.1, 5000.0);
		posicionarObservador(80, 80, 80, 0, 0, 0, 0, 1, 0);
	}
	if(camera == 3){
		
	} 

	desenharMundo();
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
			break;
		}
		case 'O':
		case 'o':
		{
			int inc = anguloCamera >= 180 ? 0 : 1;
			anguloCamera += inc;
			break;
		}
		case '1':
			camera = 1;
			break;
		case '2':
			camera = 2;
			break;
		case '3':
			camera = 3;
			break;
		case 'n':
			if(glIsEnabled(GL_LIGHT0)){ glDisable(GL_LIGHT0); }else{ glEnable(GL_LIGHT0); }
			break;
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

	cout << jogador.x << " " << jogador.y << " " << jogador.z << endl;

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
