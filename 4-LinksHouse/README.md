# 4-LinksHouse

Renders Links house using ply files and textures. You are able to move around using the arrow keys.
C looks at the center, the left and right braces move up and down.
All assets are in ```./LinksHouse```.

At this point I transitioned to C++ instead of Python. This is because C++ seemed like a more 
likely use case in the future, and I was having trouble running shaders on an M1 mac, and OpenGL is deprecated on M1,
so from now on I have been running these projects on M1 inside a VM running Ubuntu

I used a makefile to work with this, so just run 'make' to run it.

It was a little laggy for me, but im pretty sure that is because I am running it in a VM.
