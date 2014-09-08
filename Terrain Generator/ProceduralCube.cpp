#include "ProceduralCube.h"

ProceduralCube::ProceduralCube(GLfloat halfsize)
{
	SetNumVertices(24);
	m_pVertices[0].Pos = Vector4(0.0f-halfsize, halfsize, 0.0f-halfsize, 1.0f);
	m_pVertices[0].Norm = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	m_pVertices[0].Tex = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	m_pVertices[1].Pos = Vector4(0.0f-halfsize, 0.0f-halfsize, 0.0f-halfsize, 1.0f);
	m_pVertices[1].Norm = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	m_pVertices[1].Tex = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

	m_pVertices[2].Pos = Vector4(halfsize, 0.0f-halfsize, 0.0f-halfsize, 1.0f);
	m_pVertices[2].Norm = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	m_pVertices[2].Tex = Vector4(1.0f, 1.0f, 0.0f, 0.0f);

	m_pVertices[3].Pos = Vector4(halfsize, halfsize, 0.0f-halfsize, 1.0f);
	m_pVertices[3].Norm = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	m_pVertices[3].Tex = Vector4(1.0f, 0.0f, 0.0f, 0.0f);


	m_pVertices[4].Pos = Vector4(halfsize, halfsize, halfsize, 1.0f);
	m_pVertices[4].Norm = Vector4(0.0f, 0.0f, -1.0f, 0.0f);
	m_pVertices[4].Tex = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	m_pVertices[5].Pos = Vector4(halfsize, 0.0f-halfsize, halfsize, 1.0f);
	m_pVertices[5].Norm = Vector4(0.0f, 0.0f, -1.0f, 0.0f);
	m_pVertices[5].Tex = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

	m_pVertices[6].Pos = Vector4(0.0f-halfsize, 0.0f-halfsize, halfsize, 1.0f);
	m_pVertices[6].Norm = Vector4(0.0f, 0.0f, -1.0f, 0.0f);
	m_pVertices[6].Tex = Vector4(1.0f, 1.0f, 0.0f, 0.0f);

	m_pVertices[7].Pos = Vector4(0.0f-halfsize, halfsize, halfsize, 1.0f);
	m_pVertices[7].Norm = Vector4(0.0f, 0.0f, -1.0f, 0.0f);
	m_pVertices[7].Tex = Vector4(1.0, 0.0f, 0.0f, 0.0f);


	m_pVertices[8].Pos = Vector4(halfsize, halfsize, 0.0f-halfsize, 1.0f);
	m_pVertices[8].Norm = Vector4(-1.0f, 0.0f, 0.0f, 0.0f);
	m_pVertices[8].Tex = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	m_pVertices[9].Pos = Vector4(halfsize, 0.0f-halfsize, 0.0f-halfsize, 1.0f);
	m_pVertices[9].Norm = Vector4(-1.0f, 0.0f, 0.0f, 0.0f);
	m_pVertices[9].Tex = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

	m_pVertices[10].Pos = Vector4(halfsize, 0.0f-halfsize, halfsize, 1.0f);
	m_pVertices[10].Norm = Vector4(-1.0f, 0.0f, 0.0f, 0.0f);
	m_pVertices[10].Tex = Vector4(1.0f, 1.0f, 0.0f, 0.0f);

	m_pVertices[11].Pos = Vector4(halfsize, halfsize, halfsize, 1.0f);
	m_pVertices[11].Norm = Vector4(-1.0f, 0.0f, 0.0f, 0.0f);
	m_pVertices[11].Tex = Vector4(1.0f, 0.0f, 0.0f, 0.0f);

	
	m_pVertices[12].Pos = Vector4(0.0f-halfsize, halfsize, halfsize, 1.0f);
	m_pVertices[12].Norm = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	m_pVertices[12].Tex = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	m_pVertices[13].Pos = Vector4(0.0f-halfsize, 0.0f-halfsize, halfsize, 1.0f);
	m_pVertices[13].Norm = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	m_pVertices[13].Tex = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

	m_pVertices[14].Pos = Vector4(0.0f-halfsize, 0.0f-halfsize, 0.0f-halfsize, 1.0f);
	m_pVertices[14].Norm = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	m_pVertices[14].Tex = Vector4(1.0f, 1.0f, 0.0f, 0.0f);

	m_pVertices[15].Pos = Vector4(0.0f-halfsize, halfsize, 0.0f-halfsize, 1.0f);
	m_pVertices[15].Norm = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	m_pVertices[15].Tex = Vector4(1.0f, 0.0f, 0.0f, 0.0f);


	m_pVertices[16].Pos = Vector4(0.0f-halfsize, halfsize, 0.0f-halfsize, 1.0f);
	m_pVertices[16].Norm = Vector4(0.0f, -1.0f, 0.0f, 0.0f);
	m_pVertices[16].Tex = Vector4(1.0f, 0.0f, 0.0f, 0.0f);

	m_pVertices[17].Pos = Vector4(halfsize, halfsize, 0.0f-halfsize, 1.0f);
	m_pVertices[17].Norm = Vector4(0.0f, -1.0f, 0.0f, 0.0f);
	m_pVertices[17].Tex = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	m_pVertices[18].Pos = Vector4(halfsize, halfsize, halfsize, 1.0f);
	m_pVertices[18].Norm = Vector4(0.0f, -1.0f, 0.0f, 0.0f);
	m_pVertices[18].Tex = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

	m_pVertices[19].Pos = Vector4(0.0f-halfsize, halfsize, halfsize, 1.0f);
	m_pVertices[19].Norm = Vector4(0.0f, -1.0f, 0.0f, 0.0f);
	m_pVertices[19].Tex = Vector4(1.0f, 1.0f, 0.0f, 0.0f);


	m_pVertices[20].Pos = Vector4(0.0f-halfsize, 0.0f-halfsize, halfsize, 1.0f);
	m_pVertices[20].Norm = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	m_pVertices[20].Tex = Vector4(1.0f, 0.0f, 0.0f, 0.0f);

	m_pVertices[21].Pos = Vector4(halfsize, 0.0f-halfsize, halfsize, 1.0f);
	m_pVertices[21].Norm = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	m_pVertices[21].Tex = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	m_pVertices[22].Pos = Vector4(halfsize, 0.0f-halfsize, 0.0f-halfsize, 1.0f);
	m_pVertices[22].Norm = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	m_pVertices[22].Tex = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

	m_pVertices[23].Pos = Vector4(0.0f-halfsize, 0.0f-halfsize, 0.0f-halfsize, 1.0f);
	m_pVertices[23].Norm = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	m_pVertices[23].Tex = Vector4(1.0f, 1.0f, 0.0f, 0.0f);



	GenerateVertexBuffer();




	SetNumSubsets(6);
	for(GLuint i=0;i<m_uiNumSubsets;i++)
	{
		ResizeSubset(i,2);
		m_pSubsets[i].pIndices[0] = 4*i+0;
		m_pSubsets[i].pIndices[1] = 4*i+1;
		m_pSubsets[i].pIndices[2] = 4*i+2;
		m_pSubsets[i].pIndices[3] = 4*i+2;
		m_pSubsets[i].pIndices[4] = 4*i+3;
		m_pSubsets[i].pIndices[5] = 4*i+0;
		GenerateSubsetIndicesBuffer(i);

		//Generate material properties
		m_pSubsets[i].Material.Specular[0] = 0.0f;
		m_pSubsets[i].Material.Specular[1] = 0.0f;
		m_pSubsets[i].Material.Specular[2] = 0.0f;
		m_pSubsets[i].Material.Specular[3] = 1.0f;
		m_pSubsets[i].Material.Emissive[0] = 0.0f;
		m_pSubsets[i].Material.Emissive[1] = 0.0f;
		m_pSubsets[i].Material.Emissive[2] = 0.0f;
		m_pSubsets[i].Material.Emissive[3] = 1.0f;
		m_pSubsets[i].Material.Diffuse[0] = 0.0f;
		m_pSubsets[i].Material.Diffuse[1] = 0.0f;
		m_pSubsets[i].Material.Diffuse[2] = 0.0f;
		m_pSubsets[i].Material.Diffuse[3] = 1.0f;
		m_pSubsets[i].Material.Ambient[0] = 1.0f;
		m_pSubsets[i].Material.Ambient[1] = 1.0f;
		m_pSubsets[i].Material.Ambient[2] = 1.0f;
		m_pSubsets[i].Material.Ambient[3] = 1.0f;
		m_pSubsets[i].Material.Shininess[0] = 15.0f;
	}
}

ProceduralCube::~ProceduralCube(void)
{
}

void ProceduralCube::Render(void)
{
	GLuint i;
	for(i=0;i<m_uiNumSubsets;i++)
	{
		DrawSubset(i);
	}
}