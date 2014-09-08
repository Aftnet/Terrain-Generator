/*
This class creates a geometry by reading a .ms3d binary file generated from the MilkShape 3D modelling package.
The object can be divided in an arbitrary number of subsets, each having its own associated material parameters and a texture.
The object is rendered using standard per pixel phong lighting; there is no support for animation.
*/

#pragma once

#include "BaseGeometry.h"
#include "MS3Dff.h"
#include <iostream>
#include <fstream>
#include "Libs\SOIL.h"
#include "PhongTechs.h"

class MS3Dmodel : public BaseGeometry
{
public:
	MS3Dmodel(char * filename);
	~MS3Dmodel(void);
	void Render(void);

protected:
	GLuint *m_hTex;

private:
	PhongTech m_Tech;
	PhongNoTexTech m_TechNoTex;
};
