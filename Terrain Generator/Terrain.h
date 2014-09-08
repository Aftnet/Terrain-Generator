/*
Class representing the actual terrain shown in the application.

Derived from ProceduralGrid, uses TerrTech to render.
*/

#pragma once

#include "ProceduralGrid.h"
#include "Libs\SOIL.h"
#include "Texture.h"
#include "Terrtech.h"

class Terrain : public ProceduralGrid
{
public:
	Terrain(GLuint width, GLuint height, GLfloat tilesize, GLuint htexturetiles, GLuint vtexturetiles);
	~Terrain(void);
	void Render(void);
protected:
	TerrTech m_Technique;
	GLuint m_hTex[4];
};
