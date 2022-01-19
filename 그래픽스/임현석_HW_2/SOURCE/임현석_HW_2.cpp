#include "glSetup.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <iostream>
using namespace std;
#define _USE_MATH_DEFINES
#include <math.h>

vec4 p[36][18];
vec3 N[36][18];

vec4 lightInitialP(5,3.5, 2, 1);
bool lightOn[3];
bool attenuation = false;
bool exponent = false;
bool rotationlight = true;
float exponentinitial = 0.0;
float exponentValue = exponentinitial;
float exponentNorm = exponentValue / 128.0;
bool cutoff = false;
float cutoffMax = 60;
float cutoffInitial = 30.0;
float cutoffValue = cutoffInitial;
float cutoffNorm = cutoffValue / cutoffMax;

float thetaLight[3];

void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);
void savePoint();
void saveNormalVector();

vec3 eye(7, 8, 7);
vec3 center(0, 0, 0);
vec3 up(0, 1, 0);

float AXIS_LENGTH = 3;
float AXIS_LINE_WIDTH = 2;

GLfloat bgColor[4] = { 1,1,1,1 };
int selection = 1;

bool pause = false;
float period = 4.0;
int frame = 0;
GLfloat mat_shininess = 100;
bool shininess = true;

int xF = 35, xE = 1;
int yF = 17, yE = 1;

int tog = 0;
int Stog = 0;
float timeStep = 1.0 / 120;

void init();
void reinitialize();

void animate()
{
	frame += 1;
	if(rotationlight)
		for (int i = 0; i < 3; i++)
		{
			if (lightOn[i])
				thetaLight[i] += 4 / period;
		}
	if (lightOn[2] && exponent)
	{
		exponentNorm += radians(4.0 / period) / M_PI;
		exponentValue = 128.0 * (acos(cos(exponentNorm * M_PI)) / M_PI);
	}
	if (lightOn[2] && cutoff)
	{
		cutoffNorm += radians(4.0 / period) / M_PI;
		cutoffValue = cutoffMax * (acos(cos(cutoffNorm * M_PI)) / M_PI);
	}
	if (Stog == 1)
	{
		if (shininess)
			mat_shininess+=2;
		else
			mat_shininess-=2;
		if (mat_shininess == 128 || mat_shininess == 0)
			shininess =!shininess;
	}
}

int main(int argc, char* argv[])
{
	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL)
		return -1;
	glfwSetKeyCallback(window, keyboard);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	reshape(window, windowW, windowH);
	savePoint();
	saveNormalVector();
	init();

	float previous = glfwGetTime();
	float elapsed = 0;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		float now = glfwGetTime();
		float delta = now - previous;
		previous = now;
		elapsed += delta;
		if (elapsed > timeStep)
		{
			if (!pause)
				animate();
			elapsed = 0;
		}
		render(window);
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

GLUquadricObj* sphere = NULL;
GLUquadricObj* cylinder = NULL;
GLUquadricObj* cone = NULL;

void reinitialize()
{
	lightOn[0] = true;
	lightOn[1] = false;
	lightOn[2] = false;
	for (int i = 0; i < 3; i++)
		thetaLight[i] = 0;
	exponentValue = exponentinitial;
	exponentNorm = exponentValue / 128.0;
	cutoffValue = cutoffInitial;
	cutoffNorm = cutoffValue / cutoffMax;
}
void init()
{
	reinitialize();
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricOrientation(sphere, GLU_OUTSIDE);
	gluQuadricTexture(sphere, GL_FALSE);

	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricNormals(cylinder, GLU_SMOOTH);
	gluQuadricOrientation(cylinder, GLU_OUTSIDE);
	gluQuadricTexture(cylinder, GL_FALSE);

	cone = gluNewQuadric();
	gluQuadricDrawStyle(cone, GLU_FILL);
	gluQuadricNormals(cone, GLU_SMOOTH);
	gluQuadricOrientation(cone, GLU_OUTSIDE);
	gluQuadricTexture(cone, GL_FALSE);
}

void drawSphere(float radius, int slices, int stacks)
{
	gluSphere(sphere, radius, slices, stacks);
}
void drawCylinder(float radius, float height, int slices, int stacks)
{
	gluCylinder(cylinder, radius, radius, height, slices, stacks);
}
void drawCone(float radius, float height, int slices, int stacks)
{
	gluCylinder(cone, 0, radius, height, slices, stacks);
}

