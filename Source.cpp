#include "Header.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <math.h>
#define dreapta 0x64 //a
#define stanga 0x61 //d
#define jos 0x73 //s
#define sus 0x77 //w

void CALLBACK miscareMaini();
void CALLBACK miscarePicioare();
void CALLBACK capesaAdd(AUX_EVENTREC *event);
void CALLBACK capesaSubtract(AUX_EVENTREC *event);
void CALLBACK miscareStanga(void);
void CALLBACK miscareDreapta(void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void myinit(void);
static GLfloat pozSursa[] = { 0.0, 8.0, 12.0, 1.0 };
static int shoulder = 0, rotire = 0, alfa = 90, spin = 0,
rotire1 = 0, leg = 0, directie = -1, directie1 = -1, head = 0;

GLfloat negru[4] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat verde[4] = { 0.0, 1.0, 0.0, 0.0 };
GLdouble eqn[4] = { 0.0, 0.0, -1.0, 0.0 };
GLdouble eqn1[4] = { 0.0, 0.0, 1.0, 0.0 };
GLdouble eqn3[4] = { 0.0, 1.0, 0.0, 0.0 };
GLUquadricObj* Cylinder = gluNewQuadric();
GLUquadricObj* Circle = gluNewQuadric();

void calcCoeficientiPlan(float P[3][3], float coef[4])
{
	float v1[3], v2[3];
	float length;
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;
	//calculeazã doi vectori din trei puncte
	v1[x] = P[0][x] - P[1][x];
	v1[y] = P[0][y] - P[1][y];
	v1[z] = P[0][z] - P[1][z];

	v2[x] = P[1][x] - P[2][x];
	v2[y] = P[1][y] - P[2][y];
	v2[z] = P[1][z] - P[2][z];

	//se cacluleazã produsul vectorial al celor doi vectori
	// care reprezintã un al treilea vector perpendicular pe plan 
	// ale cãrui componente sunt chiar coeficienþii A, B, C din ecuaþia planului
	coef[x] = v1[y] * v2[z] - v1[z] * v2[y];
	coef[y] = v1[z] * v2[x] - v1[x] * v2[z];
	coef[z] = v1[x] * v2[y] - v1[y] * v2[x];
	//normalizeazã vectorul
	length = (float)sqrt((coef[x] * coef[x]) + (coef[y] * coef[y]) + (coef[z] * coef[z]));
	coef[x] /= length;
	coef[y] /= length;
	coef[z] /= length;
}
void MatriceUmbra(GLfloat puncte[3][3], GLfloat pozSursa[4], GLfloat mat[4][4])
{
	GLfloat coefPlan[4];
	GLfloat temp;
	//determinã coeficienþii planului
	calcCoeficientiPlan(puncte, coefPlan);
	//determinã si pe D
	coefPlan[3] = -(
		(coefPlan[0] * puncte[2][0]) +
		(coefPlan[1] * puncte[2][1]) +
		(coefPlan[2] * puncte[2][2]));
	//temp=A*xL+B*yL+C*zL+D*w
	temp = coefPlan[0] * pozSursa[0] +
		coefPlan[1] * pozSursa[1] +
		coefPlan[2] * pozSursa[2] +
		coefPlan[3] * pozSursa[3];
	//prima coloanã
	mat[0][0] = temp - pozSursa[0] * coefPlan[0];
	mat[1][0] = 0.0f - pozSursa[0] * coefPlan[1];
	mat[2][0] = 0.0f - pozSursa[0] * coefPlan[2];
	mat[3][0] = 0.0f - pozSursa[0] * coefPlan[3];
	//a doua coloana
	mat[0][1] = 0.0f - pozSursa[1] * coefPlan[0];
	mat[1][1] = temp - pozSursa[1] * coefPlan[1];
	mat[2][1] = 0.0f - pozSursa[1] * coefPlan[2];
	mat[3][1] = 0.0f - pozSursa[1] * coefPlan[3];
	//a treia coloana
	mat[0][2] = 0.0f - pozSursa[2] * coefPlan[0];
	mat[1][2] = 0.0f - pozSursa[2] * coefPlan[1];
	mat[2][2] = temp - pozSursa[2] * coefPlan[2];
	mat[3][2] = 0.0f - pozSursa[2] * coefPlan[3];
	//a patra coloana
	mat[0][3] = 0.0f - pozSursa[3] * coefPlan[0];
	mat[1][3] = 0.0f - pozSursa[3] * coefPlan[1];
	mat[2][3] = 0.0f - pozSursa[3] * coefPlan[2];
	mat[3][3] = temp - pozSursa[3] * coefPlan[3];
}
void plan(void)
{
	glBegin(GL_QUADS);
	glVertex3f(500.0f, -2.55f, -500.0f);
	glVertex3f(-500.0f, -2.55f, -500.0f);
	glVertex3f(-500.0f, -2.55f, 500.0f);
	glVertex3f(500.0f, -2.55f, 500.0f);
	glEnd();
}
void CALLBACK miscareStanga(void)
{
	rotire = (rotire - 5) % 360;
}
void CALLBACK miscareDreapta(void)
{
	rotire = (rotire + 5) % 360;
}
void CALLBACK miscareSus(void)
{
	rotire1 = (rotire1 - 5) % 360;
}
void CALLBACK miscareJos(void)
{
	rotire1 = (rotire1 + 5) % 360;
}
void CALLBACK capesaAdd(AUX_EVENTREC *event)
{

	head = (head + 10) % 360;

}
void CALLBACK capesaSubtract(AUX_EVENTREC *event)
{

	head = (head - 10) % 360;

}
void CALLBACK miscareMaini()
{
	if (shoulder > 0)
		directie = -1;
	if (shoulder < 0)
		directie = 1;

	shoulder += 20 * directie;

}
void CALLBACK miscarePicioare()
{
	if (leg > 0)
		directie1 = -1;
	if (leg < 0)
		directie1 = 1;

	leg += 11 * directie1;

}
void CALLBACK luminaDr()//pt sursa de lumina
{
	pozSursa[0] += 0.3;
}
void CALLBACK luminaSt()//pt sursa de lumina
{
	pozSursa[0] -= 0.3;
}
void CALLBACK luminaSus()//pt sursa de lumina
{
	pozSursa[1] += 0.3;
}
void CALLBACK luminaJos()//pt sursa de lumina
{
	pozSursa[1] -= 0.3;
}
void CALLBACK IdleFunction(void) {

	miscareDreapta();
	miscareMaini();
	miscarePicioare();
	display();
	Sleep(250);
}


void myinit(void)
{
	glClearColor(0.5, 0.5, 0.5, 1.0);
	//setãri pentru sursa de luminã
	GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };


	GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

	GLfloat lmodel_ambient[] = { 1.0, 1.0, 1.0, 1.0 };


	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glFrontFace(GL_CW);
	glEnable(GL_LIGHTING); // activare iluminare
	glEnable(GL_LIGHT0);	// activare sursa 0
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glRotatef(-20.0, -1.0, 1.0, 0.0);


}
void manaDreapta() {


	glPushMatrix();
	glRotatef(90, 1, 0, 0);

	glPushMatrix();
	glTranslatef(-1.1, 0, -0.5);
	gluQuadricDrawStyle(Cylinder, GLU_FILL);
	gluCylinder(Cylinder, 0.15, 0.15, 1.3, 200, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.1, 0.0, -0.5);
	glClipPlane(GL_CLIP_PLANE3, eqn);
	glEnable(GL_CLIP_PLANE3);
	auxSolidSphere(0.15);
	glDisable(GL_CLIP_PLANE3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.1, 0.0, 0.8);
	glClipPlane(GL_CLIP_PLANE3, eqn1);
	glEnable(GL_CLIP_PLANE3);
	auxSolidSphere(0.15);
	glDisable(GL_CLIP_PLANE3);
	glPopMatrix();

	glPopMatrix();
}
void manaStanga() {



	glPushMatrix();
	glRotatef(90, 1, 0, 0);

	glPushMatrix();
	glTranslatef(1.1, 0, -0.5);
	gluQuadricDrawStyle(Cylinder, GLU_FILL);
	gluCylinder(Cylinder, 0.15, 0.15, 1.3, 200, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.1, 0.0, -0.5);
	glClipPlane(GL_CLIP_PLANE3, eqn);
	glEnable(GL_CLIP_PLANE3);
	auxSolidSphere(0.15);
	glDisable(GL_CLIP_PLANE3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.1, 0.0, 0.8);
	glClipPlane(GL_CLIP_PLANE3, eqn1);
	glEnable(GL_CLIP_PLANE3);
	auxSolidSphere(0.15);
	glDisable(GL_CLIP_PLANE3);
	glPopMatrix();

	glPopMatrix();

}
void maini() {

	glPushMatrix();

	//mana dreapta
	glPushMatrix();
	glTranslatef(0.05, 1, 0);
	glRotatef((GLfloat)shoulder, 1.0, 0.0, 0.0);
	glTranslatef(0, -1, 0);
	manaDreapta();
	glPopMatrix();

	//mana stanga
	glPushMatrix();
	glTranslatef(-0.05, 1, 0);
	glRotatef((GLfloat)-shoulder, 1.0, 0.0, 0.0);
	glTranslatef(0, -1, 0);
	manaStanga();
	glPopMatrix();

	glPopMatrix();
}
void corp() {


	glPushMatrix();
	glRotatef(90, 1, 0, 0);


	glPushMatrix();
	glTranslatef(0, 0, -1);
	gluQuadricDrawStyle(Cylinder, GLU_FILL);
	gluCylinder(Cylinder, 0.85, 0.85, 2.4, 200, 50);
	glPopMatrix();

	/*glPushMatrix();
	glTranslatef(0, 0, 1.5);
	gluQuadricDrawStyle(Circle, GLU_FILL);
	gluDisk(Circle, 0.0, 0.85, 100, 20);
	glPopMatrix();*/

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.85);
	glClipPlane(GL_CLIP_PLANE3, eqn1);
	glEnable(GL_CLIP_PLANE3);
	auxSolidSphere(0.85);
	glDisable(GL_CLIP_PLANE3);
	glPopMatrix();

	glPopMatrix();

}
void piciorStang() {

	glPushMatrix();
	glRotatef(90, 1, 0, 0);

	glPushMatrix();
	glTranslatef(-0.3, 0, 1.5);
	gluQuadricDrawStyle(Cylinder, GLU_FILL);
	gluCylinder(Cylinder, 0.18, 0.18, 0.85, 200, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.3, 0.0, 2.35);
	glClipPlane(GL_CLIP_PLANE3, eqn1);
	glEnable(GL_CLIP_PLANE3);
	auxSolidSphere(0.18);
	glDisable(GL_CLIP_PLANE3);
	glPopMatrix();

	glPopMatrix();


}
void piciorDrept() {



	glPushMatrix();
	glRotatef(90, 1, 0, 0);

	glPushMatrix();
	glTranslatef(0.3, 0, 1.5);
	gluQuadricDrawStyle(Cylinder, GLU_FILL);
	gluCylinder(Cylinder, 0.18, 0.18, 0.85, 200, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, 0.0, 2.35);
	glClipPlane(GL_CLIP_PLANE3, eqn1);
	glEnable(GL_CLIP_PLANE3);
	auxSolidSphere(0.18);
	glDisable(GL_CLIP_PLANE3);
	glPopMatrix();

	glPopMatrix();


}
void picioare() {

	glPushMatrix();

	glPushMatrix();
	glRotatef((GLfloat)leg, 1.0, 0.0, 0.0);
	piciorDrept();
	glPopMatrix();

	glPushMatrix();
	glRotatef((GLfloat)-leg, 1.0, 0.0, 0.0);
	piciorStang();
	glPopMatrix();

	glPopMatrix();
}
void antenaStanga() {




	glPushMatrix();
	glRotatef(90, 1, 0, 0);

	glPushMatrix();
	glTranslatef(0.5, -0.6, -1);
	gluQuadricDrawStyle(Cylinder, GLU_FILL);
	gluCylinder(Cylinder, 0.04, 0.04, 0.5, 200, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, -0.6, -1);
	glClipPlane(GL_CLIP_PLANE3, eqn);
	glEnable(GL_CLIP_PLANE3);
	auxSolidSphere(0.04);
	glDisable(GL_CLIP_PLANE3);
	glPopMatrix();

	glPopMatrix();

}
void antenaDreapta() {



	glPushMatrix();
	glRotatef(90, 1, 0, 0);

	glPushMatrix();
	glTranslatef(0.15, -0.6, -0.55);
	gluQuadricDrawStyle(Cylinder, GLU_FILL);
	gluCylinder(Cylinder, 0.04, 0.04, 0.5, 200, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15, -0.6, -0.55);
	glClipPlane(GL_CLIP_PLANE3, eqn);
	glEnable(GL_CLIP_PLANE3);
	auxSolidSphere(0.04);
	glDisable(GL_CLIP_PLANE3);
	glPopMatrix();

	glPopMatrix();

}
void cap() {



	glPushMatrix();

	glRotatef((GLfloat)head, 0.0, 1.0, 0.0);
	//cap
	glTranslatef(0.0, 1.1, 0.0);
	glClipPlane(GL_CLIP_PLANE3, eqn3);
	glEnable(GL_CLIP_PLANE3);
	auxSolidSphere(0.9);
	glDisable(GL_CLIP_PLANE3);

	//capac

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, 0.0);
	gluQuadricDrawStyle(Circle, GLU_FILL);
	gluDisk(Circle, 0.0, 0.9, 100, 20);
	glPopMatrix();

	//ochi

	glColor4fv(negru);

	glTranslatef(0.4, 0.43, 0.6);
	auxSolidSphere(0.1);
	glTranslatef(-0.8, 0.0, 0.0);
	auxSolidSphere(0.1);


	//urechi
	glColor4fv(verde);
	//antena stanga
	glPushMatrix();
	glRotatef(-35.0, 0, 0, 1);
	antenaStanga();
	glPopMatrix();

	//antena dreapta
	glPushMatrix();
	glRotatef(35.0, 0, 0, 1);
	antenaDreapta();
	glPopMatrix();

	glPopMatrix();
}
void cap1() {


	glPushMatrix();

	glRotatef((GLfloat)head, 0.0, 1.0, 0.0);
	//cap
	glTranslatef(0.0, 1.1, 0.0);
	glClipPlane(GL_CLIP_PLANE3, eqn3);
	glEnable(GL_CLIP_PLANE3);
	auxSolidSphere(0.9);
	glDisable(GL_CLIP_PLANE3);

	//capac

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, 0.0);
	gluQuadricDrawStyle(Circle, GLU_FILL);
	gluDisk(Circle, 0.0, 0.9, 100, 20);
	glPopMatrix();

	//urechi
	glColor3f(0.4, 0.4, 0.4);
	//antena stanga
	glPushMatrix();
	glRotatef(-35.0, 0, 0, 1);
	antenaStanga();
	glPopMatrix();

	//antena dreapta
	glPushMatrix();
	glRotatef(35.0, 0, 0, 1);
	antenaDreapta();
	glPopMatrix();

	glPopMatrix();

	
}

