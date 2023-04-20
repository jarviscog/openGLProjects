import math

from OpenGL.GL import *
import glfw

glfw.init()
window = glfw.create_window(1280, 1000, "Testing", None, None)
glfw.make_context_current(window)

glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(-5,5,-5,5,-1,1)

glMatrixMode ( GL_MODELVIEW );
glLoadIdentity()

i = 0
while not glfw.window_should_close(window):

    i += 1
    # glTranslatef(2*math.sin(i/20-180)/20,2*math.sin(i/20)/20,0)
    glfw.poll_events()
    glClear(GL_COLOR_BUFFER_BIT)
    glColor3f(0.9, 0.9, 0.9)
    glBegin(GL_TRIANGLES)
    glVertex2f(0, 0.5);
    glVertex2f(0.5, -0.25);
    glVertex2f(-0.5, -0.25);
    glEnd();

    glColor3f(0.5, 0.4, 0.5)
    glBegin(GL_TRIANGLES)
    glVertex2f(0, 0.25);
    glVertex2f(0.25, -0.125);
    glVertex2f(-0.25, -0.125);
    glEnd();

    glLineWidth(30)
    glBegin(GL_LINE_STRIP)
    glVertex2f(-3.5, -3.51)
    glVertex2f(0.0, 7.0);
    glVertex2f(3.5, -3.5);
    glVertex2f(-5.0, 3.5);
    glVertex2f(5.0, 3.5);
    glVertex2f(-3.5, -3.5);
    glEnd();

    glfw.swap_buffers(window)

glfw.terminate()
