/*
The terrain shader:

Blends between 4 different textures (sand,grass,snow and rock) according to each pixel's height and "slope".
"slope" is defined as the dot product between the pixel's interpolated normal and teh up vector: the closer to 1.0 the flatter
the surface the pixel is part of; 0 means a vertical wall.
The value is used to transition between rock and snow/grass/sand at different heights in the created terrain.
To make the transition smoother, the actual slope is raised to powers between 3 and 8 according to the pixel's height.

*/

#pragma once

#include "TechBase.h"

class TerrTech: public TechBase
{
public:
	TerrTech();
	~TerrTech();
	void SetTexture0(GLuint hTex);
	void SetTexture1(GLuint hTex);
	void SetTexture2(GLuint hTex);
	void SetTexture3(GLuint hTex);
	void SetMedianHeight(GLfloat value);
};