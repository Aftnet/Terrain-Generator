/*
Class representing the water plane

Derived from ProceduralGrid, uses WaterTech for rendering.
*/

#pragma once

#include "ProceduralGrid.h"
#include "WaterTech.h"
#include "Libs\SOIL.h"

class Water : public ProceduralGrid
{
public:
	Water(GLuint width, GLuint height, GLfloat tilesize, GLuint htexturetiles, GLuint vtexturetiles);
	~Water(void);
	void Render(float time);

protected:
	WaterTech m_Technique;
	GLuint m_hTex[1];
};
