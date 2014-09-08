/*
Procedural grid class

The real heart of the terrain generator: allows the creation of a grid of square cells each made up of two triangles, 
taking care to tesellate using diamond patterns for nicer looking terrain when undulated, as well as of calculating normals.
The class implements a number of terrain generation techniques (to be called at runtime after instantiation), which can be combined
to generate more complex terrains.
*/

#pragma once

#include "BaseGeometry.h"

class ProceduralGrid : public BaseGeometry
{
public:
	ProceduralGrid(GLuint width, GLuint height, GLfloat tilesize, GLuint htexturetiles, GLuint vtexturetiles);
	~ProceduralGrid(void);
	virtual void Render(void);
	Vector4 GetGridSize(void) const;
	float GetMedianHeight(void) const;
	float GetPointHeight(float xpos, float zpos) const;
	Vector4 GetPointNormal(float xpos, float zpos) const;
	void Reset(float height = 0.0f);
	void Smooth(GLuint iterations, GLuint centerweight);
	void RandomNoise(float magnitude);
	void Fault(GLuint iterations, float initdisplacement, float dampening);
	

protected:
	enum VertexAttribute {POSITION, NORMAL, TEXCOORDINATE};
	bool GetVertexAttrib(VertexAttribute attr, GLuint column, GLuint row, Vector4 &output) const;
	void NormalGen(void);
	void CalculateMedianHeight(void);
	GLuint m_iWidth, m_iHeight;
	GLfloat m_fHsize,m_fVsize,m_fMedianHeight,m_fTileSize;
};
