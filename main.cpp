#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include "shader.h"
#include "shaderprogram.h"

#include <vector> //
#include <math.h> //

// Colors
GLfloat WHITE[] = { 1, 1, 1 };
GLfloat BLACK[] = { 0, 0, 0 };
GLfloat YELLOW[] = { 1, 1, 0 };
GLfloat CYAN[] = { 0, 1, 1 };
GLfloat RED[] = { 1, 0, 0 };
GLfloat GREEN[] = { 0, 1, 0 };
GLfloat MAGENTA[] = { 1, 0, 1 };

// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 2.0f, z = 5.0f;
// all variables initialized to 1.0, meaning
float angle = 0.0f;


void changeSize(int w, int h)
{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

class Sphere {
	double radius; // Defines the radius
	GLfloat* color; //Dynamically assigns the color
	double maximumHeight; //Defines the maximum height
	double x; //x
	double y; //y
	double z; //z
	int direction; // direction
public:
	Sphere(double r, GLfloat* c, double h, double x, double z) : //constructor
		radius(r), color(c), maximumHeight(h), direction(-1), y(h), x(x), z(z) {
	}

	void update() { //method
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color); // Color and shading
		//glColor3ub(color[0], color[1], color[2]);
		glTranslated(x, y, z);
		//glutSolidSphere(radius, 30, 30);
		glutWireSphere(radius, 30, 30);
		glPopMatrix();

	}

	~Sphere() {}; //destructor

};

// Cube Class
class Cube {
	double leSize; // Defines the radius
	GLfloat* color; //Dynamically assigns the color
	double maximumHeight; //Defines the maximum height
	double x; //x
	double y; //y
	double z; //z
	int direction; // direction
public:
	Cube(double s, GLfloat* c, double h, double x, double z) :
		leSize(s), color(c), maximumHeight(h), direction(-1), y(h), x(x), z(z) {
	}

	void update() {

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		//glColor3ub(color[0], color[1], color[2]);
		glTranslated(x, y, z);
		glutSolidCube(leSize);

		glPopMatrix();

	}

	~Cube() {};

};

class Cylinder {
	double radius;
	double leSize; // Defines the radius
	GLfloat* color; //Dynamically assigns the color
	double maximumHeight; //Defines the maximum height
	double x; //x
	double y; //y
	double z; //z
	int direction; // direction
public:
	Cylinder(double r, double s, GLfloat* c, double h, double x, double z) :
		radius(r), leSize(s), color(c), maximumHeight(h), direction(-1), y(h), x(x), z(z) {
	}

	void update() {

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		//glColor3ub(color[0], color[1], color[2]);
		glTranslated(x, y, z);
		glutSolidCylinder(radius, leSize, 30, 30);

		glPopMatrix();

	}

	~Cylinder() {};

};

// Plane class
class Plane {
	int displayListId;
	int width;
	int depth;
public:
	Plane(int width, int depth) : width(width), depth(depth) {} //Planes parameters
	double centerx() { return width / 2; } //Cameras focus point [Where the camera fixates to]
	double centerz() { return depth / 2; }
	void create() {
		displayListId = glGenLists(1);
		glNewList(displayListId, GL_COMPILE);
		//GLfloat lightPosition[] = { 4, 3, 7, 1 };
		//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);


		glBegin(GL_QUADS);
		glNormal3d(0, 1, 0); //Shader of plane
		for (int x = 0; x < width - 1; x++) {
			for (int z = 0; z < depth - 1; z++) { //Dimensions of the plane [Plane gets weird if parameters are changed here]
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, WHITE); //Color of plane
				glVertex3d(x, 0, z);
				glVertex3d(x + 1, 0, z);
				glVertex3d(x + 1, 0, z + 1);
				glVertex3d(x, 0, z + 1);
			}
		}
		glEnd();
		glEndList();
	}
	void draw() {
		glCallList(displayListId);
	}
};



Plane plane(15, 15);

Sphere sphere[] = {
	//radius, color, initial starting position on the y axis [x], positon on board[x,z]
   Sphere(1, GREEN, 1, 6, 1),
   Sphere(0.4, CYAN, 0.4, 1, 7)
};

Cube cube[] = {
	//size, initial starting position on the y axis [x], positon on board[x,z]
  Cube(1.5, MAGENTA, 1.5 / 2, 3, 4)
};

Cylinder cylinder[] = {
	//radius, size, color, initial starting position on the y axis [x], positon on board[x,z]
  Cylinder(1.5, 2, YELLOW, 1.5, 6, 6)
};

void init() {

	glClearColor(0.1, 0.39, 0.88, 1.0); //Changes color of background

	glEnable(GL_DEPTH_TEST); //Objects will clip in cameras view if disabled
	glEnable(GL_LIGHTING); //Everything turns pure white if disabled
	glEnable(GL_LIGHT0); //Everything turns pure dark if disabled

	plane.create();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();


	// Set the camera
	gluLookAt(x, 1.0f, z, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);

	// Draw ground
	plane.draw();


	for (int i = 0; i < sizeof sphere / sizeof(Sphere); i++) {
		sphere[i].update();
	}
	for (int i = 0; i < sizeof cube / sizeof(Cube); i++) {
		cube[i].update();
	}
	for (int i = 0; i < sizeof cylinder / sizeof(Cylinder); i++) {
		cylinder[i].update();
	}

	glFlush();
	glutSwapBuffers();
}


void processSpecialKeys(int key, int xx, int yy)
{

	float fraction = 0.1f;

	switch (key) {
	case GLUT_KEY_LEFT:
		angle -= 0.05f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.05f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:
		x += lx * fraction;
		z += lz * fraction;
		break;
	case GLUT_KEY_DOWN:
		x -= lx * fraction;
		z -= lz * fraction;
		break;
	}
}

void keyboard_func(unsigned char key, int xx, int yy)
{

	float fraction = 0.1f;

	switch (key) {

	case 'w':
	{
		x += lx * fraction;
		z += lz * fraction;
		break;
	}
	case 's':
	{
		x -= lx * fraction;
		z -= lz * fraction;
		break;
	}

	}
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Project");
	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);
	glutIdleFunc(display);
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(keyboard_func);
	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	init();
	glutMainLoop();

}

