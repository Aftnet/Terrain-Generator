#include "MS3Dmodel.h"

MS3Dmodel::MS3Dmodel(char * filename)
{
	//Initialize textures handles array
	m_hTex = NULL;

	//Stage 1: Read binary file into temporary structures in main memory
	ms3d_header header;
	word NumVertices;
	ms3d_vertex *pvert;
	word NumTriangles;
	ms3d_triangle *ptri;
	word NumGroups;
	ms3d_group *pgrp;
	word NumMaterials;
	ms3d_material *pmat;

	GLuint i,j;
	char tmpstr[150];
	std::fstream file;
	file.open(filename, std::fstream::in | std::fstream::binary);
	if(file.is_open())
	{	//Due to strange alignment issues (header structure should have a sizeof 14 but is 16) each member will be read separately
		//Read header
		file.read((char*)(&header.id),10*sizeof(char));
		file.read((char*)(&header.version),sizeof(int));
		//Read number of vertices
		file.read((char*)(&NumVertices),sizeof(word));
		//Allocate space for vertices information
		pvert = new ms3d_vertex[NumVertices];
		//Read vertex information
		for(i=0; i<NumVertices; i++)
		{
			file.read((char*)&pvert[i].flags,sizeof(byte));
			file.read((char*)&pvert[i].vertex,3*sizeof(float));
			file.read((char*)&pvert[i].boneId,sizeof(char));
			file.read((char*)&pvert[i].referenceCount,sizeof(byte));
		}
		//Read number of triangles
		file.read((char*)(&NumTriangles),sizeof(word));
		//Allocate space for triangles information
		ptri = new ms3d_triangle[NumTriangles];
		//Read triangle information
		for(i=0; i<NumTriangles; i++)
		{
			file.read((char*)&ptri[i].flags,sizeof(word));
			file.read((char*)&ptri[i].vertexIndices,3*sizeof(word));
			file.read((char*)&ptri[i].vertexNormals,9*sizeof(float));
			file.read((char*)&ptri[i].s,3*sizeof(float));
			file.read((char*)&ptri[i].t,3*sizeof(float));
			file.read((char*)&ptri[i].smoothingGroup,sizeof(byte));
			file.read((char*)&ptri[i].groupIndex,sizeof(byte));
		}
		//Read number of groups
		file.read((char*)(&NumGroups),sizeof(word));
		//Allocate space for groups information
		pgrp = new ms3d_group[NumGroups];
		//Read group information
		for(i=0; i<NumGroups; i++)
		{
			file.read((char*)&pgrp[i].flags,sizeof(byte));
			file.read((char*)&pgrp[i].name,32*sizeof(char));
			file.read((char*)&pgrp[i].numtriangles,sizeof(word));
			pgrp[i].triangleIndices = new word[pgrp[i].numtriangles];
			for(j=0; j<pgrp[i].numtriangles; j++)
			{
				file.read((char*)&pgrp[i].triangleIndices[j],sizeof(word));
			}
			file.read((char*)&pgrp[i].materialIndex,sizeof(char));
		}
		//Read number of materials
		file.read((char*)(&NumMaterials),sizeof(word));
		//Allocate space for material information
		pmat = new ms3d_material[NumMaterials];
		//Read material information
		for(i=0; i<NumMaterials; i++)
		{
			file.read((char*)&pmat[i].name,32*sizeof(char));
			file.read((char*)&pmat[i].ambient,4*sizeof(float));
			file.read((char*)&pmat[i].diffuse,4*sizeof(float));
			file.read((char*)&pmat[i].specular,4*sizeof(float));
			file.read((char*)&pmat[i].emissive,4*sizeof(float));
			file.read((char*)&pmat[i].shininess,sizeof(float));
			file.read((char*)&pmat[i].transparency,sizeof(float));
			file.read((char*)&pmat[i].mode,sizeof(char));
			file.read((char*)&pmat[i].texture,128*sizeof(char));
			file.read((char*)&pmat[i].alphamap,128*sizeof(char));
		}
		//Close model file
		file.close();


		SetNumVertices(NumTriangles*3);
		for(i=0; i<NumTriangles; i++)
		{
			m_pVertices[3*i+0].Pos.x = pvert[ptri[i].vertexIndices[0]].vertex[0];
			m_pVertices[3*i+0].Pos.y = pvert[ptri[i].vertexIndices[0]].vertex[1];
			m_pVertices[3*i+0].Pos.z = pvert[ptri[i].vertexIndices[0]].vertex[2];
			m_pVertices[3*i+0].Pos.w = 1.0f;

			m_pVertices[3*i+1].Pos.x = pvert[ptri[i].vertexIndices[1]].vertex[0];
			m_pVertices[3*i+1].Pos.y = pvert[ptri[i].vertexIndices[1]].vertex[1];
			m_pVertices[3*i+1].Pos.z = pvert[ptri[i].vertexIndices[1]].vertex[2];
			m_pVertices[3*i+1].Pos.w = 1.0f;

			m_pVertices[3*i+2].Pos.x = pvert[ptri[i].vertexIndices[2]].vertex[0];
			m_pVertices[3*i+2].Pos.y = pvert[ptri[i].vertexIndices[2]].vertex[1];
			m_pVertices[3*i+2].Pos.z = pvert[ptri[i].vertexIndices[2]].vertex[2];
			m_pVertices[3*i+2].Pos.w = 1.0f;


			m_pVertices[3*i+0].Norm.x = ptri[i].vertexNormals[0][0];
			m_pVertices[3*i+0].Norm.y = ptri[i].vertexNormals[0][1];
			m_pVertices[3*i+0].Norm.z = ptri[i].vertexNormals[0][2];
			m_pVertices[3*i+0].Norm.w = 0.0f;

			m_pVertices[3*i+1].Norm.x = ptri[i].vertexNormals[1][0];
			m_pVertices[3*i+1].Norm.y = ptri[i].vertexNormals[1][1];
			m_pVertices[3*i+1].Norm.z = ptri[i].vertexNormals[1][2];
			m_pVertices[3*i+1].Norm.w = 0.0f;

			m_pVertices[3*i+2].Norm.x = ptri[i].vertexNormals[2][0];
			m_pVertices[3*i+2].Norm.y = ptri[i].vertexNormals[2][1];
			m_pVertices[3*i+2].Norm.z = ptri[i].vertexNormals[2][2];
			m_pVertices[3*i+2].Norm.w = 0.0f;
			

			m_pVertices[3*i+0].Tex.x = ptri[i].s[0];
			m_pVertices[3*i+0].Tex.y = ptri[i].t[0];
			m_pVertices[3*i+0].Tex.z = 0.0f;
			m_pVertices[3*i+0].Tex.w = 1.0f;

			m_pVertices[3*i+1].Tex.x = ptri[i].s[1];
			m_pVertices[3*i+1].Tex.y = ptri[i].t[1];
			m_pVertices[3*i+1].Tex.z = 0.0f;
			m_pVertices[3*i+1].Tex.w = 1.0f;

			m_pVertices[3*i+2].Tex.x = ptri[i].s[2];
			m_pVertices[3*i+2].Tex.y = ptri[i].t[2];
			m_pVertices[3*i+2].Tex.z = 0.0f;
			m_pVertices[3*i+2].Tex.w = 1.0f;
		}
		GenerateVertexBuffer();

		SetNumSubsets(NumGroups);
		m_hTex = new GLuint[m_uiNumSubsets];

		for(i=0; i<m_uiNumSubsets; i++)
		{
			ResizeSubset(i,pgrp[i].numtriangles);
			for(j=0; j<4; j++)
			{
				m_pSubsets[i].Material.Ambient[j] = pmat[pgrp[i].materialIndex].ambient[j];
				m_pSubsets[i].Material.Diffuse[j] = pmat[pgrp[i].materialIndex].diffuse[j];
				m_pSubsets[i].Material.Emissive[j] = pmat[pgrp[i].materialIndex].emissive[j];
				m_pSubsets[i].Material.Specular[j] = pmat[pgrp[i].materialIndex].specular[j];
			}
			if(pmat[pgrp[i].materialIndex].shininess != 0.0f)
			{
				m_pSubsets[i].Material.Shininess[0] = pmat[pgrp[i].materialIndex].shininess;
			}
			else
			{
				m_pSubsets[i].Material.Shininess[0] = 1.0f;
			}

			char prefix[] = "Textures\\\\";
			strncpy_s(tmpstr, prefix, strlen(prefix));

			j=2;
			while(pmat[pgrp[i].materialIndex].texture[j] != '\0')
			{
				tmpstr[j+8]=pmat[pgrp[i].materialIndex].texture[j];
				j++;
			}
			tmpstr[j+8] = '\0';
			
			m_hTex[i] = SOIL_load_OGL_texture(tmpstr, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO);

			for(j=0; j<pgrp[i].numtriangles; j++)
			{
				m_pSubsets[i].pIndices[3*j] = 3*(pgrp[i].triangleIndices[j]);
				m_pSubsets[i].pIndices[3*j+1] = (3*(pgrp[i].triangleIndices[j]))+1;
				m_pSubsets[i].pIndices[3*j+2] = (3*(pgrp[i].triangleIndices[j]))+2;
			}

			GenerateSubsetIndicesBuffer(i);
		}
		delete[] pvert;
		delete[] ptri;
		delete[] pgrp;
		delete[] pmat;
	}
}

MS3Dmodel::~MS3Dmodel(void)
{
	if(m_hTex!=NULL)
	{
		delete[] m_hTex;
	}
}

void MS3Dmodel::Render(void)
{
	GLuint i=0;
	for(i=0; i<m_uiNumSubsets; i++)
	{
		if(m_hTex[i]==0)
		{
			m_TechNoTex.Set();
		}
		else
		{
			m_Tech.Set();
			m_Tech.SetTexture(m_hTex[i]);
		}
		DrawSubset(i);
	}
}