﻿#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include "shader.h"
#include "shaderprogram.h"

#include <vector>
#include <math.h>

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
   Sphere(0.3, ORANGE, 6, 7.5, 1.5)
   //Sphere(0.3, BLACK, 5, 7.5, 7.5)
};

Cube cube[] = {
	//size, initial starting position on the y axis [x], positon on board[x,z]
  Cube(2, BLACK,  -0.9, 7.5, 7.5),
  Cube(14.9, ORANGE,  -6.93, 7.5, 7.5),
  Cube(2,BLACK,5,7.5,0),
  Cube(40, BLACK,-19.5,20,20),
  Cube(15, BROWN,-6.93,8,32),
  Cube(15, WHITE,-6.93,32,32),
  Cube(15, GREEN,-6.93,32,8)
  //
};

Cylinder cylinder[] = {
	//radius, size, color, initial starting position on the y axis [x], positon on board[x,z]
  Cylinder(0.8, 15, YELLOW, 0, 14.9, 0),
  Cylinder(0.8, 15, YELLOW, 0, 0, 0),
  Cylinder(10, 0.4, RED, 0, 7.5, 0.1),
  //
  Cylinder(4, 5.5, RED, 6, 5, 33.2)
};

//======Down facing torus=====
Torus torus[] = {
	//radius 1, radius 2, color, initial starting position on the y axis [x], positon on board[x,z]
   Torus(0.15, 0.8, GREY, 4.5, 7.5, 1.5)
   //
};

//=======Front facing torus=====
Torus2 torus2[] = {
	//radius 1, radius 2, color, initial starting position on the y axis [x], positon on board[x,z]
   Torus2(0.2, .5, WHITE, 5.2, 7.5, .82)
   //
};

/*
Pyramid pyramid[] = {
	//color, initial starting position on the y axis [x], positon on board[x,z]
	Pyramid(YELLOW, 0, 2, 4)
};
Cone cone[] = {
	//color, initial starting position on the y axis [x], positon on board[x,z]
	Cone(1,1,ORANGE, 0, 2, -4)
};
*/
Rect rect[] = {
	//width, height, depth, color, initial starting position on the y axis [x], positon on board[x,z] 
	Rect(8,12,6,RED, 1.5 / 2, 5, 36),
	Rect(2,1,1,YELLOW, 1.5 / 2, 12, 36),
	Rect(2,1,1,YELLOW, 1.5 / 2, 11.9, 37.2),
	Rect(2,0.6,1,YELLOW, 1.2, 11.95, 36.8)
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

float angle2 = 0.0f;
float lx2 = 0.788119f, ly2 = 2.02862e-32f, lz2 = -1.2804f;
// XZ position of the camera
float x2 = 39.4179f, y2 = 3.19189f, z2 = 12.7503f;


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();


	// Set the camera
	if (choice == 0) {
		// First person
		std::cout << "x2: " << x2 << ", y2: " << y2 << ", z2: " << z2 << std::endl;
		std::cout << "lx2: " << lx2 << ", ly2: " << ly2 << ", lz2: " << lz2 << std::endl;
		gluLookAt(x2, y2, z2, lx2, ly2, lz2, 0.0f, 1.0f, 0.0f);
	}
	if (choice == 1) {
		gluLookAt(camera.getX(), camera.getY(), camera.getZ(), plane.centerx(), 0.0, plane.centerz(), 0.0, 1.0, 0.0);
	}
	if (choice == 2) {
		gluLookAt(x, 75.0f, z, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);
	}

	// Draw ground
	plane.draw();


	for (int i = 0; i < sizeof sphere / sizeof(Sphere); i++) {
		sphere[i].update();
	}
	for (int i = 0; i < sizeof torus / sizeof(Torus); i++) {
		torus[i].update();
	}
	for (int i = 0; i < sizeof torus2 / sizeof(Torus2); i++) {
		torus2[i].update();
	}
	for (int i = 0; i < sizeof cube / sizeof(Cube); i++) {
		cube[i].update();
	}
	for (int i = 0; i < sizeof cylinder / sizeof(Cylinder); i++) {
		cylinder[i].update();
	}
	/*
	for (int i = 0; i < sizeof pyramid / sizeof(Pyramid); i++) {
		pyramid[i].update();
	}
	for (int i = 0; i < sizeof cone / sizeof(Cone); i++) {
		cone[i].update();
	}
	*/
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

	if (choice != 0)
	{
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
	else
	{
		switch (key) {
		case GLUT_KEY_LEFT:
			//angle -= 0.05f;
			//lx = sin(angle);
			//lz = -cos(angle);
			x2 -= lx2 * fraction;
			y2 -= ly2 * fraction;
			z2 -= lz2 * fraction;
			break;
		case GLUT_KEY_RIGHT:
			angle2 += 0.05f;
			//lx = sin(angle);
			//lz = -cos(angle);
			x2 += lx2 * fraction;
			y2 += ly2 * fraction;
			z2 += lz2 * fraction;
			break;
		case GLUT_KEY_UP:
			angle2 += 0.05f;
			lx2 = sin(angle2);
			ly2 += ly2 * .5;
			lz2 = tan(angle2);
			//x += lx * fraction;
			//z += lz * fraction;
			break;
		case GLUT_KEY_DOWN:
			angle2 -= 0.05f;
			lx2 = sin(angle2);
			ly2 -= ly2 * .5;
			lz2 = tan(angle2);
			//x -= lx * fraction;
			//z -= lz * fraction;
			break;
		}
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
			choice++;
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