void setuplight(const vec4& p, int i)
{
	GLfloat ambient[4] = { 0.1,0.1,0.1,1 };
	GLfloat diffuse[4] = { 0.8,0.8,0.8,1 };
	GLfloat specular[4] = { 0.8,0.8,0.8,1 };

	glLightfv(GL_LIGHT0 + i, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0 + i, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0 + i, GL_POSITION, value_ptr(p));
	if (i == 0 && attenuation)
	{
		glLightf(GL_LIGHT0 + i, GL_CONSTANT_ATTENUATION, 1.0);
		glLightf(GL_LIGHT0 + i, GL_LINEAR_ATTENUATION, 0.1);
		glLightf(GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, 0.05);
	}
	else
	{
		glLightf(GL_LIGHT0 + i, GL_CONSTANT_ATTENUATION, 1.0);
		glLightf(GL_LIGHT0 + i, GL_LINEAR_ATTENUATION, 0.0);
		glLightf(GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, 0.0);
	}
	if (i == 2)
	{
		vec3 spotDirection = -vec3(p);
		glLightfv(GL_LIGHT0 + i, GL_SPOT_DIRECTION, value_ptr(spotDirection));
		glLightf(GL_LIGHT0 + i, GL_SPOT_CUTOFF, cutoffValue);
		glLightf(GL_LIGHT0 + i, GL_SPOT_EXPONENT, exponentValue);
	}
	else
	{
		glLightf(GL_LIGHT0 + i, GL_SPOT_CUTOFF, 180);
	}
}

