/*
BoidsEnv: Simple Boid program that draws a new boid when the left mouse button is clicked
Author: Joseph Janecek
October 15, 2017
Version 1
*/

#include "GL\glew.h"
#include "GL\freeglut.h"
#include "GL\vec.h"
#include "GL\GLSL.h"
#include <vector>
using namespace std;

float vertices[][3] = { {-.015f, 0, 0}, {0, .05f, 0}, {.015f, 0, 0} };
//vector<vec3> normal;
vector<int3> triangles;

int vertexSize = sizeof(vertices);
//int normalsSize;

GLuint vBufferID = 0;
GLuint shaderID = 0;

const float maxSpeed = .3f;

vec2 mouseDown;

char* vertexShader = "\
	#version 130									\n\
	in vec3 vPos;									\n\
	//in vec3 color;									\n\
	out vec4 oColor;								\n\
	void main()										\n\
	{												\n\
		gl_Position = vec4 (vPos, 1);				\n\
		oColor = vec4(1,.5f,.2f,1);						\n\
	}												\n\
";

char* fragmentShader = "\
	#version 130									\n\
	in vec4 oColor;									\n\
	out vec4 fColor;									\n\
	void main()										\n\
	{												\n\
		fColor = oColor;								\n\
	}												\n\
";

void InitVertexBuffer()
{
	//Create Buffers
	glGenBuffers(1, &vBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vBufferID);

	//alocate memory
	glBufferData(GL_ARRAY_BUFFER, vertexSize, NULL, GL_STATIC_DRAW);

	//Load Data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, vertices);
}

void SetupVertexFeeder()
{
	GLuint position = glGetAttribLocation(shaderID, "vPos");
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

}

void Display()
{
	glClearColor(.3f, .3f, .3f, 1);
	//Clear the frame buffer
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	SetupVertexFeeder();

	//int3 tri = { 0, 1, 2 };

	triangles.push_back(int3(0,1,2));

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &triangles[0]);

	// ensure all gl commands executed
	glFlush();
}

void Close()
{
	glBindBuffer(GL_ARRAY_BUFFER, vBufferID);
	glDeleteBuffers(1, &vBufferID);
}

void main(int argc, char** argv)
{
	//Initialize libraries and window
	glutInit(&argc, argv);
	glutInitWindowSize(900, 600);
	glutCreateWindow("Boids");
	glewInit();

	// build shader program from inline GLSL code
	shaderID = InitShader(vertexShader, fragmentShader);
	if (!shaderID)
	{
		// report error
		printf("Error linking shader program\n");
		getchar();
	}

	// enable shader program
	glUseProgram(shaderID);

	InitVertexBuffer();
	//glutMouseFunc(mouseButton);
	//glutMotionFunc(mouseDrag);
	// set GLUT display callback
	glutDisplayFunc(Display);
	glutCloseFunc(Close);
	// no return from this event loop
	glutMainLoop();
}

