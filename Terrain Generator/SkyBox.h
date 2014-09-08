/*
Skybox class

Derived from ProceduralCube, uses SkyTech shader technique.

Unfortunately, using SOIL instead of the NeHe loader set to GL_CLAMP causes seams to appear in the skybox.
This means that the biggest textures can't be compressed.
*/

#pragma once

#include "ProceduralCube.h"
#include "SkyTech.h"
#include "Texture.h"

class SkyBox : public ProceduralCube
{
public:
	SkyBox(void);
	~SkyBox(void);
	void Render(void);

private:
	SkyTech m_Technique;
	GLuint m_hTex[6];
};
