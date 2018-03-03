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
// Sleep time in milliseconds for timer function
const int TIME = 20;
const int INIT_X = 200;
const int INIT_Y = WINDOW_HEIGHT - 200;
const int GRAVITY = -20;
const int MAX_ANIMATION_STEPS = 20;
bool brickSelected = false;
bool isAirborn = false;
bool landed = false;
bool randColor = false;
int xangle = 0;
int yangle = 0;
int zangle = 0;
float xRotateSpeed = 0.0;
float yRotateSpeed = 0.0;
float zRotateSpeed = 0.0;
int brickX = INIT_X;
int brickY = INIT_Y;
int brickZ = 0;
int brickWidth = 20;
int brickHeight = 10;
int brickDepth = 10;
int initialX = brickX;
int initialY = brickY;
int finalX = initialX;
int finalY = initialY;
int animationStep = 0;
float xVelocity = 0;
float yVelocity = 0;
float downwardForce = GRAVITY;
double airTime = 0;
clock_t startTime;
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

	// Reset tip
	cout << "To reset the program hit \"r\"" << endl;

	// Get random rotation speeds
	srand(time(NULL));
	xRotateSpeed = rand() % 10 + 1;
	yRotateSpeed = rand() % 10 + 1;
	zRotateSpeed = rand() % 10 + 1;
}

void randomRGB(float& red, float& green, float& blue) {
	red = ((float)rand() / (RAND_MAX));
	green = ((float)rand() / (RAND_MAX));
	blue = ((float)rand() / (RAND_MAX));
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

	if (randColor) {
		// Draw brick with random colors
		float red, green, blue;
		randomRGB(red, green, blue);

		// Draw 6 faces
		glBegin(mode);
		glColor3f(red, green, blue);
		glVertex3f(ax, ay, az);
		glVertex3f(bx, by, bz);
		glVertex3f(cx, cy, cz);
		glVertex3f(dx, dy, dz);
		glEnd();

		randomRGB(red, green, blue);
		glBegin(mode);
		glColor3f(red, green, blue);
		glVertex3f(gx, gy, gz);
		glVertex3f(fx, fy, fz);
		glVertex3f(ex, ey, ez);
		glVertex3f(hx, hy, hz);
		glEnd();

		randomRGB(red, green, blue);
		glBegin(mode);
		glColor3f(red, green, blue);
		glVertex3f(ax, ay, az);
		glVertex3f(dx, dy, dz);
		glVertex3f(hx, hy, hz);
		glVertex3f(ex, ey, ez);
		glEnd();

		randomRGB(red, green, blue);
		glBegin(mode);
		glColor3f(red, green, blue);
		glVertex3f(ax, ay, az);
		glVertex3f(ex, ey, ez);
		glVertex3f(fx, fy, fz);
		glVertex3f(bx, by, bz);
		glEnd();

		randomRGB(red, green, blue);
		glBegin(mode);
		glColor3f(red, green, blue);
		glVertex3f(gx, gy, gz);
		glVertex3f(cx, cy, cz);
		glVertex3f(bx, by, bz);
		glVertex3f(fx, fy, fz);
		glEnd();

		randomRGB(red, green, blue);
		glBegin(mode);
		glColor3f(red, green, blue);
		glVertex3f(gx, gy, gz);
		glVertex3f(hx, hy, hz);
		glVertex3f(dx, dy, dz);
		glVertex3f(cx, cy, cz);
		glEnd();
	}
	else {
		// Draw standard brick
		// Draw 6 faces
		glBegin(mode);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(ax, ay, az);
		glVertex3f(bx, by, bz);
		glVertex3f(cx, cy, cz);
		glVertex3f(dx, dy, dz);
		glEnd();

		glBegin(mode);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(gx, gy, gz);
		glVertex3f(fx, fy, fz);
		glVertex3f(ex, ey, ez);
		glVertex3f(hx, hy, hz);
		glEnd();

		glBegin(mode);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(ax, ay, az);
		glVertex3f(dx, dy, dz);
		glVertex3f(hx, hy, hz);
		glVertex3f(ex, ey, ez);
		glEnd();

		glBegin(mode);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(ax, ay, az);
		glVertex3f(ex, ey, ez);
		glVertex3f(fx, fy, fz);
		glVertex3f(bx, by, bz);
		glEnd();


		glBegin(mode);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(gx, gy, gz);
		glVertex3f(cx, cy, cz);
		glVertex3f(bx, by, bz);
		glVertex3f(fx, fy, fz);
		glEnd();

		glBegin(mode);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(gx, gy, gz);
		glVertex3f(hx, hy, hz);
		glVertex3f(dx, dy, dz);
		glVertex3f(cx, cy, cz);
		glEnd();
	}
}

void rotateBrick(int x, int y, int z) {
	xangle = (xangle + x) % 360;
	yangle = (yangle + y) % 360;
	zangle = (zangle + z) % 360;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(brickX, brickY, brickZ);
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);
	glRotatef(zangle, 0.0, 0.0, 1.0);
	glTranslatef(-brickX, -brickY, -brickZ);
	glutPostRedisplay();
}

void landingAnimation() {
	if (animationStep < MAX_ANIMATION_STEPS) {
		// Change colors
		randColor = true;
		glutPostRedisplay();
		// Explode brick
	}
}

// Our launch animation for the brick
void launch(int value) {
	// Number of animation steps
	// If the brick is in the air update the position
	if (isAirborn) {
		if (brickX + brickWidth >= WINDOW_WIDTH || brickX - brickWidth <= 0) {
			// bounce off wall
			xVelocity *= -1;
		}

		if (brickY + brickHeight <= 0) {
			// bounce off ceiling
			yVelocity *= -1;
		}

		// Update the bricks position
		airTime = (clock() - startTime) / (double)CLOCKS_PER_SEC;
		downwardForce = GRAVITY * (airTime*airTime);
		brickX += xVelocity;
		brickY += yVelocity - downwardForce;

		rotateBrick(xRotateSpeed, yRotateSpeed, zRotateSpeed);

		// Check if the brick has hit the ground
		if (brickY + brickWidth >= WINDOW_HEIGHT) {
			isAirborn = false;
			landed = true;
			xVelocity = 0;
			yVelocity = 0;
			animationStep = 0;
		}
	}
	else if (landed) {
		landingAnimation();
		animationStep++;
	}
	glutTimerFunc(TIME, launch, 0);
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
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && brickSelected) {
		// Update brick velocity variables for launch
		int deltaX = x - initialX;
		int deltaY = y - initialY;
		brickSelected = false;
		xVelocity = -(deltaX)/10;
		yVelocity = -(deltaY)/10;
		// Start timer to simulate gravity
		startTime = clock();
		isAirborn = true;

		glutPostRedisplay();
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		cout << x << ", " << y << endl;
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
	else if (key == 'r') {
		// Reset all variables
		brickX = INIT_X;
		brickY = INIT_Y;
		xVelocity = 0;
		yVelocity = 0;
		downwardForce = GRAVITY;
		randColor = false;
		// Rotate back to original angle
		rotateBrick(-xangle, -yangle, -zangle);
	}

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
	glutTimerFunc(TIME, launch, 0);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	init();
	glutMainLoop();
	return 0;
}