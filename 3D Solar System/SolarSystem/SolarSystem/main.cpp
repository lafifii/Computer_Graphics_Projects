#pragma warning(disable:4996)
#pragma once
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <cmath>
#include <GL/glew.h>
#include <glut.h>
double  velocidad_cohete = 1;
bool stop = 1;
struct Point {
	float x, y, z;
	void setxyz(float xa, float ya, float za) {
		x = xa, y = ya, z = za;
	}
};
struct BMP {
	int w, h;
	BYTE* data;
	static unsigned int textura;
};


struct Particula {
	Point punto;
	float lifespan, tiempovivo, velocidad;
	Particula(Point p) {
		punto.setxyz(p.x, 9 + rand() % 3, p.z);
		tiempovivo = 0;
		lifespan = (rand() % 101) / 100.0;
		velocidad = velocidad_cohete;
	}
	void mover() {
		tiempovivo += 0.03;
	}
	void recal(Point p) {
		punto.setxyz(p.x, (90 + rand() % 30) / 10.0, p.z);
		tiempovivo = 0;
		lifespan = (rand() % 100) / 100.0;
		velocidad = velocidad_cohete;
	}
};

using namespace std;
unsigned int texture[11];
int actual[7];
bool debe[3] = { 0 };
Point punto_cohete;
float radios[8] = { 1.5,1.8,2,1.2,2.5, 1.6,2.1,2.1 };

float t_ = 0, dxt = 0.005;
GLfloat girax = 15, giray = 0, zoom[3] = { 0,0,-60 }, rx = 0, ry = 0, rz = 0;
const int maxi = 40;
vector<float> Asteroides;
vector<Point> PuntosEstrellas, PC(4), PC2(4);
vector<Point> PCE[5];
vector<Point> PO[7];
vector<BMP> Texturas;
vector<Particula> SistemaParticulas;

int dp[maxi][maxi];
float j_cohete = 0;
void trianguloPascal() {
	for (int line = 0; line < maxi; line++)
	{
		for (int i = 0; i <= line; i++) {
			if (line == i || i == 0) dp[line][i] = 1;
			else dp[line][i] = dp[line - 1][i - 1] + dp[line - 1][i];
		}
	}
}
void mover(void) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(zoom[0], zoom[1], zoom[2]);
	glRotated(girax, 1.0, 0.0, 0.0);
	glRotated(giray, 0.0, 1.0, 0.0);
	glRotated(40, 0, 0, 1);
}

