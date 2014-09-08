#include "WaterTech.h"

WaterTech::WaterTech(void)
:TechBase("Shaders\\WaterVS.txt","Shaders\\WaterPS.txt")
{
}

WaterTech::~WaterTech(void)
{
}

void WaterTech::SetTexture(GLuint hTex)
{
	BindTex2DToTextureUnit(hTex,GL_TEXTURE0_ARB);
	BindTexUnitToSampler(0,"TextureS");
}

void WaterTech::SetTime(float time)
{
	SetUniform1f(time,"time");
}