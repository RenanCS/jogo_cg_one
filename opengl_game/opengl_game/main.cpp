/*
* OGL02Animation.cpp: 3D Shapes with animation
*/
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <math.h>	//inclue math
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Global variables */
typedef struct coord {
	float x;
	float y;
	float z;
}coord;
static const GLfloat cores[4][3] = {
	0.38f, 0.58f, 0.93f, // azul
	1.0f, 0.0f, 0.0f,    // vermelho
	1.0f, 0.5f, 0.0f,    // selecionado
};
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

char title[] = "3D Shapes with animation";
GLfloat anglePyramid = 0.0f;  // Rotational angle for pyramid [NEW]
GLfloat angleCube = 0.0f;     // Rotational angle for cube [NEW]
int refreshMills = 15;        // refresh interval in milliseconds [NEW]
int numInimigos, nivel;
double rotate_z, rotate_y, rotate_x;
float angle, eixoX, eixoY, eixoZ;
struct coord cameraPos, cameraAng;
struct inimigo inimigos[255];
GLfloat Zoom, passo, ratio;





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

	glTranslatef(ini.pontos.x *10.0, ini.pontos.y, ini.pontos.z * 10.0);
	glutSolidSphere(0.1f, 20, 20);
	glPopMatrix();
}

void drawPilar(void) {

	printf("\n Inicializou drawPilar");
	//glPushMatrix();
	glDisable(GL_CULL_FACE);
	glColor4f(1.0f, 1.0f, 0.0f, 0.5);

	glBegin(GL_POLYGON);
	glLineWidth(2);
	glVertex3f(0.5, -2.0, 0.5);
	glVertex3f(0.5, 1.0, 0.5);
	glVertex3f(-0.5, 1.0, 0.5);
	glVertex3f(-0.5, -2.0, 0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glLineWidth(2);
	glVertex3f(0.5, -2.0, -0.5);
	glVertex3f(0.5, 1.0, -0.5);
	glVertex3f(0.5, 1.0, 0.5);
	glVertex3f(0.5, -2.0, 0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glLineWidth(2);
	glVertex3f(-0.5, -2.0, 0.5);
	glVertex3f(-0.5, 1.0, 0.5);
	glVertex3f(-0.5, 1.0, -0.5);
	glVertex3f(-0.5, -2.0, -0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glLineWidth(2);
	glVertex3f(0.5, 1.0, 0.5);
	glVertex3f(0.5, 1.0, -0.5);
	glVertex3f(-0.5, 1.0, -0.5);
	glVertex3f(-0.5, 1.0, 0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glLineWidth(2);
	glVertex3f(0.5, -1.0, -0.5);
	glVertex3f(0.5, -1.0, 0.5);
	glVertex3f(-0.5, -1.0, 0.5);
	glVertex3f(-0.5, -1.0, -0.5);
	glEnd();

	//glFlush();
	//glPopMatrix();
}

void drawBase(void) {
	printf("\n Inicializou drawPilar");
	glPushMatrix();
	glDisable(GL_CULL_FACE);

	//base
	glBegin(GL_QUADS);
	glColor4f(0.0f, 1.0f, 0.0f, 0.5);
	glVertex3f(eixoX, -eixoY, eixoZ);
	glVertex3f(-eixoX, -eixoY, eixoZ);
	glVertex3f(-eixoX, -eixoY, -eixoZ);
	glVertex3f(eixoX, -eixoY, -eixoZ);
	glEnd();


	glBegin(GL_QUADS);
	glColor4f(0.0f, 5.0f, 0.0f, 0.1);
	glVertex3f(-eixoX, -eixoY, eixoZ);    // Top Right Of The Quad (Left)
	glVertex3f(-eixoX, eixoY, eixoZ);    // Top Left Of The Quad (Left)
	glVertex3f(-eixoX, eixoY, -eixoZ);    // Bottom Left Of The Quad (Left)
	glVertex3f(-eixoX, -eixoY, -eixoZ);    // Bottom Right Of The Quad (Left)
	glEnd();

	glBegin(GL_QUADS);
	glColor4f(0.0f, 5.0f, 0.0f, 0.1);
	glVertex3f(eixoX, eixoY, -eixoZ);    // Top Right Of The Quad (Right)
	glVertex3f(eixoX, eixoY, eixoZ);    // Top Left Of The Quad (Right)
	glVertex3f(eixoX, -eixoY, eixoZ);    // Bottom Left Of The Quad (Right)
	glVertex3f(eixoX, -eixoY, -eixoZ);    // Bottom Right Of The Quad (Right) 
	glEnd();

	glBegin(GL_QUADS);
	glColor4f(0.0f, 5.0f, 0.0f, 0.1);
	glVertex3f(eixoX, -eixoY, -eixoZ);    // Top Right Of The Quad (Back)
	glVertex3f(-eixoX, -eixoY, -eixoZ);    // Top Left Of The Quad (Back)
	glVertex3f(-eixoX, eixoY, -eixoZ);    // Bottom Left Of The Quad (Back)
	glVertex3f(eixoX, eixoY, -eixoZ);    // Bottom Right Of The Quad (Back)
	glEnd();

	glFlush();
	glPopMatrix();



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

void teclasEspeciais(int key, int x, int y) {
	float fraction = 0.1;

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
		printf("\n x %f", rotate_x);
		rotate_x += cameraAng.x  * fraction;
		rotate_z += cameraAng.z * fraction;
		break;
	case GLUT_KEY_DOWN:
		printf("\n z %f", rotate_z);
		rotate_x -= cameraAng.x  * fraction;
		rotate_z -= cameraAng.z * fraction;
		break;
	case GLUT_KEY_F5:
		break;
	}

	glutPostRedisplay();
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

void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

	rotate_z = 10.0f;
	rotate_y = 1.0f;
	rotate_x = -0.16f;
	angle = 0.0;
	Zoom = 60;
	passo = 0;
	ratio = 0;
	numInimigos = 10;
	eixoX = 5.0;
	eixoY = 2.0;
	eixoZ = 3.0;



	for (int i = 0; i < numInimigos; i++) {
		float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		inimigos[i].pontos.x = x;
		inimigos[i].pontos.y = y;
		inimigos[i].pontos.z = z;

		inimigos[i].nivel = 1;
		inimigos[i].selecionado = false;
		inimigos[i].id = i;

	}

}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Render a color-cube consisting of 6 quads with different colors
	glLoadIdentity();                 // Reset the model-view matrix
	gluLookAt(
		rotate_x, rotate_y, rotate_z,//visao da camera
		rotate_x + cameraAng.x, rotate_y + cameraAng.y, rotate_z + cameraAng.z, // coordenadas do objeto
		0.0f, 1.0f, 0.0f);

	//Piso
	drawBase();
	//Pilar
	drawPilar();

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//Desenhar inimigos
	montarInimigos();


	glutSwapBuffers();

}

void timer(int value) {
	glutPostRedisplay();      // Post re-paint request to activate display()
	glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}

void reshape(GLsizei width, GLsizei height) {
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
								  // Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	glutInit(&argc, argv);            // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // Enable double buffered mode
	glutInitWindowSize(640, 480);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow("T2 OPENGL");          // Create window with the given title
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event
	glutSpecialFunc(teclasEspeciais); //Eventos teclado	
	glutMouseFunc(OnMouseClick);	  //Eventos de mouse
	initGL();                       // Our own OpenGL initialization
	glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}