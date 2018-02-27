#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

// Global variables 
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 500;
bool brickSelected = false;
bool isAirborn = false;
int xangle = 0;
int yangle = 0;
int brickX = 200;
int brickY = WINDOW_HEIGHT - 200;
int brickZ = 0;
int brickWidth = 20;
int brickHeight = 10;
int brickDepth = 10;
int initialX = brickX;
int initialY = brickY;
int finalX = initialX;
int finalY = initialY;
float xVelocity = 0;
float yVelocity = 0;
float gravity = 0;
// GLenum mode = GL_POLYGON;
GLenum mode = GL_LINE_LOOP;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init() {
	glClearColor(0.0, 0.0, 0.0, 1.0); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1000.0, 500.0, 0.0, -500.0, 500.0);
	glEnable(GL_DEPTH_TEST);
}

//---------------------------------------
// Function to draw 3D block
//---------------------------------------
void brick(float xmin, float xmax, float ymin,
	float ymax, float zmin, float zmax) {
	// Define 8 vertices
	float ax = xmin, ay = ymin, az = zmax;
	float bx = xmax, by = ymin, bz = zmax;
	float cx = xmax, cy = ymax, cz = zmax;
	float dx = xmin, dy = ymax, dz = zmax;
	float ex = xmin, ey = ymin, ez = zmin;
	float fx = xmax, fy = ymin, fz = zmin;
	float gx = xmax, gy = ymax, gz = zmin;
	float hx = xmin, hy = ymax, hz = zmin;

	// Draw 6 faces
	glBegin(mode);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(ax, ay, az);
	glVertex3f(bx, by, bz);
	glVertex3f(cx, cy, cz);
	glVertex3f(dx, dy, dz);
	glEnd();

	glBegin(mode);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(ax, ay, az);
	glVertex3f(dx, dy, dz);
	glVertex3f(hx, hy, hz);
	glVertex3f(ex, ey, ez);
	glEnd();

	glBegin(mode);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(ax, ay, az);
	glVertex3f(ex, ey, ez);
	glVertex3f(fx, fy, fz);
	glVertex3f(bx, by, bz);
	glEnd();

	glBegin(mode);
	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(gx, gy, gz);
	glVertex3f(fx, fy, fz);
	glVertex3f(ex, ey, ez);
	glVertex3f(hx, hy, hz);
	glEnd();

	glBegin(mode);
	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(gx, gy, gz);
	glVertex3f(cx, cy, cz);
	glVertex3f(bx, by, bz);
	glVertex3f(fx, fy, fz);
	glEnd();

	glBegin(mode);
	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(gx, gy, gz);
	glVertex3f(hx, hy, hz);
	glVertex3f(dx, dy, dz);
	glVertex3f(cx, cy, cz);
	glEnd();
}

//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y) {
	// y = WINDOW_HEIGHT - y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x <= brickX + brickWidth && x >= brickX - brickWidth
		&& y <= brickY + brickHeight && y >= brickY - brickHeight) {
		// draw the brick centered at your mouse cursor
		initialX = brickX;
		initialY = brickY;
		brickSelected = true;
		cout << "Left mouse click!" << x << ", " << y << endl;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		cout << "Release the brick @ " << x << ", " << y << endl;
		finalX = x;
		finalY = y;
		brickSelected = false;
		isAirborn = true;

		// Launch brick
		xVelocity = finalX - initialX;
		yVelocity = finalY - initialY;
		clock_t start;
		double duration;

		start = clock();

		while (isAirborn) {
			brickX = brickX + (xVelocity * (clock() - start) / (double)CLOCKS_PER_SEC);
			brickY = brickY + (yVelocity * (std::clock() - start) / (double)CLOCKS_PER_SEC);
			if (brickX >= WINDOW_WIDTH || brickY >= WINDOW_HEIGHT || brickX <= 0 || brickY <= 0) {
				isAirborn = false;
			}
			glutPostRedisplay();
		}

		cout << "Brick landed" << endl;
	}
}

//---------------------------------------
// Mouse motion callback for OpenGL
//---------------------------------------
void drag(int x, int y) {
	if (brickSelected) {
		// redraw the brick at the current x, y
		brickX = x;
		brickY = y;
		cout << "Dragging at " << x << ", " << y << endl;

		// Redraw objects
		glutPostRedisplay();
	}
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y) {
	// Update angles
	if (key == 'x')
		xangle = (xangle - 5) % 360;
	else if (key == 'y')
		yangle = (yangle - 5) % 360;
	else if (key == 'X')
		xangle = (xangle + 5) % 360;
	else if (key == 'Y')
		yangle = (yangle + 5) % 360;

	// Redraw objects
	glutPostRedisplay();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display() {
	// Incrementally rotate objects
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);

	// Draw brick
	brick(brickX - brickWidth, brickX + brickWidth, brickY - brickHeight, brickY + brickHeight, brickZ - brickDepth, brickZ + brickDepth);
	glFlush();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(250, 250);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutCreateWindow("Angry Bricks");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	init();
	glutMainLoop();
	return 0;
}