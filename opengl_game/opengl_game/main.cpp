#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

// Globais
static const GLfloat cores[4][3] = {
	0.38f, 0.58f, 0.93f, // azul
	1.0f, 0.0f, 0.0f,    // vermelho
	1.0f, 0.5f, 0.0f,    // selecionado
};


typedef struct coord {
	float x;
	float y;
	float z;
}coord;

typedef struct inimigo {
	int vida;
	int status; //0-vivo, 1-morto
	int posicionado;
	int nivel;
	int id;
	bool selecionado;
	struct coord pontos;
	struct coord posInicial;

}inimigo;


struct coord cameraPos, cameraAng;
struct inimigo inimigos[255];

double rotate_z = 0.0f;
double rotate_y = 1.0f;
double rotate_x = 0.0f;
int numInimigos = 10;
int numTiros, veloTiro;
int raioInimigo, raioBala, raioTorre;
float angle = 0.0;
GLfloat Zoom, passo, ratio;

int checarColisao(int x, int y, int z, int p) {


	return 1;

}

void drawInimigo(struct inimigo &ini) {
	printf("\n Inicializou drawInimigo");
	glPushMatrix();
	glTranslatef(ini.posInicial.x, ini.posInicial.y, ini.posInicial.z);
	//alterar cor
	switch (ini.nivel) {
	case 1:
		printf("\n azul drawInimigo");
		glColor3f(cores[0][0], cores[0][1], cores[0][2]);
		break;
	case 2:

		printf("\n vermelho drawInimigo");
		glColor3f(cores[1][0], cores[1][1], cores[1][2]);
		break;
	}

	if (ini.selecionado == true) {
		glColor3f(cores[2][0], cores[2][1], cores[2][2]);
	}

	glTranslatef(ini.pontos.x *10.0, 0, ini.pontos.z * 10.0);
	glutSolidSphere(0.1f, 20, 20);
	glPopMatrix();
}

void drawPilar(void) {

	printf("\n Inicializou drawPilar");
	glPushMatrix();
	glDisable(GL_CULL_FACE);
	glColor3f(0.85f, 0.75f, 0.85f);
	// Lado multicolorido - FRENTE
	glLineWidth(2);
	glVertex3f(-0.5, -0.5, -0.5);       // P1
	glVertex3f(-0.5, 0.5, -0.5);       // P2
	glVertex3f(0.5, 0.5, -0.5);       // P3
	glVertex3f(0.5, -0.5, -0.5);       // P4
	glEnd();

	// Lado branco - TRASEIRA
	glBegin(GL_POLYGON);
	glLineWidth(2);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glEnd();

	// Lado roxo - DIREITA
	glBegin(GL_POLYGON);
	glLineWidth(2);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glEnd();

	// Lado verde - ESQUERDA
	glBegin(GL_POLYGON);
	glLineWidth(2);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glEnd();

	// Lado azul - TOPO
	glBegin(GL_POLYGON);
	glLineWidth(2);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glEnd();

	// Lado vermelho - BASE
	glBegin(GL_POLYGON);
	glLineWidth(2);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glEnd();

	glFlush();

	glPopMatrix();
}