void setupColoredMaterial(const vec3& color)
{
	GLfloat mat_ambient[4] = { 0.1,0.1,0.1,1 };
	GLfloat mat_diffuse[4] = { color[0],color[1],color[2],1 };
	GLfloat mat_specular[4] = { 0.8,0.8,0.8,1 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void computeRotation(const vec3& a, const vec3& b, float& theta, vec3& axis)
{
	axis = cross(a, b);
	float sinTheta = length(axis);
	float cosTheta = dot(a, b);
	theta = atan2(sinTheta, cosTheta) * 180 / M_PI;
}

void drawArrow(const vec3& p, bool tailOnly)
{
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	GLfloat mat_specular[4] = { 1,1,1,1 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glPushMatrix();
	glTranslatef(p.x, p.y, p.z);
	if (!tailOnly)
	{
		float theta;
		vec3 axis;
		computeRotation(vec3(0, 0, 1), vec3(0, 0, 0) - vec3(p), theta, axis);
		glRotatef(theta, axis.x, axis.y, axis.z);
	}
	float arrowTailRadius = 0.05;
	glColor3f(1, 0, 0);
	drawSphere(arrowTailRadius, 16, 16);
	if (!tailOnly)
	{
		float arrowshaftRadius = 0.02;
		float arrowShaftLength = 0.2;
		glColor3f(0, 1, 0);
		drawCylinder(arrowshaftRadius, arrowShaftLength, 16, 5);

		float arrowheadHeight = 0.09;
		float arrowheadRadius = 0.06;
		glTranslatef(0, 0, arrowShaftLength + arrowheadHeight);
		glRotatef(180, 1, 0, 0);
		glColor3f(0, 0, 1);
		drawCone(arrowheadRadius, arrowheadHeight, 16, 5);
	}
	glColor3f(1,1,1);
	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);
}

void drawSpotLight(const vec3& p, float cutoff)
{
	glPushMatrix();
	glTranslatef(p.x, p.y, p.z);
	float theta;
	vec3 axis;
	computeRotation(vec3(0, 0, 1), vec3(0, 0, 0) - vec3(p), theta, axis);
	glRotatef(theta, axis.x, axis.y, axis.z);
	setupColoredMaterial(vec3(1,1,1));
	float h = 1.5;
	float r = h * tan(radians(cutoff))/2;
	drawCone(r, h, 16, 5);
	setupColoredMaterial(vec3(1,1,1));
	float apexRadius = 0.06 * (0.5 + exponentValue / 128.0);
	drawSphere(apexRadius, 16, 16);
	glPopMatrix();;
}

void savePoint()
{
	mat4 trans_first = mat4(1.0);
	mat4 trans_second = mat4(1.0);
	mat4 rotate_circle = mat4(1.0);
	mat4 rotate_donut = mat4(1.0);

	trans_first = glm::translate(trans_first, vec3(-2, -2, 0));
	trans_second = glm::translate(trans_second, vec3(2, 2, 0));
	rotate_circle = glm::rotate(rotate_circle, glm::radians(20.0f), vec3(0, 0, 1));
	rotate_donut = glm::rotate(rotate_donut, glm::radians(10.0f), vec3(0, 1, 0));
	p[0][0] = vec4(1.5, 1.5, 0, 1);
	for (int j = 1; j < 36; j++)
	{
		p[j][0] = rotate_donut * p[j-1][0];
	}
	for (int i = 1; i < 36; i++)
	{
		for (int a = 1; a < 18; a++)
		{
			p[0][a] = trans_second * rotate_circle * trans_first * p[0][a-1];
			p[i][a] = rotate_donut * p[i-1][a];
		}
	}
}

void saveNormalVector()
{
	for (int i = 0; i < yF; i++)
	{
		for (int j = 0; j < xF; j++)
		{
			vec3 a = p[j + 1][i] - p[j][i];
			vec3 b = p[j][i + 1] - p[j][i];
			N[j][i] = glm::cross(a, b);
		}
	}
	for (int i = 0; i < xF; i++)
		{
			vec3 a = p[i][0] - p[i][17];
			vec3 b = p[i+1][17] - p[i][17];
			N[i][17] = glm::cross(a, b);
		}
	for (int i = 0; i < yF; i++)
		{
			vec3 a = p[0][i] - p[35][i];
			vec3 b = p[35][i+1] - p[35][i];
			N[35][i] = glm::cross(a, b);
		}
	vec3 a = p[0][17] - p[35][17];
	vec3 b = p[35][0] - p[35][17];
	N[35][17] = glm::cross(a, b);
}

void drawQuad()
{
	glColor3f(0, 0, 0);
	glPolygonOffset(1.f, 1.f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	{
		for (int i = 0; i < yF; i++)
		{
			for (int j = 0; j < xF; j++)
			{
				glNormal3f(N[j][i].x, N[j][i].y, N[j][i].z);
				glVertex3f(p[j][i].x, p[j][i].y, p[j][i].z);
				glVertex3f(p[j + 1][i].x, p[j + 1][i].y, p[j + 1][i].z);
				glVertex3f(p[j + 1][i + 1].x, p[j + 1][i + 1].y, p[j + 1][i + 1].z);
				glVertex3f(p[j][i + 1].x, p[j][i + 1].y, p[j][i + 1].z);
			}
		}
	}
	if (yF == 17 && yE == 1)
		for (int a = 0; a < xF; a++)
		{
			glNormal3f(N[a][17].x, N[a][17].y, N[a][17].z);
			glVertex3f(p[a][0].x, p[a][0].y, p[a][0].z);
			glVertex3f(p[a + 1][0].x, p[a + 1][0].y, p[a + 1][0].z);
			glVertex3f(p[a + 1][17].x, p[a + 1][17].y, p[a + 1][17].z);
			glVertex3f(p[a][17].x, p[a][17].y, p[a][17].z);
		}
	if (xF == 35 && xE == 1)
		for (int a = 0; a < yF; a++)
		{
			glNormal3f(N[35][a].x, N[35][a].y, N[35][a].z);
			glVertex3f(p[35][a].x, p[35][a].y, p[35][a].z);
			glVertex3f(p[0][a].x, p[0][a].y, p[0][a].z);
			glVertex3f(p[0][a + 1].x, p[0][a + 1].y, p[0][a + 1].z);
			glVertex3f(p[35][a + 1].x, p[35][a + 1].y, p[35][a + 1].z);
		}
	glEnd();
}

void drawNormalVector()
{
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	{
		for (int i = 0; i < 36; i++)
		{
			for (int j = 0; j < 18; j++)
			{
				glVertex3f(p[i][j].x, p[i][j].y, p[i][j].z);
				glVertex3f(p[i][j].x + N[i][j].x*8, p[i][j].y + N[i][j].y*8, p[i][j].z + N[i][j].z*8);
			}
		}
	}
	glEnd();
}

void render(GLFWwindow* window)
{
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
	
	glDisable(GL_LIGHTING);
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);
	
	glShadeModel(GL_SMOOTH);

	vec3 axis(0, 1, 0);
	glEnable(GL_LIGHTING);
	vec4 lightP[3];
	for (int i = 0; i < 3; i++)
	{
		if (!lightOn[i])
		{
			glDisable(GL_LIGHT0 + i);
			continue;
		}
		glEnable(GL_LIGHT0 + i);
		lightP[i] = lightInitialP;
		if (i == 1)
			lightP[i].w = 0;
		mat4 R = rotate(mat4(1.0), radians(thetaLight[i]), axis);;
		lightP[i] = R * lightP[i];
		setuplight(lightP[i], i);
	}
	for (int i = 0; i < 3; i++)
	{
		if (!lightOn[i])
			continue;
		if (i == 2)
		{
			drawSpotLight(lightP[i], cutoffValue);
		}
		else
			drawArrow(lightP[i], i == 0);
	}
	
	switch (selection)
	{
	case 1:
		drawQuad();
		if (tog == 1)
			drawNormalVector();
		break;
	}
}

void keyboard(GLFWwindow* window, int key, int code, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		
		case GLFW_KEY_N:
			selection = 1;
			if (tog == 1)
				tog = 0;
			else
				tog = 1;
			break;
		case GLFW_KEY_P:
			lightOn[0] = !lightOn[0];
			break;
		case GLFW_KEY_D:
			lightOn[1] = !lightOn[1];
			break;
		case GLFW_KEY_S:
			lightOn[2] = !lightOn[2];
			break;
		case GLFW_KEY_T:
			if (Stog == 1)
				Stog = 0;
			else
				Stog = 1;
			break;
		}
	}
}
