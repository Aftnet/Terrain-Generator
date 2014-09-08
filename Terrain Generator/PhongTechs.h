/*

Per pixel phong shading techniques: first one works using a texture as well as material properties, while the second only uses material properties.

The second version is a fallback in case a Milkshape3d model does not specify a texture for one or more subsets.

*/

#pragma once

#include "TechBase.h"

class PhongTech : public TechBase
{
public:
	PhongTech(void);
	~PhongTech(void);
	void SetTexture(GLuint hTex);
};

class PhongNoTexTech : public TechBase
{
public:
	PhongNoTexTech(void);
	~PhongNoTexTech(void);
};
