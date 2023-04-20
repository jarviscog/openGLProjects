import sys
from OpenGL.GL import *
import glfw
from random import randint, random

args = sys.argv
n = int(args[1])
window_width = int(args[2])
window_height = int(args[3])

# Set up the window
glfw.init()
window = glfw.create_window(window_width, window_height, "Fractal", None, None)
glfw.make_context_current(window)

# Set to projection mode
glMatrixMode(GL_PROJECTION)
glLoadIdentity()

# Set the background color
glClearColor(1, 1, 1, 255)
glClear(GL_COLOR_BUFFER_BIT)
# Set the viewing volume
glOrtho(-1.1, 1.1, -1.1, 1.1, -1, 1)

# Set the background color
glClearColor(1, 1, 1, 1)

# Set the color and point size
glColor3f(0, 0, 0)
glPointSize(2.0)

# Step 1
x_min = -1.0
y_min = -1.0
WIDTH = 2
HEIGHT = 2
x_max = x_min + WIDTH
y_max = y_min + HEIGHT
# Init variables
cx = x_min
cy = y_min

# Step 2
if randint(0, 1) == 1:
    # print("Top")
    cx = cx * -1
if randint(0, 1) == 1:
    # print("Right")
    cy = cy * -1

# Step 3
px = (random() * WIDTH) + x_min
# print(px)
py = (random() * HEIGHT) + y_min
# print(py)

for i in range(n):

    # A.
    # Choose a random adjacent corner
    randy = randint(0, 2)
    if randy == 0:
        cx = cx * -1
    elif randy == 1:
        cy = cy * -1

    # B.
    px = (px + cx)/2
    py = (py + cy)/2

    # C.
    glBegin(GL_POINTS)
    glVertex2f(px, py)
    glEnd()

glfw.swap_buffers(window)

while not glfw.window_should_close(window):

    glfw.poll_events()

glfw.terminate()
