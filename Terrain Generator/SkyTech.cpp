#include "SkyTech.h"

SkyTech::SkyTech(void)
:TechBase("Shaders\\SkyVS.txt","Shaders\\SkyPS.txt")
{
}

SkyTech::~SkyTech(void)
{
}

void SkyTech::SetTexture(GLuint hTex)
{
	BindTex2DToTextureUnit(hTex,GL_TEXTURE0_ARB);
	BindTexUnitToSampler(0,"TextureS");
}