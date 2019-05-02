#pragma warning(disable : 4996) 
#include <iostream>
#include <glut.h>
#include <math.h>
#include <vector>
#include <utility>
#include <time.h>
#include <string>
#include <algorithm>
#include <conio.h>

using namespace std;
#define Frac_Circ 360  // Cien fracciones de circulo 
#define PI 3.1415926535897932 


typedef pair<float, float> pff;
int SCREEN_HEIGHT = 480;
vector<pff> PC(4);
vector<pff> PB, PR[3];
pff nuevo;
float mouseX, mouseY;
bool mouseleft;

float t_ = 1.0;
const int maxi = 100;
int dp[maxi][maxi], dx = 1, t = 0, id = 0, apreto = 0, id_cambio = -1;
float color1, color2, color3;

void init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(4.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}


void DibujaPunto(int x, int y)
{
	glPointSize(9);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void DibujarPuntoCircular(int x, int y) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < Frac_Circ + 1; i++) {  // +1 para cerrar 
		glVertex2f(x + 5 * cos(2.0 * PI * i / Frac_Circ),
			y + 5 * sin(2.0 * PI * i / Frac_Circ));
	}
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(1.0);
	glBegin(GL_LINES);
	for (int i = 0; i < Frac_Circ + 1; i++) {  // +1 para cerrar 
		glVertex2f(x + 5 * cos(2.0 * PI * i / Frac_Circ),
			y + 5 * sin(2.0 * PI * i / Frac_Circ));
	}
	glEnd();
}

void DibujarPuntoCircularDelgado(int x, int y) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < Frac_Circ + 1; i++) {  // +1 para cerrar 
		glVertex2f(x + 3 * cos(2.0 * PI * i / Frac_Circ),
			y + 3 * sin(2.0 * PI * i / Frac_Circ));
	}
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(1.0);
	glBegin(GL_LINES);
	for (int i = 0; i < Frac_Circ + 1; i++) {  // +1 para cerrar 
		glVertex2f(x + 3 * cos(2.0 * PI * i / Frac_Circ),
			y + 3 * sin(2.0 * PI * i / Frac_Circ));
	}
	glEnd();
}

void DibujaLinea(pff p1, pff p2)
{
	glLineWidth(1.0);
	glBegin(GL_LINES);
	glVertex3f(p1.first, p1.second, 0);
	glVertex3f(p2.first, p2.second, 0);
	glEnd();
}

void DibujaLineaGruesa(pff p1, pff p2)
{
	glLineWidth(2.5);
	glBegin(GL_LINES);
	glVertex3f(p1.first, p1.second, 0);
	glVertex3f(p2.first, p2.second, 0);
	glEnd();
}

pff DibujaBezier(double t)
{
	pff P = { 0.0, 0.0 };
	int n = PC.size() - 1;
	for (int i = 0; i < PC.size(); i++)
	{
		P.first += 1.0 * dp[n][i] * (PC[i].first) * (pow((1 - t), n - i)) * (pow(t, i));
		P.second += 1.0 * dp[n][i] * (PC[i].second) * (pow((1 - t), n - i)) * (pow(t, i));
	}
	return P;
}
pff puntoRecta(double t, pff p1, pff p2) {
	return { p1.first * t + (1 - t) * p2.first , p1.second * t + (1 - t) * p2.second };
}

