// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <iostream>
#include <vector>

#define TOP_LEFT     8
#define TOP_RIGHT    4
#define BOTTOM_RIGHT 2
#define BOTTOM_LEFT  1

typedef float (*scalar_field_2d)(float, float);

int marching_squares_lut[16][4] = {
	{-1, -1, -1, -1},
	{2, 3, -1, -1},
	{1, 2, -1, -1},
	{1, 3, -1, -1},
	{0, 1, -1, -1},
	{0, 1, 2, 3},
	{0, 2, -1, -1},
	{0, 3, -1, -1},
	{0, 3, -1, -1},
	{0, 2, -1, -1},
	{0, 3, 1, 2},
	{0, 1, -1, -1},
	{1, 3, -1, -1},
	{1, 2, -1, -1},
	{2, 3, -1, -1},
	{-1, -1, -1, -1}
};

float g_verts[4][2] = {
	{0.5f, 1.0f},
	{1.0f, 0.5f},
	{0.5f, 0.0f},
	{0.0f, 0.5f}
};

float f1(float x, float y) {
	return x*x + y*y;
}

float f2(float x, float y) {
	return sin(x*y);
}

float f3(float x, float y) {
	return sin(x)*cos(y);
}

std::vector<float> marching_squares(scalar_field_2d f, float isoval, float minx, float maxx, float miny, float maxy, float stepsizex, float stepsizey) {

	std::vector<float> vertices;

	float x = minx;
	float y = miny;
	float tl, tr, br, bl;
	int which = 0;
	int* verts;
	for ( ; y < maxy; y += stepsizey) {
		for (x = minx ; x < maxx; x += stepsizex) {
			//test the square
			tl = (*f)(x, y+stepsizey);
			tr = (*f)(x+stepsizex, y+stepsizey);
			br = (*f)(x+stepsizex, y);
			bl = (*f)(x, y);

			which = 0;
			if (tl < isoval) {
				which |= TOP_LEFT;
			}
			if (tr < isoval) {
				which |= TOP_RIGHT;
			}
			if (br < isoval) {
				which |= BOTTOM_RIGHT;
			}
			if (bl < isoval) {
				which |= BOTTOM_LEFT;
			}

			verts = marching_squares_lut[which];
			if (verts[0] >= 0) {
				vertices.emplace_back(x+stepsizex*g_verts[verts[0]][0]);
				vertices.emplace_back(y+stepsizey*g_verts[verts[0]][1]);
				vertices.emplace_back(x+stepsizex*g_verts[verts[1]][0]);
				vertices.emplace_back(y+stepsizey*g_verts[verts[1]][1]);
			}
			if (verts[2] >= 0) {
				vertices.emplace_back(x+stepsizex*g_verts[verts[2]][0]);
				vertices.emplace_back(y+stepsizey*g_verts[verts[2]][1]);
				vertices.emplace_back(x+stepsizex*g_verts[verts[3]][0]);
				vertices.emplace_back(y+stepsizey*g_verts[verts[3]][1]);
			}
		}
	}

	return vertices;
}

//////////////////////////////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[])
{

	///////////////////////////////////////////////////////
	float screenW = 900;
	float screenH = 900;
	float xstepsize = 0.4f;
	float ystepsize = 0.8f;

	float xmin = -10;
	float xmax = 10;
	float isoval = 1;

	if (argc > 1 ) {
		screenW = atoi(argv[1]);
	}
	if (argc > 2) {
		screenH = atoi(argv[2]);
	}
	if (argc > 3) {
		xstepsize = atof(argv[3]);
	}
	if (argc > 4) {
		xmin = atof(argv[4]);
	}
	if (argc > 5) {
		xmax = atof(argv[5]);
	}
	if (argc > 6) {
		isoval = atof(argv[6]);
	}
	float ymin = xmin;
	float ymax = xmax;
	///////////////////////////////////////////////////////

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( screenW, screenH, "Marching", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}


	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.2f, 0.2f, 0.3f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(xmin, xmax, ymin, ymax, -1, 1);

	std::vector<float> marchingVerts = marching_squares(f1, isoval, xmin, xmax, ymin, ymax, xstepsize, ystepsize);



	do{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        Draw the lines
		glLineWidth(0.5f);
        glBegin(GL_LINES);
        for (float y = ymin; y < ymax; y += ystepsize) {
            for (float x = xmin; x < xmax; x += xstepsize) {
                glVertex2f(x, y);
                glVertex2f(x+xmax, y);
            }
        }
        for (float y = ymin; y < ymax; y += ystepsize) {
            for (float x = xmin; x < xmax; x += xstepsize) {
                glVertex2f(x, y);
                glVertex2f(x, y + ymax);
            }
        }
        glEnd();
		glLineWidth(2.0f);
		glBegin(GL_LINES);
			for (size_t i = 0; i < marchingVerts.size(); i += 2) {
				glVertex2f(marchingVerts[i], marchingVerts[i+1]);
			}
		glEnd();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

