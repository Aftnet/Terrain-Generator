#include "SkyBox.h"
#include "Libs\SOIL.h"

SkyBox::SkyBox(void)
:ProceduralCube(50.0f)
{
	m_hTex[0] = SOIL_load_OGL_texture("Textures\\North.dds", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO);
	m_hTex[1] = SOIL_load_OGL_texture("Textures\\South.dds", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO);
	m_hTex[2] = SOIL_load_OGL_texture("Textures\\East.dds", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO);
	m_hTex[3] = SOIL_load_OGL_texture("Textures\\West.dds", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO);
	m_hTex[4] = SOIL_load_OGL_texture("Textures\\Up.dds", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO);
	m_hTex[5] = SOIL_load_OGL_texture("Textures\\Down.dds", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO);
}

SkyBox::~SkyBox(void)
{
}

void SkyBox::Render(void)
{
	m_Technique.Set();
	GLuint i;
	for(i=0;i<m_uiNumSubsets;i++)
	{
		m_Technique.SetTexture(m_hTex[i]);
		DrawSubset(i);
	}
}
