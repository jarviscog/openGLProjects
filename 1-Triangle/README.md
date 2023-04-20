# 1-Triangle

This is the "Hello World" of OpenGL

### Setting up the window

Make sure you install PyOpenGL. You should be able to do this using ```pip install PyOpenGL```

First, glfw must be initialized, and then a window object must be created.
The None and None parameters here are the 'monitor' and 'share' variables. Finally, the context of the window must 
be set to the window we just created.

### Drawing to the screen

We would like our code to run continuously, so all drawing code should go in the loop on line 8. This runs
the code for as long as the window is open. A few things must be run continuously, like poll_events and swap_buffers so 
that the image is displayed correctly. Then, we set the mode to immediate mode by using glBegin() and glEnd(). 
We set the color on lines 11 and 12, and describe the vertices of the triangle we would like to draw on lines
14-16. Then, when we press run, we see our triangle!