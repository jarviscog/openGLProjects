# Waves

Creates realistic waves using tesselation, textures, bump maps, and Gerstner wave functions.

Waves.cpp contains all the code to initialize OpenGL, as well.

PlaneMesh.hpp contains all the code for the water, including the functions to compile shaders, and link them

All the shaders that get compiled are in the "Shaders", directory, and all the images including bump maps are in
Assets

This was created in C++, inside multipass on an M1 mac.
If there are bugs with multisampling, or performance, this is probably why
The program lags a little. I think this is because it is
running in a VM. Your milage may vary.