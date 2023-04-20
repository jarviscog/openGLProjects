import math

from OpenGL.GL import *
import glfw
from math import sin, cos

glfw.init()
window = glfw.create_window(1280, 1000, "Dog", None, None)
glfw.make_context_current(window)

# Set to projection mode
glMatrixMode(GL_PROJECTION)
glLoadIdentity()

# Make the view 60 by 60 units, and translate to 30, 30
glOrtho(0, 60, 0, 60, -1, 1)
glTranslatef(30,30,0)

# Set the background color
glClearColor(1, 1, 1, 255)

model_angle = 0

# Get the dog file information
with open("dog.txt", "r") as txt_file:
    dog_string = ""
    for line in txt_file.readlines():
        dog_string += line
    # Make an array from the points
    dog_points_arr = dog_string.split(" ")

def draw_dog():
    # Set dog color
    glColor3f(0, 0, 0)
    # Start drawing the dog
    glBegin(GL_LINE_STRIP)
    for i in range(0, len(dog_points_arr) - 1, 2):
        # print(dog_points_arr[i])
        glVertex2f(float(dog_points_arr[i]), float(dog_points_arr[i + 1]))
    glEnd()


while not glfw.window_should_close(window):

    glfw.poll_events()
    glClear(GL_COLOR_BUFFER_BIT)

    RADIUS = 25
    angles = [0, 45, 90, 135, 180, 225, 270, 315]
    # angles = [0, 90, 180, 270]

    # Set the information for the rotation of each dog
    model_angle += 1
    if model_angle > 360:
        model_angle = 0

    for i in range(len(angles)):
        # Calculate values of dog location
        a = angles[i]
        x = sin(math.radians(a)) * RADIUS
        y = cos(math.radians(a)) * RADIUS
        # For debugging
        # print("Angle: ", a)
        # print(x," ", y)

        # Set the mode to model view, so we can rotate and translate each dog independently
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()

        # Translate to the desired spot, depending on the angle
        glTranslatef(x, y, 0)
        # Rotate the dog
        glRotate(model_angle, 0, 0, 1)
        # Draw the dog in the desired location
        draw_dog()
        # Return to projection mode
        glLoadIdentity()
        # Undo the translation done above to get back to the origin
        glTranslatef(-x, -y, 0)

    glfw.swap_buffers(window)

glfw.terminate()
