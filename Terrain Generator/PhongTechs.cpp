#include "PhongTechs.h"

PhongTech::PhongTech(void)
:TechBase("Shaders\\PhongVS.txt","Shaders\\PhongPS.txt")
{
}

PhongTech::~PhongTech(void)
{
}

void PhongTech::SetTexture(GLuint hTex)
{
	BindTex2DToTextureUnit(hTex,GL_TEXTURE0_ARB);
	BindTexUnitToSampler(0,"TextureS");
}



PhongNoTexTech::PhongNoTexTech(void)
:TechBase("Shaders\\PhongNoTexVS.txt","Shaders\\PhongNoTexPS.txt")
{
}

PhongNoTexTech::~PhongNoTexTech(void)
{
}