// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <vector>

#include "PlaneMesh.hpp"

// Include GLFW and GLEW
#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


// Variables for camera
float last_mouse_x_position = 0;
float last_mouse_y_position = 0;

float x_angle = 45.0f;
float y_angle = 45.0f;
float radius = 10;

float eye_x, eye_y, eye_z;

glm::vec3 eye = {4.0f, 0.0f, 0.0f};
glm::vec3 center = {0.0f, 0.0f, 0.0f};
glm::vec3 up = {0.0f, 1.0f, 0.0f};

// Open a window and create its OpenGL context
float screen_width = 1400;
float screen_height = 900;

void cameraControls(mat4& V){

//        // Do user input
    if(glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS && radius > 0.1){
        radius-=0.2;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS){
        radius+=0.2;
    }
//        printf("ANGLE: X:%f, Y:%f\n",x_angle, y_angle);

    // Camera manipulation
    float x_rad = radians(x_angle);
    float y_rad = radians(y_angle);
    eye_y = radius * cos(y_rad);
    eye_x = radius * cos(x_rad) * sin(y_rad);
    eye_z = radius * sin(x_rad) * sin(y_rad);
    eye = {eye_x, eye_y, eye_z};

    V = glm::lookAt(eye, center, up);

}


static void cursor_position_callback(GLFWwindow* callback_window, double xpos, double ypos)
{
//    std::cout << glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) << std::endl;
//    If the mouse is not clicked, don't move the camera
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        return;
    }
    glfwGetCursorPos(window, &xpos, &ypos);

    float delta_x;
    float delta_y;

    if(last_mouse_x_position != 0){
        delta_x = last_mouse_x_position - xpos;

    }else{
        delta_x = xpos;
    }
    if(last_mouse_y_position != 0){
        delta_y = last_mouse_y_position - ypos;
    }else{
        delta_y = ypos;
    }

//    Disregard big jumpy movements
    if(delta_x > 60){
        delta_x = 0;
    }
    if(delta_x < -60){
        delta_x = 0;
    }
    if(delta_y > 60){
        delta_y = 0;
    }
    if(delta_y < -60){
        delta_y = 0;
    }

    x_angle -= delta_x/3;
    y_angle += delta_y/3;

    if(y_angle < 5){
        y_angle = 5;
    }else if (y_angle > 175){
        y_angle = 175;
    }
    last_mouse_x_position = xpos;
    last_mouse_y_position = ypos;

}

int init_opengl(){

    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( screen_width, screen_height, "Waves", NULL, NULL);
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
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // Dark blue background
    glClearColor(0.2f, 0.2f, 0.3f, 0.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);

    return 0;
}

//////////////////////////////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[])
{

	///////////////////////////////////////////////////////
    float xmin = -10;
    float xmax = 10;
	float stepsize = 1.0f;
    ///////////////////////////////////////////////////////
	if (argc > 1 ) {
		screen_width = atoi(argv[1]);
	}
	if (argc > 2) {
		screen_height = atoi(argv[2]);
	}
	if (argc > 3) {
		stepsize = atof(argv[3]);
	}
	if (argc > 4) {
		xmin = atof(argv[4]);
	}
	if (argc > 5) {
		xmax = atof(argv[5]);
	}
    init_opengl();
	///////////////////////////////////////////////////////

	// Camera
    glm::mat4 M = glm::mat4(1.0f);
    glm::mat4 V;
	glm::mat4 P = glm::perspective(glm::radians(45.0f), screen_width/screen_height, 0.001f, 1000.0f);


    glm::vec3 lightpos(10.0f, 20.0f, 5.0f);
    glm::vec4 color1(1.0f, 0.1f, 0.1f, 0.5f);

	///////////////////////////////////////////////////////

    glPatchParameteri(GL_PATCH_VERTICES, 4);

    PlaneMesh water(xmin, xmax, stepsize);

    float time = 0;

//    This is to show the wireframe of the waves
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    do{
        i = 1;
        loop:
        std::cout << (2 << 4 << 3) << endl;
        time += 0.04;
        int8_t a = 200;
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cameraControls(V);

        glLoadMatrixf(glm::value_ptr(P));
        glLoadMatrixf(glm::value_ptr(V));

        water.draw(lightpos, M, V, P, color1, time);

        // Swap buffers and poll
        glfwSwapBuffers(window);
        glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

