/*
Renderable base class: stores and pumps geometric data about anything that can be rendered in 3D to the graphic card;
it is meant to be used as a base for other more specialized classes to be derived from it.

More specifically, the class is structured to hold information about all vertices (position and normal vectors in model space as well as texture coordinates)
comprising the model as well as the subsets, groups of vertices sharing a material and material information.

As different objects are rendered using different shaders, each using textures in different ways, texture information is retained in the derived classes.

With the notable exception of 3D models loaded directly from a Milkshape (3D authoring package) model file, the hierarchy of derived classes is 2 levels deep:
Geometries directly derived from this class act as primitives (cube, grid), from which different objects are created.
This allows for 3d objects sharing their geometric setup but rendered with different shaders and having different methods of manipulation.

Es: BaseGeometry--->ProceduralGrid--->Terrain
                 |                 |
				 |  			   -->Water
                 |
				 -->ProceduralCube--->SkyBox
*/

#pragma once

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glext.h"
#include "vector.h"

struct MaterialStr
{
	MaterialStr(void) {GLuint i; for(i=0;i<4;i++) {Specular[i]=0.0f; Emissive[i]=0.0f; Diffuse[i]=0.0f; Ambient[i]=0.0f;} Shininess[0]=1.0f;}
	GLfloat Specular[4];
	GLfloat Emissive[4];
	GLfloat Diffuse[4];
	GLfloat Ambient[4];
	GLfloat Shininess[1];
};

#pragma pack(push, 1)
struct VertexStr
{
	Vector4 Pos, Norm, Tex;
};
#pragma pack(pop)

struct SubsetStr
{
	SubsetStr() {NumTriangles=0; hVBOIndexBuffer=0; pIndices=NULL;}
	~SubsetStr() {if(pIndices) {delete[] pIndices;}}

	GLuint NumTriangles;
	MaterialStr Material;
	GLuint * pIndices;
	
	GLuint hVBOIndexBuffer; 
};

class BaseGeometry
{
public:
protected:
	BaseGeometry(void);
	~BaseGeometry(void);
	void SetNumVertices(GLuint numvertices);
	void GenerateVertexBuffer(void);
	void SetNumSubsets(GLuint numsubsets);
	void ResizeSubset(GLuint numsubset, GLuint numtriangles);
	void GenerateSubsetIndicesBuffer(GLuint numsubset);
	void DrawSubset(GLuint numsubset) const;
	virtual void Render()=0;

	GLuint m_uiNumSubsets;
	SubsetStr * m_pSubsets;

	GLuint m_uiNumVertices;
	VertexStr * m_pVertices;
	GLuint m_hVBOVertexBuffer;

	//OpenGL extensions function pointers
	PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
	PFNGLGENBUFFERSARBPROC glGenBuffersARB;					// VBO Name Generation Procedure
	PFNGLBINDBUFFERARBPROC glBindBufferARB;					// VBO Bind Procedure
	PFNGLBUFFERDATAARBPROC glBufferDataARB;					// VBO Data Loading Procedure
	PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;			// VBO Deletion Procedure
};
