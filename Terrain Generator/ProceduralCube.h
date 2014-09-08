/*
Simple implementation of a procedural cube using the BaseGeometry class
Initially used for testing, has also become the base for the skybox class.
*/

#pragma once

#include "BaseGeometry.h"

class ProceduralCube : public BaseGeometry
{
public:
	ProceduralCube(GLfloat halfsize);
	~ProceduralCube(void);
	virtual void Render(void);
};