void DrawLineaPunteada(pff p1, pff p2) {
	float dist = sqrt(pow((p2.first - p1.first), 2) + pow((p2.second - p1.second), 2));
	float inters = dist / 10;
	float aumento = 1.0 / inters;
	for (float t = 0; t < 1.0; t = t + aumento) {
		float x1 = (p2.first - p1.first) * t + p1.first;
		float y1 = (p2.second - p1.second) * t + p1.second;
		float x2 = (p2.first - p1.first) * (t + (aumento * 0.6)) + p1.first;
		float y2 = (p2.second - p1.second) * (t + (aumento * 0.6)) + p1.second;
		glBegin(GL_LINES);
		glVertex3f(x1, y1, 0);
		glVertex3f(x2, y2, 0);
		glEnd();
	}
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glPointSize(5);
	for (int i = 0; i < 4; i++) {
		if (i == 1 || i == 2) {
			glColor3f(0.0, 0.0, 1.0);
			DibujaPunto(PC[i].first, PC[i].second);
		}
		else {
			glColor3f(1.0, 0.0, 0.0);
			DibujarPuntoCircular(PC[i].first, PC[i].second);
		}
	}

	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < PB.size() - 1; i++) DibujaLineaGruesa(PB[i], PB[i + 1]);

	for (int i = 0; i < 3; i++) {
		if (i == 1) {
			glColor3f(0.33, 0.33, 0.33);
			DrawLineaPunteada(PC[i], PC[i + 1]);
		}
		else {
			glColor3f(0.0, 1.0, 0.0);
			DibujaLinea(PC[i], PC[i + 1]);
		}
	}
	glColor3f(1.0, 0.0, 0.0);
	DibujarPuntoCircularDelgado(PB[id].first, PB[id].second);

	for (int i = 0; i < 3; i++) {
		glColor3f(0.0, 0.5, 0.5);
		DibujarPuntoCircularDelgado(PR[i][id].first, PR[i][id].second);
		if (i < 2) {
			if (i == 0) glColor3f(color1, color3, color2); else if (i == 1) glColor3f(color2, color3, color1);
			DrawLineaPunteada(PR[i][id], PR[i + 1][id]);
		}
	}

	pff pc1 = puntoRecta(t_, PR[0][id], PR[1][id]);
	pff pc2 = puntoRecta(t_, PR[1][id], PR[2][id]);

	glColor3f(1.0, 0.3, 1.0);
	DibujarPuntoCircularDelgado(pc1.first, pc1.second);
	DibujarPuntoCircularDelgado(pc2.first, pc2.second);

	glColor3f(color3, color2, color1);
	DrawLineaPunteada(pc1, PB[id]);
	DrawLineaPunteada(PB[id], pc2);

	glColor3f(0, 0, 0);
	for (char letra = 'A'; letra < 'E'; letra++) {
		glRasterPos2i(PC[letra - 'A'].first + 10, PC[letra - 'A'].second + 10);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, letra);
	}
	glutSwapBuffers();

}
void trianguloPascal() {
	for (int line = 0; line < maxi; line++)
	{
		for (int i = 0; i <= line; i++) {
			if (line == i || i == 0) dp[line][i] = 1;
			else dp[line][i] = dp[line - 1][i - 1] + dp[line - 1][i];
		}
	}
}
void puntos() {
	pff POld = PC[0];
	for (double t = 0.0; t <= 1.0; t += 0.005) {
		pff P = DibujaBezier(t);
		PB.push_back(P);
		POld = P;
	}
	for (int i = 0; i < 3; i++) {
		for (double t = 0.0; t <= 1.0; t += 0.005)
			PR[i].push_back(puntoRecta(t, PC[i], PC[i + 1]));
		reverse(PR[i].begin(), PR[i].end());
	}
}
void idle(void) {
	t++;
	if (t == 2e6) {
		if (id + dx >= PB.size() || id + dx < 0) dx *= -1;
		t_ += (dx < 0 ? 0.005 : -0.005);
		id += dx, t = 0;
		glutPostRedisplay();
	}
}
void mouseCB(int button, int state, int x, int y) {
	mouseX = x;
	mouseY = y;
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN) {
			mouseleft = true;
		}
		if (state == GLUT_UP && apreto)
		{
			mouseleft = false;
		}
	}
}


void mouseMotionCB(int x, int y) {
	if (mouseleft) {
		apreto = 1;
		for (int i = 0; i < 4; i++) {
			nuevo = { x,480 - y };
			if (abs(PC[i].first - nuevo.first) < 5 && abs(PC[i].second - nuevo.second) < 5) {
				id_cambio = i;
				break;
			}
		}
		nuevo = { x,480 - y };
		apreto = 0;
		if (id_cambio == -1) return;
		PC[id_cambio] = nuevo;
		id_cambio = -1;
		PB.clear();
		for (int i = 0; i < 3; i++)
			PR[i].clear();
		puntos();
	}
}

int main() {

	time_t now = time(0);
	tm* time = localtime(&now);
	srand(time->tm_sec);
	trianguloPascal();
	color1 = (rand() % 100) * 0.01; color2 = (rand() % 100) * 0.01; color3 = (rand() % 100) * 0.01;
	for (int i = 0; i < 4; i++)	PC[i] = { rand() % 600 + 20, rand() % 440 + 20 };
	puntos();

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Bezier Curves Animation");
	init();
	glutDisplayFunc(draw);
	glutPostRedisplay();
	glutIdleFunc(idle);
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);
	glutMainLoop();
	return 0;
}
