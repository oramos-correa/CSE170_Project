#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include "shader.h"
#include "shaderprogram.h"

#include <vector> //
#include <math.h> //

#include "Objects.h"

int choice = 1;
int night = 0;


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


//Plane plane(bounds, bounds);
Plane plane(20, 20);

Camera camera;

Sphere sphere[] = {
	//radius, color, initial starting position on the y axis [x], positon on board[x,z]
   Sphere(1, GREEN, 10, 6, 1),
   Sphere(0.4, CYAN, 0.4, 1, 7)
};

Torus torus[] = {
	//radius, color, initial starting position on the y axis [x], positon on board[x,z]
   Torus(1, 3, GREY, 1, 6, 6)
};

Cube cube[] = {
	//size, initial starting position on the y axis [x], positon on board[x,z]
  Cube(1.5, MAGENTA, 1.5 / 2, 3, 40/4),
  Cube(2, ORANGE, 10, 6, 1),
  Cube(2, ORANGE,  -0.9, 6, 1)
};

Cylinder cylinder[] = {
	//radius, size, color, initial starting position on the y axis [x], positon on board[x,z]
  Cylinder(1.5, 2, YELLOW, 1.5, 6, 6)
};

Pyramid pyramid[] = {
	//color, initial starting position on the y axis [x], positon on board[x,z] 
	Pyramid(YELLOW, 0, 2, 4)
};

Cone cone[] = {
	//color, initial starting position on the y axis [x], positon on board[x,z] 
	Cone(1,1,ORANGE, 0, 2, -4)
};

Rect rect[] = {
	//width, height, depth, color, initial starting position on the y axis [x], positon on board[x,z] 
	Rect(5,2,3,ORANGE, 1.5/2, -2, 4)
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

	// Draw ground
	plane.draw();


	for (int i = 0; i < sizeof sphere / sizeof(Sphere); i++) {
		sphere[i].update();
	}
	for (int i = 0; i < sizeof torus / sizeof(Torus); i++) {
		torus[i].update();
	}
	for (int i = 0; i < sizeof cube / sizeof(Cube); i++) {
		cube[i].update();
	}
	for (int i = 0; i < sizeof cylinder / sizeof(Cylinder); i++) {
		cylinder[i].update();
	}
	for (int i = 0; i < sizeof pyramid / sizeof(Pyramid); i++) {
		pyramid[i].update();
	}
	for (int i = 0; i < sizeof cone / sizeof(Cone); i++) {
		cone[i].update();
	}
	for (int i = 0; i < sizeof rect / sizeof(Rect); i++) {
		rect[i].update();
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
			glutSpecialFunc(special);
			break;
		}

		if (choice == 1) {
			choice = 0;
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

	case 'z':
	{
		bounds++;
		break;
	}
	case 'x':
	{
		bounds--;
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
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutTimerFunc(100, timer, 0);
	glutKeyboardFunc(keyboard_func);
	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	init();
	glutMainLoop();

}