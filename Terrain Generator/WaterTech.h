/*
A cheap way of rendering water: uses the shader to offset vertices texture coordinates for the water grid.
Phong lighting with texture animation.
*/

#pragma once

#include "TechBase.h"

class WaterTech : public TechBase
{
public:
	WaterTech(void);
	~WaterTech(void);
	void SetTexture(GLuint hTex);
	void SetTime(float time);
};
