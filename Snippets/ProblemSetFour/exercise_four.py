from OpenGL.GL import *
import glfw

glfw.init()
window = glfw.create_window(1280, 1000, "Problem set 4", None, None)
glfw.make_context_current(window)

# Set to projection mode
glMatrixMode(GL_PROJECTION)
glLoadIdentity()

# Set the viewing volume
glOrtho(-20, 20, -20, 20, -1, 1)

# Set the background color
glColor3f(0, 0, 0)


star = [0, 10, 2.5, 2.5, 10, 2.5, 4, -2.5, 7, -10, 0, -5, -7, -10, -4, -2.5, -10, 2.5, -2.5, 2.5, 0, 10]
decagon = [5, 15.388, 13.0902, 9.51056, 16.1803, 0, 13.0902, -9.51056, 5, -15.388, -5, -15.388,
           -13.0902, -9.51056, -16.1803, 0, -13.0902, 9.51056, -5, 15.388, 5, 15.388]

animation_frame = 1
forwards = True


while not glfw.window_should_close(window):

    glClearColor(1, 1, 1, 0)
    glClear(GL_COLOR_BUFFER_BIT)
    glLineWidth(4.0)

    glfw.poll_events()

    number_of_animation_frames = 400
    if forwards:
        animation_frame += 1
        if animation_frame > number_of_animation_frames:
            forwards = False
    else:
        animation_frame -= 1
        if animation_frame < 1:
            forwards = True

    normalized_frame = animation_frame/number_of_animation_frames

    glBegin(GL_LINE_STRIP)
    for i in range(0, len(star), 2):
        star_x = star[i]
        star_y = star[i+1]
        deca_x = decagon[i]
        deca_y = decagon[i+1]
        x = star_x*(1 - normalized_frame) + deca_x * normalized_frame
        y = star_y * (1 - normalized_frame) + deca_y * normalized_frame
        glVertex2f(x, y)

    glEnd()

    glfw.swap_buffers(window)

glfw.terminate()
