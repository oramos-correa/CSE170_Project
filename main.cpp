#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include "shader.h"
#include "shaderprogram.h"

#include <vector> //
#include <math.h> //

int choice = 1;
int night = 0;

// Colors
GLfloat WHITE[] = { 1, 1, 1 };
GLfloat GREY[] = { 0.5, 0.5, 0.5 };
GLfloat BLACK[] = { 0, 0, 0 };
GLfloat YELLOW[] = { 1, 1, 0 };
GLfloat CYAN[] = { 0, 1, 1 };
GLfloat RED[] = { 1, 0, 0 };
GLfloat GREEN[] = { 0, 1, 0 };
GLfloat MAGENTA[] = { 1, 0, 1 };
GLfloat ORANGE[] = { 1, 0.5, 0 };
GLfloat SILVER[] = { 0.90, 0.91, 0.98 };
GLfloat BROWN[] = { 0.5, 0.35, 0.05 };

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

class Camera {
	double theta;      // determines the x and z positions
	double y;          // the current y axis position
	double dTheta;     // increment in theta for moving the camera
	double dy;         // increment in y 
public:
	Camera() : theta(0), y(3), dTheta(0.04), dy(0.2) {} //The starting position of the camera
	double getX() { return 10 * cos(theta); } //Bounds of the camera
	double getY() { return y; }
	double getZ() { return 10 * sin(theta); }
	void moveRight() { theta += dTheta; } //Camera movement
	void moveLeft() { theta -= dTheta; }
	void moveUp() { y += dy; }
	void moveDown() { if (y > dy) y -= dy; }
};

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

	void update() {
		y += direction * 0.005;
		if (y > maximumHeight) {
			y = maximumHeight; direction = -1;
		}
		else if (y < radius) {
			y = radius; direction = 1;
		}
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslated(x, y, z);
		glutSolidSphere(radius, 30, 30);
		glPopMatrix();
	}

	~Sphere() {}; //destructor

};

class Torus {
	double radius; // Defines the radius
	double r;
	GLfloat* color; //Dynamically assigns the color
	double maximumHeight; //Defines the maximum height
	double x; //x
	double y; //y
	double z; //z
	int direction; // direction
public:
	Torus(double r, double R, GLfloat* c, double h, double x, double z) : //constructor
		radius(r), r(R), color(c), maximumHeight(h), direction(-1), y(h), x(x), z(z) {
	}

	void update() {
		//y += direction * 0.005;
		//if (y > maximumHeight) {
		//	y = maximumHeight; //direction = -1;
		//}
		//else if (y < radius) {
		//	y = radius; direction = 1;
		//}
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslated(x, y, z);
		glutSolidTorus(radius, r, 30, 30);
		glPopMatrix();
	}

	~Torus() {}; //destructor

};

#define red {0xff, 0x00, 0x00}
#define yellow {0xff, 0xff, 0x00}
#define magenta {0xff, 0, 0xff}
GLubyte texture[][3] = {
	red, yellow,
	yellow, red,
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
		GLfloat lightPosition[] = { 4, 3, 7, 1 };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

		glBegin(GL_QUADS);
		glNormal3d(0, 1, 0); //Shader of plane
		for (int x = 0; x < width - 1; x++) {
			for (int z = 0; z < depth - 1; z++) { //Dimensions of the plane [Plane gets weird if parameters are changed here]
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, SILVER); //Color of plane
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



Plane plane(40, 40);

Camera camera;

Sphere sphere[] = {
	//radius, color, initial starting position on the y axis [x], positon on board[x,z]
   Sphere(0.25, ORANGE, 5, 7.5, 7.5),
   Sphere(0.25, BLACK, 5, 7.5, 7.5)
};

Cube cube[] = {
	//size, initial starting position on the y axis [x], positon on board[x,z]
  Cube(2, BLACK,  -0.9, 7.5, 7.5),
  Cube(15, ORANGE,  -6.8, 7.5, 7.5),
  Cube(2,BLACK,5,7.5,0),
  Cube(40, BLACK,-19.5,20,20),
  Cube(15, BROWN,-6.8,8,32)
};

Cylinder cylinder[] = {
	//radius, size, color, initial starting position on the y axis [x], positon on board[x,z]
  Cylinder(0.8, 15, YELLOW, 0, 14.9, 0),
  Cylinder(0.8, 15, BROWN, 0, 0, 0),
  Cylinder(10, 0.4, RED, 0, 7.5, 0)
};

Torus torus[] = {
	//radius 1, radius 2, color, initial starting position on the y axis [x], positon on board[x,z]
   Torus(0.2, 1, GREY, 5, 7.5, 1)
};

void init() {

	glClearColor(0.1, 0.39, 0.88, 1.0); //Changes color of background


	glEnable(GL_DEPTH_TEST); //Objects will clip in cameras view if disabled
	glEnable(GL_LIGHTING); //Everything turns pure white if disabled
	glEnable(GL_LIGHT0); //Everything turns pure dark if disabled

	glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
	glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);

	plane.create();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();


	// Set the camera
	if (choice == 0) {
		gluLookAt(x, 1.0f, z, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);
	}
	if (choice == 1) {
		gluLookAt(camera.getX(), camera.getY(), camera.getZ(), plane.centerx(), 0.0, plane.centerz(), 0.0, 1.0, 0.0);
	}
	if (choice == 2) {
		gluLookAt(x, 75.0f, z, x + lx, 1.0f, z + lz, 0.0f,1.0f, 0.0f);
	}

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
	for (int i = 0; i < sizeof torus / sizeof(Torus); i++) {
		torus[i].update();
	}

	glFlush();
	glutSwapBuffers();
}

void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
}

void timer(int v) {
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, v);
}



void special(int key, int, int) {
	switch (key) {
	case GLUT_KEY_RIGHT: camera.moveLeft(); break;
	case GLUT_KEY_LEFT: camera.moveRight(); break;
	case GLUT_KEY_UP: camera.moveUp(); break;
	case GLUT_KEY_DOWN: camera.moveDown(); break;
	}
	glutPostRedisplay();
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
	case ' ':
	{
		if (choice == 0) {
			choice++;
			glutReshapeFunc(reshape);
			glutSpecialFunc(processSpecialKeys);
			glutSpecialFunc(special);
			break;
		}

		if (choice == 1) {
			choice++;
			glutReshapeFunc(reshape);
			glutSpecialFunc(processSpecialKeys);
			break;
		}
		if (choice == 2) {
			choice = 0;
			glutReshapeFunc(reshape);
			glutSpecialFunc(processSpecialKeys);
			break;
		}
	}

	case 'n':
	{
		if (night == 0) {
			glEnable(GL_LIGHT0);
			night++;
			break;
		}
		if (night == 1) {
			glDisable(GL_LIGHT0);
			night = 0;
			break;
		}
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
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutTimerFunc(100, timer, 0);
	glutKeyboardFunc(keyboard_func);
	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	init();
	glutMainLoop();

}