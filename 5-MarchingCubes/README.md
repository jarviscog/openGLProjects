# Marching Cubes

Uses the marching cubes algorithm to create geometry based on an arbitrary 3D Scalar Field function

This uses more advanced shader techniques such as phong reflections. This uses the camera position as well as the normal of each tri
to calculate realistic lighting.

Two screenshots and ply files have been included to show the result.

### Issues

The program lags when closing. This is because it is saving the mesh to the ply file. This can be fixed by commenting out
the writePLY function at the end of MarchingCubes.cpp

Cull face is at the end of my init_opengl() function if you want to disable it.


<figure>
<img src="MarchingCubes.gif" alt="Marching Wave" style="width:100%">
<figcaption align = "center"><b>Marching Wave</b></figcaption>
</figure>