void drawPiso(void) {
	printf("\n Inicializou drawPiso");
	glColor3f(0.5f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();

}

void montarInimigos(void) {
	printf("\n Inicializou montarInimigos");
	for (int i = 0; i < numInimigos; i++) {
		if (inimigos[i].vida == 0) {
			glStencilFunc(GL_ALWAYS, i, -1);
			drawInimigo(inimigos[i]);
		}
	}
}

void montarTorre(void) {
	printf("\n Inicializou montarTorre");
	drawPilar();
}

void montarPiso(void) {
	printf("\n Inicializou montarPiso");
	//Define propriedades do objeto
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Mudar visão da câmera
	/*
	1- posição da camera
	2- o ponto que estamos olhando
	3-  inclinar a câmera
	*/
	gluLookAt(rotate_x, rotate_y, rotate_z,
		rotate_x + cameraAng.x, rotate_y + cameraAng.y, rotate_z + cameraAng.z,
		0.0f, 1.0f, 0.0f);
	//desenha piso
	drawPiso();

	glutSwapBuffers();

}

void alteraTamanhoJanela(GLsizei w, GLsizei h) {
	if (h == 0) h = 1;// Para previnir uma divisão por zero
	ratio = (GLfloat)w / (GLfloat)h;// Calcula a correção de aspecto
	glViewport(0, 0, w, h);// Especifica o tamanho da viewport
}

void camera(void) {
	printf("\n Inicializou camera");
	//Define propriedades da câmera
	glMatrixMode(GL_PROJECTION);
	//referência para o ponto original
	glLoadIdentity();
	//perspectiva imagem
	gluPerspective(Zoom, ratio, 1.01, 2000);
}

void desenha(void) {
	printf("\n Inicializou desenha");
	glClearStencil(0); // this is the default value
					   //Limpa o buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//Propriedades camera
	camera();
	//piso
	montarPiso();
	//Desenha Torre
	montarTorre();

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//Desenhar inimigos
	montarInimigos();

	glutSwapBuffers();
}

void OnMouseClick(int button, int state, int x, int y) {
	if (state != GLUT_DOWN)
		return;

	float   window_width = glutGet(GLUT_WINDOW_WIDTH);
	float  window_height = glutGet(GLUT_WINDOW_HEIGHT);

	GLbyte color[4];
	GLfloat depth;
	GLuint id;

	glReadPixels(x, window_height - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
	glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	glReadPixels(x, window_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &id);

	printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u \n",
		x, y, color[0], color[1], color[2], color[3], depth, id);

	inimigos[id].selecionado = true;


}

void teclasEspeciais(int key, int x, int y) {
	float fraction = 0.2f;

	switch (key) {
	case GLUT_KEY_LEFT: // gira a câmera para esquerda
		angle -= 0.02f;
		cameraAng.x = sin(angle);
		cameraAng.z = -cos(angle);
		break;
	case GLUT_KEY_RIGHT: // gira a câmera para direita
		angle += 0.02f;
		cameraAng.x = sin(angle);
		cameraAng.z = -cos(angle);
		break;
	case GLUT_KEY_UP:
		rotate_x += cameraAng.x  * fraction;
		rotate_z += cameraAng.z * fraction;
		break;
	case GLUT_KEY_DOWN:
		rotate_x -= cameraAng.x  * fraction;
		rotate_z -= cameraAng.z * fraction;
		break;
	case GLUT_KEY_F1: // Zoom-in
		if (Zoom >= 10) {
			Zoom -= 3;
			rotate_y -= cameraAng.y  * fraction;
		}

		break;
	case GLUT_KEY_F2: 	// Zoom-out
		if (Zoom <= 300) {
			Zoom += 3;
			rotate_y += cameraAng.y  * fraction;
		}
		break;
	}
	glutPostRedisplay();
}

void ini(void) {
	Zoom = 60;
	passo = 0;
	ratio = 0;
	numTiros = 0;
	veloTiro = 10;

	for (int i = 0; i < numInimigos; i++) {
		float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//Verifica colisao
		if (checarColisao(x, y, z, i)) {
			inimigos[i].pontos.x = x;
			inimigos[i].pontos.y = y;
			inimigos[i].pontos.z = z;
		}
		inimigos[i].nivel = 1;
		inimigos[i].selecionado = false;
		inimigos[i].id = i;

	}

	printf("\n Inicializou variaveis");
}

int main(void) {
	int argc = 0;
	char *argv[] = { (char *)"gl", 0 };

	glutInit(&argc, argv);

	ini();
	//Mais dee um buffer para animações
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	//Tamanho da janela
	glutInitWindowSize(500, 500);
	//Tamanho da tela centralizada dividida por 2
	glutInitWindowPosition(320, 150);
	//Titulo da tela
	glutCreateWindow("Aplicação OPENGL");
	//Função de callback - toda ação realizada com a tela
	glutDisplayFunc(desenha);
	//Redimenciona
	glutReshapeFunc(alteraTamanhoJanela);
	//Eventos de mouse
	glutMouseFunc(OnMouseClick);
	//Eventos teclado
	glutSpecialFunc(teclasEspeciais);
	//Sistema entra em loop
	glutMainLoop();

	return 0;
}
