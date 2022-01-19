#include "glSetup.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <iostream>
using namespace std;
#define _USE_MATH_DEFINES
#include <math.h>

vec4 p[36][18];

void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);
void savePoint();

vec3 eye(7, 6, 7);
vec3 center(0, 0, 0);
vec3 up(0, 1, 0);

float AXIS_LENGTH = 3;
float AXIS_LINE_WIDTH = 2;

GLfloat bgColor[4] = { 1,1,1,1 };
int selection = 1;

bool pause = false;
float period = 4.0;
int frame = 0;

int xF = 35, xE = 1;
int yF = 17, yE = 1;

int main(int argc, char* argv[])
{
	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL)
		return -1;
	glfwSetKeyCallback(window, keyboard);
	glEnable(GL_DEPTH_TEST);
	reshape(window, windowW, windowH);
	savePoint();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		render(window);
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

//mat4 trans = mat4(1.0);
//
//transg = glm::translate(trans, vec3(-5, -5, 0));
//transf = glm::translate(trans, vec3(5, 5, 0));
//
//		trans = glm::rotate(trans, glm::radians(20.0f), vec3(0, 0, 1));
//		p [i][j + 1] = transf * trans * transg * p[i][0];
//
//mat4 trans = mat4(1.0);
//trans2 = glm::rotate(trans2, glm::radians(10.0f), vec3(0, 1, 0));
//
//p[0][j]
//p[i][j] p[i][j+1] p[i+1][j+1] p[]
//0 35 i
//0 17 j
//
//0 35 0 17 1

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

void drawPoint()
{
	glColor3f(0, 0, 0);
	glPointSize(2);
	glBegin(GL_POINTS);
	{
		for (int i = 0; i < xF; i++)
		{
			for (int a = 0; a < yF; a++)
			{
				glVertex3f(p[i][a].x, p[i][a].y, p[i][a].z);
			}
		}
	}
	if (xF == 35&&xE==1)
		for (int a = 0; a < yF; a++)
			glVertex3f(p[35][a].x, p[35][a].y, p[35][a].z);
	if (yF == 17&&yE==1)
		for (int a = 0; a < xF; a++)
			glVertex3f(p[a][17].x, p[a][17].y, p[a][17].z);
	glEnd();
}

void drawLine()
{
	glLineWidth(2 * dpiScaling);
	glColor3f(0, 0, 0);
	glPolygonOffset(0.f, 0.f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	{
		for (int i = 0; i < yF; i++)
		{
			for (int j = 0; j < xF; j++)
			{
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
			glVertex3f(p[a][0].x, p[a][0].y, p[a][0].z);
			glVertex3f(p[a + 1][0].x, p[a + 1][0].y, p[a + 1][0].z);
			glVertex3f(p[a + 1][17].x, p[a + 1][17].y, p[a + 1][17].z);
			glVertex3f(p[a][17].x, p[a][17].y, p[a][17].z);
		}
	if (xF == 35 && xE == 1)
		for (int a = 0; a < yF; a++)
		{
			glVertex3f(p[35][a].x, p[35][a].y, p[35][a].z);
			glVertex3f(p[0][a].x, p[0][a].y, p[0][a].z);
			glVertex3f(p[0][a + 1].x, p[0][a + 1].y, p[0][a + 1].z);
			glVertex3f(p[35][a + 1].x, p[35][a + 1].y, p[35][a + 1].z);
		}
	glEnd();
}

void drawQuad()
{
	glColor3f(0, 0, 1);
	glPolygonOffset(1.f, 1.f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	{
		for (int i = 0; i < yF; i++)
		{
			for (int j = 0; j < xF; j++)
			{
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
			glVertex3f(p[a][0].x, p[a][0].y, p[a][0].z);
			glVertex3f(p[a + 1][0].x, p[a + 1][0].y, p[a + 1][0].z);
			glVertex3f(p[a + 1][17].x, p[a + 1][17].y, p[a + 1][17].z);
			glVertex3f(p[a][17].x, p[a][17].y, p[a][17].z);
		}
	if (xF == 35 && xE == 1)
		for (int a = 0; a < yF; a++)
		{
			glVertex3f(p[35][a].x, p[35][a].y, p[35][a].z);
			glVertex3f(p[0][a].x, p[0][a].y, p[0][a].z);
			glVertex3f(p[0][a + 1].x, p[0][a + 1].y, p[0][a + 1].z);
			glVertex3f(p[35][a + 1].x, p[35][a + 1].y, p[35][a + 1].z);
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
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);
	
	switch (selection)
	{
	case 1:
		drawPoint();
		break;
	case 2:
		drawLine();
		break;
	case 3:
		drawQuad();
		break;
	case 4:
		drawLine();
		drawQuad();
		break;
	}
}

void changeXPlus()
{
	if (xF == 35)
	{
		xF = xF;
		xE = 1;
	}	
	else
	{
		xF += 1;
	}
}
void changeXMinus()
{
	if (xF == 0)
		xF = xF;
	else if (xF == 35&&xE==1)
	{
		xE = 0;
	}
	else
		xF -= 1;
}
void changeYPlus()
{
	if (yF == 17)
	{
		yF = yF;
		yE = 1;
	}
	else
	{
		yF += 1;
	}
}
void changeYMinus()
{
	if (yF == 0)
		yF = yF;
	else if (yF == 17&&yE==1)
	{
		yE = 0;
	}
	else
		yF -= 1;
}

void keyboard(GLFWwindow* window, int key, int code, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_1:
			selection = 1;
			break;
		case GLFW_KEY_2:
			selection = 2;
			break;
		case GLFW_KEY_3:
			selection = 3;
			break;
		case GLFW_KEY_4:
			selection = 4;
			break;
		case GLFW_KEY_A:
			changeXPlus();
			break;
		case GLFW_KEY_S:
			changeXMinus();
			break;
		case GLFW_KEY_J:
			changeYPlus();
			break;
		case GLFW_KEY_K:
			changeYMinus();
			break;
		}
	}
}