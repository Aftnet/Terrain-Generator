# Terrain Generator

By [Alberto Fustinoni](http://aftnet.net)

An OpenGL based procedural terrain generator, the application creates a grid shaped triangle mesh upon launch or user request and then pseudo randomly deforms it into a shape resembling a terrestrial landscape.

In addition to this various techniques, ranging from texture splatting to procedural tree placement to water rendering, are used to increase the believability and aesthetic appeal of the generated landscape.

While the Win32 initialization code is, again, derived from Matthew Bett’s framework, calls to OpenGL and its extensions have been arranged into a framework of my own making, which greatly simplifies scene description and provides custom functionality (model loading from binary Milkshape 3D files, for example).

## Controls

- WASD: Move camera.
- Hold left mouse button + drag: look around
- 1 (numpad): Camera free fly mode
- 2 (numpad): Camera bound to terrain mode
- N: Generate new landscape

## System requirements

Windows XP or later (Windows 2000 may work but has not been tested), graphic card and drivers supporting OpenGL GLSL shaders.

To work properly, Terrain.exe has to be in the same directory as the Models, Shaders and Textures directories with their original contents.

## Credits

None of the art assets included in the application have been produced by me.
Below are their sources.

- [Sand texture obtained from](http://local.wasp.uwa.edu.au/~pbourke/texture_colour/displayimage.cgi?sand/dirtysand.jpg)
- [Snow texture obtained from](http://sophie-g.net/photo/nature/joux/pct/neige.jpg)
- Rock texture obtained from Kewcharoen Wittawat (friend)
- Grass texture obtained from Google images
- Water texture obtained from Paul Michael Bramsiepie
- [Skybox textures](http://www.hazelwhorley.com/skyboxtex1_bitmaps.html)
- Tree 3D Milkshape models obtained from Muldoon Niall (friend)
- [SOIL Image loader library](http://www.lonesock.net/soil.html)
- [Phong-Blinn GLSL shader](http://www.lighthouse3d.com/)