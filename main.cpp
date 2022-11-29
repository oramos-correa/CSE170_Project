#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include "shader.h"
#include "shaderprogram.h"

#include <vector> //
#include <math.h> //

// Colors: Will be implemented later
GLfloat WHITE[] = { 1, 1, 1 };
GLfloat YELLOW[] = { 1, 1, 0 };
GLfloat CYAN[] = { 0, 1, 1 };
GLfloat RED[] = { 1, 0, 0 };
GLfloat GREEN[] = { 0, 1, 0 };
GLfloat MAGENTA[] = { 1, 0, 1 };

//Camera class
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

// Sphere class
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
        radius(r), color(c), maximumHeight(h), direction(-1),
        y(h), x(x), z(z) {
    }
    
    void update() { //method
        glPushMatrix();
        //glColor3ub(color[0], color[1], color[2]);
        glTranslated(x, y, z);
        glutSolidSphere(radius, 30, 30);
        //glutWireSphere(radius, 30, 30);
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
        leSize(s), color(c), maximumHeight(h), direction(-1),
        y(h), x(x), z(z) {
    }

    void update() {
        
        glPushMatrix();
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
        radius(r), leSize(s), color(c), maximumHeight(h), direction(-1),
        y(h), x(x), z(z) {
    }

    void update() {
      
        glPushMatrix();
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
        glBegin(GL_QUADS);
        glNormal3d(0, 1, 0); //Shader of plane
        for (int x = 0; x < width - 1; x++) {
            for (int z = 0; z < depth - 1; z++) { //Dimensions of the plane [Plane gets weird if parameters are changed here]
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

// Global variables: a camera, a checkerboard and some balls.
Plane plane(10, 10); //Plane size
Camera camera;
Sphere sphere[] = {
   //radius, color, initial starting position on the y axis [x], positon on board[x,z]
  Sphere(1, GREEN, 1, 6, 1),
  Sphere(0.4, WHITE, 0.4, 1, 7)
};

Cube cube[] = {
    //size, initial starting position on the y axis [x], positon on board[x,z]
  Cube(1.5, MAGENTA, 1.5/2, 3, 4)
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

//Draws in the objects
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
        plane.centerx(), 0.0, plane.centerz(),
        0.0, 1.0, 0.0);
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

// On reshape, constructs a camera that fits the window.
void reshape(GLint w, GLint h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
    glMatrixMode(GL_MODELVIEW);
}

// Moves the camera
void special(int key, int, int) {
    switch (key) {
    case GLUT_KEY_RIGHT: camera.moveLeft(); break;
    case GLUT_KEY_LEFT: camera.moveRight(); break;
    case GLUT_KEY_UP: camera.moveUp(); break;
    case GLUT_KEY_DOWN: camera.moveDown(); break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(80, 80);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Project");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    init();
    glutMainLoop();
}