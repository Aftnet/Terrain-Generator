#include "Terrain.h"

Terrain::Terrain(GLuint width, GLuint height, GLfloat tilesize, GLuint htexturetiles, GLuint vtexturetiles)
:ProceduralGrid(width, height, tilesize, htexturetiles, vtexturetiles)
{
	m_hTex[0] = SOIL_load_OGL_texture("Textures\\Rock.dds", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_TEXTURE_REPEATS);
	m_hTex[1] = SOIL_load_OGL_texture("Textures\\Sand.dds", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_TEXTURE_REPEATS);
	m_hTex[2] = SOIL_load_OGL_texture("Textures\\Grass.dds", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_TEXTURE_REPEATS);
	m_hTex[3] = SOIL_load_OGL_texture("Textures\\Snow.dds", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_TEXTURE_REPEATS);
}

Terrain::~Terrain(void)
{
}

void Terrain::Render(void)
{
	m_Technique.Set();
	m_Technique.SetTexture0(m_hTex[0]);
	m_Technique.SetTexture1(m_hTex[1]);
	m_Technique.SetTexture2(m_hTex[2]);
	m_Technique.SetTexture3(m_hTex[3]);
	m_Technique.SetMedianHeight(m_fMedianHeight);
	DrawSubset(0);
}
