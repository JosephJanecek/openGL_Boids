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
#include "Boid.h"
#include <vector>
#include <list>
#include <time.h>
#include <stdlib.h>
#include <cmath>
using namespace std;

#define PI 3.14159

const int frameRate = 30;
//time_t startTime = clock();

float vertices[][3] = { {0, -.015f, 0}, {.05f, 0, 0}, {0, .015f, 0} };
//vector<vec3> normal;
vector<int3> triangles;
vector<Boid> flock;

int vertexSize = sizeof(vertices);
//int normalsSize;

GLuint vBufferID = 0;
GLuint shaderID = 0;

const float maxSpeed = .01f;
const float avgSpeed = .005f;
mat4 transMat;
int transID = 0;
int change = 0;

char* vertexShader = "\
	#version 130									\n\
	in vec3 vPos;									\n\
	//in vec3 color;								\n\
	out vec4 oColor;								\n\
	uniform mat4 view;								\n\
	void main()										\n\
	{												\n\
		gl_Position = view * vec4 (vPos, 1);				\n\
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

//Generates a random angle for the velocity but keeps all speeds the same (avgSpeed)
vec2 randAngle()
{
	vec2 v;
	double theta = ((rand() % 200) / 100.0f) * PI;
	
	v.x = avgSpeed * cos(theta);
	v.y = avgSpeed * sin(theta);

	return v;
}

void mouseClick(int button, int state, int x, int y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
		{
			if (state == GLUT_DOWN)
			{
				vec2 mousePos = vec2(((float)x - 450) / 450, ((float)y - 300) / -300);

				vec2 v = randAngle();
				Boid b(vec3(mousePos, 1), vec3(v, 0), maxSpeed);
				flock.push_back(b);
			}
			break;
		}
		default:
			break;
	}
	glutPostRedisplay();
}

int timePassed = 0;

void animate()
{
	glutPostRedisplay();
}

//-------------------------------- Boid Rules --------------------------------------------------
//Rule 1: Cohesion
vec3 boidCohesion(Boid b, int myIndex)
{
	vec3 pc = vec3(0);
	int fSize = flock.size();

	for (int i = 0; i < fSize; i++)
	{
		if (i != myIndex)
		{
			pc += flock[i].position;
		}
	}
	pc /= fSize - 1;

	//Move 1% closer to the flock for smother animation
	return (pc - b.position) / 100;
}

//Rule 2: Separation
vec3 boidSeparation(Boid b, int myIndex)
{
	vec3 sep = vec3(0);

	for (int i = 0; i < flock.size(); i++)
	{
		if (i != myIndex)
		{
			if (abs(flock[i].position.x - b.position.x) < 100 || abs(flock[i].position.y - b.position.y) < 100)
			{
				sep -= flock[i].position - b.position;
			}
		}
	}
	return sep;
}

//Rule 3: Alignment
vec3 boidAlignment(Boid b, int myIndex)
{
	vec3 pv = vec3(0);

	int fSize = flock.size();

	for (int i = 0; i < fSize; i++)
	{
		if (i != myIndex)
		{
			pv += flock[i].velocity;
		}
	}
	pv /= fSize - 1;

	//Add only an eight of the velocity
	return (pv - b.velocity) / 8;
}
//----------------------------------------------------------------------------------------------

void Display()
{
	//Clear color buffer
	glClearColor(.3f, .3f, .3f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//Draw all boids at their respective position;
	for (int i = 0; i < flock.size(); i++)
	{
		
		if (timePassed % frameRate == 0)
		{
			/*
			vec3 v1 = boidCohesion(flock[i], i);
			vec3 v2 = boidSeparation(flock[i], i);
			vec3 v3 = boidAlignment(flock[i], i);
			flock[i].velocity += v1 +v2 + v3;
			*/
			flock[i].accelerate();
		}

		transMat = Translate(flock[i].position);
		transMat *= flock[i].direction();
		GLint viewID = glGetUniformLocation(shaderID, "view");
		if (viewID >= 0)
			glUniformMatrix4fv(viewID, 1, true, (float*)&transMat);

		glDrawElements(GL_TRIANGLES, (3 * flock.size()), GL_UNSIGNED_INT, &triangles[0]);
	}

	timePassed++;
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
	srand(time(nullptr));
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
	SetupVertexFeeder();

	triangles.push_back(int3(0, 1, 2));

	glutMouseFunc(mouseClick);
	//glutMotionFunc(mouseDrag);

	// set GLUT display callback
	glutDisplayFunc(Display);
	glutIdleFunc(animate);
	glutCloseFunc(Close);
	// no return from this event loop
	glutMainLoop();
}

