#define _CRT_SECURE_NO_WARNINGS
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

void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);
void savePoint();
void saveNormalVector();
void animate();
void init();
void loadcheckerboardtexture();
void loadwoodtexture();
void loadmarbletexture();

vec3 eye(7, 8 ,7);
vec3 center(0, 0, 0);
vec3 up(0, 1, 0);
vec4 light(7, 8, 7, 0);

float AXIS_LENGTH = 3;
float AXIS_LINE_WIDTH = 2;

GLfloat bgColor[4] = { 1,1,1,1 };

bool pause = true;
float period = 4.0;
int frame = 0;
float timeStep = 1.0 / 120;

int xF = 35, xE = 1;
int yF = 17, yE = 1;

GLuint texID;

void animate()
{
	frame += 1;
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

void setupLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat ambient[4] = { 0.1,0.1,0.1,1 };
	GLfloat diffuse[4] = { 0.8,0.8,0.8,1 };
	GLfloat specular[4] = { 0.8,0.8,0.8,1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(light));
}

void init()
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures(2, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	setupLight();
	loadmarbletexture();
}

void loadcheckerboardtexture()
{
	FILE* f1 = fopen("check.raw", "rb");
	if (f1 == NULL)
		return;
	unsigned char* checkerImage;
	checkerImage = (unsigned char*)malloc(512 * 512 * 3);
	fread(checkerImage, 512 * 512 * 3, 1, f1);
	fclose(f1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, checkerImage);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
void loadwoodtexture()
{
	FILE* f1 = fopen("wood.raw", "rb");
	if (f1 == NULL)
		return;
	unsigned char* woodimage;
	woodimage = (unsigned char*)malloc(512 * 512 * 3);
	fread(woodimage, 512 * 512 * 3, 1, f1);
	fclose(f1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, woodimage);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
void loadmarbletexture()
{
	FILE* f1 = fopen("marble.raw", "rb");
	if (f1 == NULL)
		return;
	unsigned char* marbleimage;
	marbleimage = (unsigned char*)malloc(512 * 512 * 3);
	fread(marbleimage, 512 * 512 * 3, 1, f1);
	fclose(f1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, marbleimage);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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
	glPolygonOffset(1.f, 1.f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, texID);
	glBegin(GL_QUADS);
	{
		for (int i = 0; i < yF; i++)
		{
			for (int j = 0; j < xF; j++)
			{
				glNormal3f(N[j][i].x, N[j][i].y, N[j][i].z);
				glTexCoord2fv(value_ptr(vec2(j / 36.0, i / 18.0)));
				glVertex3f(p[j][i].x, p[j][i].y, p[j][i].z);
				glTexCoord2fv(value_ptr(vec2((j+1) / 36.0, i / 18.0)));
				glVertex3f(p[j + 1][i].x, p[j + 1][i].y, p[j + 1][i].z);
				glTexCoord2fv(value_ptr(vec2((j+1) / 36.0, (i+1) / 18.0)));
				glVertex3f(p[j + 1][i + 1].x, p[j + 1][i + 1].y, p[j + 1][i + 1].z);
				glTexCoord2fv(value_ptr(vec2(j / 36.0, (i+1) / 18.0)));
				glVertex3f(p[j][i + 1].x, p[j][i + 1].y, p[j][i + 1].z);
			}
		}
	}
	if (yF == 17 && yE == 1)
		for (int a = 0; a < xF; a++)
		{
			glNormal3f(N[a][17].x, N[a][17].y, N[a][17].z);
			glTexCoord2fv(value_ptr(vec2(a / 36.0, 1.0)));
			glVertex3f(p[a][0].x, p[a][0].y, p[a][0].z);
			glTexCoord2fv(value_ptr(vec2((a+1) / 36.0, 1.0)));
			glVertex3f(p[a + 1][0].x, p[a + 1][0].y, p[a + 1][0].z);
			glTexCoord2fv(value_ptr(vec2((a+1) / 36.0, 17.0 / 18.0)));
			glVertex3f(p[a + 1][17].x, p[a + 1][17].y, p[a + 1][17].z);
			glTexCoord2fv(value_ptr(vec2(a / 36.0, 17.0 / 18.0)));
			glVertex3f(p[a][17].x, p[a][17].y, p[a][17].z);
		}
	if (xF == 35 && xE == 1)
		for (int a = 0; a < yF; a++)
		{
			glNormal3f(N[35][a].x, N[35][a].y, N[35][a].z);
			glTexCoord2fv(value_ptr(vec2(35.0 / 36.0, a / 18.0)));
			glVertex3f(p[35][a].x, p[35][a].y, p[35][a].z);
			glTexCoord2fv(value_ptr(vec2(1.0, a / 18.0)));
			glVertex3f(p[0][a].x, p[0][a].y, p[0][a].z);
			glTexCoord2fv(value_ptr(vec2(1.0, (a+1) / 18.0)));
			glVertex3f(p[0][a + 1].x, p[0][a + 1].y, p[0][a + 1].z);
			glTexCoord2fv(value_ptr(vec2(35.0 / 36.0, (a+1) / 18.0)));
			glVertex3f(p[35][a + 1].x, p[35][a + 1].y, p[35][a + 1].z);
		}
	glEnd();
}

void drawNormalVector()
{
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

	setupLight();
	drawQuad();
}



void keyboard(GLFWwindow* window, int key, int code, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		
		case GLFW_KEY_1:
			loadmarbletexture();
			break;
		case GLFW_KEY_2:
			loadwoodtexture();
			break;
		case GLFW_KEY_3:
			loadcheckerboardtexture();
			break;
		}
	}
}
