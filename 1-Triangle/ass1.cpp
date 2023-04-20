#ifdef __APPLE__
// Defined before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#endif

#include <iostream>

const size_t WIDTH = 1920;
const size_t HEIGHT = 1080;
const char* WINDOW_NAME = "Test OpenGL";

/*
 * Callback to handle the "close window" event, once the user pressed the Escape key.
 */
static void quit_callback(GLFWwindow *window, int key, int scancode, int action, int _mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void error_callback( int error, const char *msg ) {
    std::string s;
    s = " [" + std::to_string(error) + "] " + msg + '\n';
    std::cerr << s << std::endl;
}

int main(void)
{
    GLFWwindow *window;

    // Minimum target is OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    if (!glfwInit()) {
        std::cerr << "ERROR: could not start GLFW3" << std::endl;
        return -1; // Initialize the lib
    }

    glfwSetErrorCallback( error_callback );
    window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_NAME, NULL, NULL);

    glMatrixMode(GL_PROJECTION);
    glOrtho(0,WIDTH, 0, HEIGHT, -1, 1);
//    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    std::cout << "Here";

    if (!window)
    {
        std::cerr << "ERROR: could not open window with GLFW3" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Close the window as soon as the Escape key has been pressed
    glfwSetKeyCallback(window, quit_callback);
    // Makes the window context current
    glfwMakeContextCurrent(window);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    // Now we have a current OpenGL context, we can use OpenGL normally
    while (!glfwWindowShouldClose(window))
    {
        // Render
        glClear(GL_COLOR_BUFFER_BIT);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.8,0.8,0.8,1);
        glColor3f(1,0,0);
        glBegin(GL_TRIANGLES);
        glVertex2d(-0.5,-0.5);
        glVertex2d(0.0,0.75);
        glVertex2d(0.5,-0.5);
        glEnd();

        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }

    // ... here, the user closed the window
    glfwTerminate();
    return 0;
}