# Import open gl and other requirements
import math
from OpenGL.GL import *
import glfw
import sys

# Constants for drawing the window, and clicking on nodes
WINDOW_WIDTH = int(sys.argv[1])
WINDOW_HEIGHT = int(sys.argv[2])
X_VARIANCE = 10
Y_VARIANCE = 10

# Initialize the window
glfw.init()
# glfw.window_hint(glfw.SAMPLES, 4)
window = glfw.create_window(WINDOW_WIDTH, WINDOW_HEIGHT, "Spline", None, None)
glfw.make_context_current(window)

# Set to projection mode
glMatrixMode(GL_PROJECTION)
glLoadIdentity()
glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1)
glEnable(GL_MULTISAMPLE)

# glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)

glColor3f(0, 0, 0)
glClearColor(1, 1, 1, 255)

glPointSize(20.0)
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)


class Node:

    def __init__(self, x, y, control_point_x, control_point_y):
        """
        A node containing a point, and a control point strength
        :param x: The x position of the point
        :param y: The y position of the point
        :param control_point_x: The control point x strength
        :param control_point_y: The control point y strength
        """
        self.x = x
        self.y = y
        self.control_point_x_strength = control_point_x
        self.control_point_y_strength = control_point_y


def check_if_on_node(nodes, mouse_x, mouse_y):
    """
    Returns the index of the point currently hovering over. If none, returns -1
    :param nodes: The array of nodes to check
    :param mouse_x: The mouse x position from the left
    :param mouse_y: The mouse y position from the bottom
    :return: The index in the array of the point found
    """

    for i, node in enumerate(nodes):
        if abs(node.x - mouse_x) < X_VARIANCE and abs(node.y - mouse_y) < Y_VARIANCE:
            return i
    return -1


def check_if_on_control(nodes, mouse_x, mouse_y):
    """
    Returns the index of the control point currently hovering over. If none, returns -1
    :param nodes: The array of nodes to check
    :param mouse_x: The mouse x position from the left
    :param mouse_y: The mouse y position from the bottom
    :return: The index in the array of the point found, as well as if it is the first point as a bool
    """
    # Control points should only be interacted with if there are more than zero
    if len(nodes) > 1:
        for i, node in enumerate(nodes):
            # Check if the mouse is close to the first control point
            if abs(node.x + node.control_point_x_strength - mouse_x) < X_VARIANCE \
                    and abs(node.y + node.control_point_y_strength - mouse_y) < Y_VARIANCE and i != len(nodes):

                # The last node should not have it's first control point
                if i != len(nodes) - 1:
                    return i, True

            # Check if the mouse is close to the second control point
            elif abs(node.x - node.control_point_x_strength - mouse_x) < X_VARIANCE \
                    and abs(node.y - node.control_point_y_strength - mouse_y) < Y_VARIANCE and i != 0:

                # The first node should not have its second control point
                if i != 0:
                    return i, False
    return -1, True


# Make list of nodes
nodes = [Node(400.0, 400.0, 0, 50.0)]
currently_selected_node_index = None
currently_selected_control_index = None

current_frame_mouse_button = 0
previous_frame_mouse_button = 0
mode = None