void robot() {

	glPushMatrix();
	glTranslatef(0, 0, 0);
	//rotire robot axa y
	glRotatef((GLfloat)rotire, 0.0, 1.0, 0.0);
	glTranslatef(-5, 0, 0);
	//rotire robot axa x
	glRotatef((GLfloat)rotire1, 1.0, 0.0, 0.0);
	cap();
	corp();
	maini();
	picioare();

	glPopMatrix();
}
void robot1() {

	glPushMatrix();
	glTranslatef(0, 0, 0);
	//rotire robot axa y
	glRotatef((GLfloat)rotire, 0.0, 1.0, 0.0);
	glTranslatef(-5, 0, 0);
	//rotire robot axa x
	glRotatef((GLfloat)rotire1, 1.0, 0.0, 0.0);
	glColor3f(0.4, 0.4, 0.4);
	//cap1();
	glColor3f(0.4, 0.4, 0.4);
	corp();
	glColor3f(0.4, 0.4, 0.4);
	maini();
	glColor3f(0.4, 0.4, 0.4);
	picioare();

	glPopMatrix();
}

void CALLBACK display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat matUmbra[4][4];
	GLfloat puncte[3][3] = { { -50.0f, -2.5f, -50.0f },
	{ -50.0, -2.5f, 50.0 },
	{ 50.0f, -2.5f, 50.0f } };

	MatriceUmbra(puncte, pozSursa, matUmbra);

	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, pozSursa);
	robot();
	glPopMatrix();

	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glMultMatrixf((GLfloat*)matUmbra);
	//glColor3f(0.4, 0.4, 0.4);
	robot1();
	glPopMatrix();

	//auxSolidSphere(0.4);
	glColor3f(1.0f, 1.0f, 0.3f);
	plan();
	glPopAttrib();
	glFlush();

}
void CALLBACK myReshape(GLsizei w, GLsizei h)
{
	if (!h) return;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-6.0, 6.0, -6.0*(GLfloat)h / (GLfloat)w,
			6.0*(GLfloat)h / (GLfloat)w, -100.0, 100.0);
	else
		glOrtho(-6.0*(GLfloat)w / (GLfloat)h,
			6.0*(GLfloat)w / (GLfloat)h, -6.0, 6.0, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
	auxInitDisplayMode(AUX_SINGLE | AUX_RGB | AUX_DEPTH16); //double
	auxInitPosition(350, 0, 1500, 1000);
	auxInitWindow("Robot Android");
	myinit();
	auxMouseFunc(AUX_RIGHTBUTTON, AUX_MOUSEDOWN, capesaSubtract);
	auxMouseFunc(AUX_LEFTBUTTON, AUX_MOUSEDOWN, capesaAdd);
	auxKeyFunc(stanga, miscareStanga);
	auxKeyFunc(dreapta, miscareDreapta);
	auxKeyFunc(sus, miscareSus);
	auxKeyFunc(jos, miscareJos);
	auxKeyFunc(AUX_RIGHT, luminaDr);
	auxKeyFunc(AUX_LEFT, luminaSt);
	auxKeyFunc(AUX_UP, luminaSus);
	auxKeyFunc(AUX_DOWN, luminaJos);
	//auxIdleFunc(IdleFunction);
	auxReshapeFunc(myReshape);
	auxMainLoop(display);
	return(0);
}

