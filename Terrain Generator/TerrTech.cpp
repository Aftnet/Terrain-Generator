#include "TerrTech.h"

TerrTech::TerrTech()
:TechBase("Shaders\\TerrainVS.txt","Shaders\\TerrainPS.txt")
{
}

TerrTech::~TerrTech()
{
}

void TerrTech::SetTexture0(GLuint hTex)
{
	BindTex2DToTextureUnit(hTex,GL_TEXTURE0_ARB);
	BindTexUnitToSampler(0,"Tex0S");
}

void TerrTech::SetTexture1(GLuint hTex)
{
	BindTex2DToTextureUnit(hTex,GL_TEXTURE1_ARB);
	BindTexUnitToSampler(1,"Tex1S");
}

void TerrTech::SetTexture2(GLuint hTex)
{
	BindTex2DToTextureUnit(hTex,GL_TEXTURE2_ARB);
	BindTexUnitToSampler(2,"Tex2S");
}

void TerrTech::SetTexture3(GLuint hTex)
{
	BindTex2DToTextureUnit(hTex,GL_TEXTURE3_ARB);
	BindTexUnitToSampler(3,"Tex3S");
}

void TerrTech::SetMedianHeight(GLfloat value)
{
	SetUniform1f(value,"terrmedheight");
}