while not glfw.window_should_close(window):

    # Set the color and clear the buffer bit
    glfw.poll_events()
    glColor3f(0, 0, 0)
    glClear(GL_COLOR_BUFFER_BIT)

    # Get mouse info
    current_frame_mouse_button = glfw.get_mouse_button(window, glfw.MOUSE_BUTTON_1)
    mouse_x, mouse_y = glfw.get_cursor_pos(window)
    # Flip the y cord to follow glOrtho's scheme
    mouse_y = WINDOW_HEIGHT - mouse_y

    # Set the current "mode"
    if current_frame_mouse_button == 0 and previous_frame_mouse_button == 0:
        mode = None
    elif current_frame_mouse_button == 1 and previous_frame_mouse_button == 0:
        mode = "Click"
    elif current_frame_mouse_button == 1 and previous_frame_mouse_button == 1:
        mode = "Hold"

    if glfw.get_key(window, glfw.KEY_E) and mode is None:
        # Reset the screen when E is pressed
        current_frame_mouse_button = 0
        previous_frame_mouse_button = 0
        nodes = []

    if mode is None:
        # If the user isn't clicking, do nothing
        currently_selected_node_index = None
        currently_selected_control_index = None

    elif mode == "Click":

        # Check if the user is over a node. If not, returns -1
        hovered_node = check_if_on_node(nodes, mouse_x, mouse_y)
        # Check if the user is over a control node. If not, returns -1
        hovered_control = check_if_on_control(nodes, mouse_x, mouse_y)

        # if the user is not over any nodes, add one
        if hovered_node == -1 and hovered_control[0] == -1:

            if len(nodes) > 0:
                # Get the distance to the front and the end
                dist_to_front_node = math.sqrt(math.pow(nodes[0].x - mouse_x, 2) + math.pow(nodes[0].y - mouse_y, 2))
                dist_to_back_node = math.sqrt(math.pow(nodes[-1].x - mouse_x, 2) + math.pow(nodes[-1].y - mouse_y, 2))

                # Add a node depending on if it is closer to the front or the end
                if dist_to_front_node >= dist_to_back_node or len(nodes) == 0:
                    nodes.append(Node(mouse_x, mouse_y, 0.0, -50.0))
                else:
                    nodes.insert(0, Node(mouse_x, mouse_y, 0.0, -50.0))
            else:
                # 50 not -50 so that the first node points up
                nodes.append(Node(mouse_x, mouse_y, 0.0, 50.0))

        # If the user is over a control node
        elif hovered_node == -1 and hovered_control[0] != -1:
            currently_selected_control_index = hovered_control
        else:
            currently_selected_node_index = hovered_node

    elif mode == "Hold":

        # At this point either the user is controlling a node, or a control point
        # The way ive done this the whole program is not halted while the user drags the node
        if currently_selected_node_index != -1 and currently_selected_node_index is not None:

            nodes[currently_selected_node_index].x = mouse_x
            nodes[currently_selected_node_index].y = mouse_y

        elif currently_selected_control_index is not None:

            x_strength = mouse_x - nodes[currently_selected_control_index[0]].x
            y_strength = mouse_y - nodes[currently_selected_control_index[0]].y

            if currently_selected_control_index[1]:
                nodes[currently_selected_control_index[0]].control_point_x_strength = x_strength
                nodes[currently_selected_control_index[0]].control_point_y_strength = y_strength
            if not currently_selected_control_index[1]:
                nodes[currently_selected_control_index[0]].control_point_x_strength = -x_strength
                nodes[currently_selected_control_index[0]].control_point_y_strength = -y_strength

    # Drawing dotted control lines
    if len(nodes) > 1:

        glColor3f(0, 0.75, 1)
        glEnable(GL_LINE_STIPPLE)
        glLineStipple(4, 101010)
        glLineWidth(5)
        for i, node in enumerate(nodes):

            # If it is the first node, don't draw one of the control lines
            if i != 0:
                glBegin(GL_LINES)
                glVertex2f(node.x, node.y)
                glVertex2f(node.x - node.control_point_x_strength, node.y - node.control_point_y_strength)
                glEnd()

            # If it is the last node, don't draw one of the control lines
            if i != len(nodes) - 1:
                glBegin(GL_LINES)
                glVertex2f(node.x, node.y)
                glVertex2f(node.x + node.control_point_x_strength, node.y + node.control_point_y_strength)
                # glVertex2f(node.x + node.control_point_x_strength, node.control_point_y_strength)
                glEnd()
                # # elif

        glDisable(GL_LINE_STIPPLE)
        glLineWidth(3)

    # Draw Nodes
    glDisable(GL_BLEND)
    glDisable(GL_POINT_SMOOTH)
    glColor3f(0, 0, 0)

    glBegin(GL_POINTS)
    for node in nodes:
        glVertex2f(node.x, node.y)
    glEnd()

    # # Draw control points
    glEnable(GL_BLEND)
    glEnable(GL_POINT_SMOOTH)

    glBegin(GL_POINTS)
    glColor3f(0.75, 0.25, 0)
    for i, node in enumerate(nodes):

        # The position plus the strength
        x1 = node.x + node.control_point_x_strength
        y1 = node.y + node.control_point_y_strength

        # The position minus the strength
        x2 = node.x - node.control_point_x_strength
        y2 = node.y - node.control_point_y_strength
        # Draw both of the control points
        if i != len(nodes) - 1:
            glVertex2f(x1, y1)
        if i != 0:
            glVertex2f(x2, y2)
    glEnd()

    # Draw spline
    glColor3f(0, 0, 0)
    glEnable(GL_BLEND)
    NUMBER_OF_SPLINE_SEGMENTS = 200

    if len(nodes) > 1:
        for i in range(len(nodes)-1):
            final_x = nodes[i].x
            final_y = nodes[i].y
            glBegin(GL_LINE_STRIP)
            x = nodes[i].x
            y = nodes[i].y
            for j in range(NUMBER_OF_SPLINE_SEGMENTS+1):

                t = j/NUMBER_OF_SPLINE_SEGMENTS
                t1 = j+1/NUMBER_OF_SPLINE_SEGMENTS

                # Calculate all the information for drawing the spline
                start_x = nodes[i].x
                start_y = nodes[i].y
                start_x_strength = nodes[i].control_point_x_strength + start_x
                start_y_strength = nodes[i].control_point_y_strength + start_y

                end_x = nodes[i+1].x
                end_y = nodes[i+1].y
                end_x_strength = -1*nodes[i+1].control_point_x_strength + end_x
                end_y_strength = -1*nodes[i+1].control_point_y_strength + end_y

                glVertex2f(x, y)

                term1x = math.pow(1-t, 3) * start_x
                term2x = 3 * t * math.pow(1-t, 2) * start_x_strength
                term3x = 3 * math.pow(t,2) * (1-t) * end_x_strength
                term4x = math.pow(t, 3) * end_x

                term1y = math.pow(1-t, 3) * start_y
                term2y = 3 * t * math.pow(1-t, 2) * start_y_strength
                term3y = 3 * math.pow(t,2) * (1-t) * end_y_strength
                term4y = math.pow(t, 3) * end_y

                x = term1x + term2x + term3x + term4x
                y = term1y + term2y + term3y + term4y

                glVertex2f(x, y)

            glEnd()

    glDisable(GL_BLEND)

    previous_frame_mouse_button = current_frame_mouse_button

    glfw.swap_buffers(window)

glfw.terminate()
