#include "Water.h"

Water::Water(GLuint width, GLuint height, GLfloat tilesize, GLuint htexturetiles, GLuint vtexturetiles)
:ProceduralGrid(width, height, tilesize, htexturetiles, vtexturetiles)
{
	//Generate material properties
	m_pSubsets[0].Material.Specular[0] = 0.4f;
	m_pSubsets[0].Material.Specular[1] = 0.4f;
	m_pSubsets[0].Material.Specular[2] = 0.4f;
	m_pSubsets[0].Material.Specular[3] = 0.0f;
	m_pSubsets[0].Material.Emissive[0] = 0.0f;
	m_pSubsets[0].Material.Emissive[1] = 0.0f;
	m_pSubsets[0].Material.Emissive[2] = 0.0f;
	m_pSubsets[0].Material.Emissive[3] = 0.0f;
	m_pSubsets[0].Material.Diffuse[0] = 0.4f;
	m_pSubsets[0].Material.Diffuse[1] = 0.4f;
	m_pSubsets[0].Material.Diffuse[2] = 0.4f;
	m_pSubsets[0].Material.Diffuse[3] = 0.0f;
	m_pSubsets[0].Material.Ambient[0] = 0.5f;
	m_pSubsets[0].Material.Ambient[1] = 0.5f;
	m_pSubsets[0].Material.Ambient[2] = 0.5f;
	m_pSubsets[0].Material.Ambient[3] = 0.4f;
	m_pSubsets[0].Material.Shininess[0] = 80.0f;

	//Load Texture
	m_hTex[0] = SOIL_load_OGL_texture("Textures\\Water.dds", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_TEXTURE_REPEATS);
}

Water::~Water(void)
{
}

void Water::Render(float time)
{
	m_Technique.Set();
	m_Technique.SetTexture(m_hTex[0]);
	m_Technique.SetTime(time);
	DrawSubset(0);
}
