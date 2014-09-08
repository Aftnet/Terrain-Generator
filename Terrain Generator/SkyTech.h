/*

SkyBox shader: similar to phong one, but with two significant twists.

The fragment color is entirely determined by the interpolated texel value and the depth value in the z buffer is set to 1.

Allows to use a small cube as a skybox as long as it is translated to be centered on the camera position; 
also allows to avoid clearing the back buffer each frame if one draws the skybox first.
*/

#pragma once

#include "TechBase.h"

class SkyTech : public TechBase
{
public:
	SkyTech(void);
	~SkyTech(void);
	void SetTexture(GLuint hTex);
};