void cohete_cub() {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	int xx = ((int)j_cohete + 1) % 301;
	double ang = 2.0 * 3.14 * xx / 300.0;
	float ult[3] = { cos(ang) * 30, 11.5, sin(ang) * 30 };
	punto_cohete.y += 2.5;

	glTranslatef(ult[0], ult[1], ult[2]);
	glBegin(GL_QUADS);
	// Front Face
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

	// Back Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);

	// Top Face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);

	// Bottom Face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);


	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);

	glEnd();

	glTranslatef(0,0,0);
	punto_cohete.y -= 2.5;
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void LoadTextures(const char* nombre) {
	int width, height;
	BYTE* data;
	FILE* file;
	// open texture data
	file = fopen(nombre, "rb");
	// allocate buffer
	BYTE header[54];
	fread(header, 1, 54, file);
	size_t dataPos = *(int*) & (header[0x0A]);
	size_t imageSize = *(int*) & (header[0x22]);
	width = *(int*) & (header[0x12]);
	height = *(int*) & (header[0x16]);

	if (imageSize == 0)    imageSize = width * height * 3;
	if (dataPos == 0)      dataPos = 54;
	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);
	BMP aux;
	aux.w = width, aux.h = height, aux.data = data;
	Texturas.push_back(aux);
}
void nueva_textura(int idx) {
	glBindTexture(GL_TEXTURE_2D, texture[idx]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, Texturas[idx].w, Texturas[idx].h, GL_BGR, GL_UNSIGNED_BYTE, Texturas[idx].data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
void cube() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glEnable(GL_LIGHTING);
	glBegin(GL_QUADS);
	// Front Face
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-60.0f, 60.0f, 60.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(60.0f, 60.0f, 60.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(60.0f, -60.0f, 60.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-60.0f, -60.0f, 60.0f);

	// Back Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(60.0f, -60.0f, -60.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(60.0f, 60.0f, -60.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-60.0f, 60.0f, -60.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-60.0f, -60.0f, -60.0f);

	// Top Face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(60.0f, 60.0f, -60.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(60.0f, 60.0f, 60.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-60.0f, 60.0f, 60.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-60.0f, 60.0f, -60.0f);

	// Bottom Face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-60.0f, -60.0f, 60.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(60.0f, -60.0f, 60.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(60.0f, -60.0f, -60.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-60.0f, -60.0f, -60.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(60.0f, -60.0f, 60.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(60.0f, 60.0f, 60.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(60.0f, 60.0f, -60.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(60.0f, -60.0f, -60.0f);


	glTexCoord2f(0.0f, 1.0f); glVertex3f(-60.0f, 60.0f, -60.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-60.0f, 60.0f, 60.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-60.0f, -60.0f, 60.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-60.0f, -60.0f, -60.0f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void estrella(Point p, int tipo) {
	glTranslatef(p.x, p.y, p.z);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0 * tipo, 0.2 * tipo, 0.0);
	glVertex3f(0.1 * tipo, 0.1 * tipo, 0.0);
	glVertex3f(0.2 * tipo, 0.05 * tipo, 0.0);
	glVertex3f(0.1 * tipo, 0.0 * tipo, 0.0);
	glVertex3f(0.2 * tipo, -0.1 * tipo, 0.0);
	glVertex3f(0.0 * tipo, 0.0 * tipo, 0.0);
	glVertex3f(-0.2 * tipo, -0.1 * tipo, 0.0);
	glVertex3f(-0.1 * tipo, 0.0 * tipo, 0.0);
	glVertex3f(-0.2 * tipo, 0.05 * tipo, 0.0);
	glVertex3f(-0.1 * tipo, 0.1 * tipo, 0.0);
	glEnd();
}
void planeta(float radio, float x, float y, float z, int idx) {
	glPushMatrix();
	glTranslatef(x, y, z);
	GLUquadric* qobj = gluNewQuadric();
	gluQuadricTexture(qobj, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[idx]);
	gluSphere(qobj, radio, 50, 50);
	gluDeleteQuadric(qobj);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void drawAsteroides() {
	for (int i = 0, j = 0; i < Asteroides.size() && j <= 300; i++, j += 6) {
		double angle = 2 * 3.14 * j / 300;
		planeta(Asteroides[i], cos(angle) * 28, (i & 2 ? -1 : 1), sin(angle) * 28, 9);
		planeta(Asteroides[i], cos(angle) * 30, (j & 1 ? 1 : 1), sin(angle) * 30, 9);
	}
}
Point DibujaBezier(double t, vector<Point> & PCA)
{
	Point P; P.setxyz(0, 0, 0);
	int n = PCA.size() - 1;
	for (int i = 0; i < PCA.size(); i++)
	{
		P.x += 1.0 * dp[n][i] * (PCA[i].x) * (pow((1 - t), n - i)) * (pow(t, i));
		P.y += 1.0 * dp[n][i] * (PCA[i].y) * (pow((1 - t), n - i)) * (pow(t, i));
		P.z += 1.0 * dp[n][i] * (PCA[i].z) * (pow((1 - t), n - i)) * (pow(t, i));
	}
	return P;
}
void orbitas_planetas(vector<Point> puntos, int idx) {
	glPushMatrix();
	glTranslatef(PO[idx][actual[idx]].x, PO[idx][actual[idx]].y, PO[idx][actual[idx]].z);
	glRotatef(rx, 1, 0, 0);
	glRotatef(ry, 0, 1, 0);
	glRotatef(rz, 0, 0, 1);
	glTranslatef(-PO[idx][actual[idx]].x, -PO[idx][actual[idx]].y, -PO[idx][actual[idx]].z);
	glLineWidth(1.5);
	glBegin(GL_LINES);
	vector<Point> vpa;
	vpa.push_back(puntos[0]);
	vpa.push_back(puntos[1]);
	vpa.push_back(puntos[2]);
	Point p1 = DibujaBezier(0, vpa);
	for (double t = 0.005; t <= 1; t += 0.005)
	{
		Point p2 = DibujaBezier(t, vpa);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
		p1 = p2;
	}
	vpa.clear();
	vpa.push_back(puntos[2]);
	vpa.push_back(puntos[3]);
	vpa.push_back(puntos[0]);

	p1 = DibujaBezier(0, vpa);
	for (double t = 0.005; t <= 1; t += 0.005)
	{
		Point p2 = DibujaBezier(t, vpa);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
		p1 = p2;
	}
	glEnd();
	glPopMatrix();
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat mat_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat mat_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat high_shininess[] = { 100.0f };
	GLfloat emission[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat emission2[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat light_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_pos[] = { 0.0, 0.0, 0.0, 1.0 };


	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission2);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	mover();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	planeta(4, 0, 0, 0, 1);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	for (int i = 0; i < 7; i++)
		planeta(radios[i], PO[i][actual[i]].x, PO[i][actual[i]].y, PO[i][actual[i]].z, i + 2);
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();


	glPointSize(1 + (rand() % 20) / 10.0);
	glColor3f(1, 1, 1);
	glBegin(GL_POINTS);
	for (int i = 0; i < 100; i++) {
		glVertex3f(SistemaParticulas[i].punto.x, SistemaParticulas[i].punto.y, SistemaParticulas[i].punto.z);
		SistemaParticulas[i].mover();
		if (SistemaParticulas[i].tiempovivo > SistemaParticulas[i].lifespan)
			SistemaParticulas[i].recal(punto_cohete);
	}
	glEnd();
	glColor3f(0, 0, 0.15);
	for (int i = 0; i < 7; i++) {
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < PO[i].size(); j++)
			glVertex3f(PO[i][j].x, PO[i][j].y, PO[i][j].z);
		glEnd();
	}
	glPopMatrix();
	glColor3f(0.2, 0.1, 0.2);
	orbitas_planetas(PC, 4);
	vector<Point> auxP = PC;
	for (int i = 0; i < 5; i++) {

		PC[0].x += (radios[4] * 1.4 + i * 0.2), PC[0].z += (radios[4] * 1.4 + i * 0.2);
		PC[3].x += (radios[4] * 1.4 + i * 0.2), PC[3].z -= (radios[4] * 1.4 + i * 0.2);
		PC[1].x -= (radios[4] * 1.4 + i * 0.2), PC[1].z += (radios[4] * 1.4 + i * 0.2);
		PC[2].x -= (radios[4] * 1.4 + i * 0.2), PC[2].z -= (radios[4] * 1.4 + i * 0.2);
		orbitas_planetas(PC, 4);
		PC = auxP;
	}
	glColor3f(0, 0.5, 0.5);
	orbitas_planetas(PC2, 5);
	PC2[0].x += (radios[5] * 1.5), PC2[0].z += (radios[5] * 1.5);
	PC2[3].x += (radios[5] * 1.5), PC2[3].z -= (radios[5] * 1.5);
	PC2[1].x -= (radios[5] * 1.5), PC2[1].z += (radios[5] * 1.5);
	PC2[2].x -= (radios[5] * 1.5), PC2[2].z -= (radios[5] * 1.5);
	orbitas_planetas(PC2, 5);

	glEnable(GL_LIGHTING);
	drawAsteroides();
	cohete_cub();
	glDisable(GL_LIGHTING);

	glColor3f(1, 0, 0);
	for (int i = 0; i < 5; i++) {
		glPushMatrix();
		Point p2 = DibujaBezier(t_, PCE[i]);
		glColor3f(0.97, 0.95, 0.17);
		estrella(p2, 2);
		glPopMatrix();
	}
	for (int j = 0; j < PuntosEstrellas.size(); j++) {
		float r = (rand() % 255) / 255.0, g = (rand() % 255) / 255.0, b = (rand() % 255) / 255.0;
		glPushMatrix();
		glColor3f(r, g, b);
		estrella(PuntosEstrellas[j], 1);
		glPopMatrix();
	}
	for (int i = 0; i < 5; i++) {
		glColor3f(1, 1, 1);
		glPushMatrix();
		glBegin(GL_POINTS);
		double tt_ = t_;
		for (int iter = 0; iter < 15; iter++) {
			tt_ -= dxt;
			if (tt_ < 0 || tt_ > 1) continue;
			Point p2 = DibujaBezier(tt_, PCE[i]);
			glVertex3f(p2.x, p2.y, p2.z);
		}
		glEnd();
		glPopMatrix();
	}
	glPushMatrix();
	cube();
	glPopMatrix();
	glutSwapBuffers();
}
void specialKeyInput(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:  giray -= 5; break;
	case GLUT_KEY_RIGHT: giray += 5; break;
	case GLUT_KEY_UP:    girax -= 5; break;
	case GLUT_KEY_DOWN:  girax += 5; break;
	case GLUT_KEY_F1: debe[0] = !debe[0]; break;
	case GLUT_KEY_F2: debe[1] = !debe[1]; break;
	case GLUT_KEY_F3: debe[2] = !debe[2]; break;
	case GLUT_KEY_F4: stop = !stop; break;
	default: break;
	}
	glutPostRedisplay();
}
void reshape(int ancho, int alto) {
	glClearColor(0, 0, 0, 0.0); //black	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, ancho, alto);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(90, 1, 12, 3000);
}
void calcular() {
	double angle = 2 * 3.14 * actual[4] / 300.0;
	for (int i = 0; i < 4; i++)
		PC[i].setxyz(cos(angle) * 35, 0, sin(angle) * 35);
	angle = 2 * 3.14 * actual[5] / 300.0;
	for (int i = 0; i < 4; i++)
		PC2[i].setxyz(cos(angle) * 40, 0, sin(angle) * 40);
}
void idle(void) {
	if (stop)giray += 0.5;
	if (t_ + dxt < 0 || t_ + dxt > 1) dxt *= -1;
	t_ += dxt;
	for (int j = 0; j < 7; j++) actual[j] = (actual[j] + 1) % 301;
	calcular();
	if (debe[0]) rx += 2.5;
	if (debe[1]) ry += 2.5;
	if (debe[2]) rz += 2.5;
	double angle = 2 * 3.14 * j_cohete / 300.0;
	punto_cohete.setxyz(cos(angle) * 30, 8, sin(angle) * 30);
	j_cohete += velocidad_cohete;
	glutPostRedisplay();
}
void iniciar() {
	double angle = 2 * 3.14 * j_cohete / 300.0;
	punto_cohete.setxyz(cos(angle) * 30, 8, sin(angle) * 30);
	j_cohete++;
	glGenTextures(12, texture);
	for(char i = 'a'; i <= 'i'; i++) 
		LoadTextures(i + ".bmp");
	LoadTextures("rocket.bmp");
	
	for (int i = 0; i < 11; i++) nueva_textura(i);
	for (int i = 0; i < 100; i++) {
		Particula pa(punto_cohete);
		SistemaParticulas.push_back(pa);
	}
	for (int i = 0; i < 50; i++) Asteroides.push_back((3 + rand() % 20) / 50.0);
	for (int i = 0; i < 500; i++) {
		Point aux;
		float x_ = (rand() % 100) * (rand() % 2 ? 1 : -1);
		float y_ = (rand() % 100) * (rand() % 2 ? 1 : -1);
		float z_ = (rand() % 100) * (rand() % 2 ? 1 : -1);
		aux.setxyz(x_, y_, z_);
		PuntosEstrellas.push_back(aux);
	}

	for (int i = 0, orb = 10; i < 7; i++, orb += 5) {
		actual[i] = rand() % 100;
		if (orb == 30) orb += 5;
		for (int j = 0; j <= 300; j++) {
			double angle = 2 * 3.14 * j / 300.0;
			Point auxp; auxp.setxyz(cos(angle) * orb, 0, sin(angle) * orb);
			PO[i].push_back(auxp);
		}
	}
	calcular();
}
void tecla(unsigned char c, int x, int y) {
	c = toupper(c);
	if (c == '+') zoom[2]++;
	else if (c == '-') zoom[2]--;
	else if (c == 'A') zoom[0]++;
	else if (c == 'D') zoom[0]--;
	else if (c == 'W') zoom[1]--;
	else if (c == 'S') zoom[1]++;
	else if (c == 'V') velocidad_cohete += 0.2;
	else if (c == 'B') velocidad_cohete -= 0.2;
	glutPostRedisplay();
}
int main(int argc, char** argv) {

	srand(time(NULL));
	for (int j = 0; j < 5; j++)
		for (int i = 0; i < 4; i++) {
			Point aux;
			aux.setxyz(rand() % 50 * (rand() % 2 ? 1 : -1), rand() % 50 * (rand() % 2 ? 1 : -1), rand() % 50 * (rand() % 2 ? 1 : -1));
			PCE[j].push_back(aux);
		}
	trianguloPascal();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowPosition(100, 0);
	glutInitWindowSize(900, 700);

	glutCreateWindow(".:: Space ::.");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutSpecialFunc(specialKeyInput);
	glutKeyboardFunc(tecla);

	iniciar();
	glutMainLoop();
	return 0;
}
