/*
TechBase class: as the name suggest, the class encapsulates the operations required to create an OpenglARB program
with corresponding vertex and pixel shaders techniques and to compile them using GLSL code specified in two plaintext files.
The class is meant never to be instantiated, existing only as a basis for technique specific derived classes;
needless to say, all technique specific data (materials, textures, etc.) is to be part of the derived classes.

A note on shaders:
Most of the shaders have been built upon basic GLSL samples (Phong shading) I found on the internet.
*/

#pragma once

#include <windows.h>
#include <fstream>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Glext.h"

class TechBase
{
public:
	virtual void Set();
protected:
	TechBase(char * VScode, char * PScode);
	~TechBase();
	bool IsExtensionSupported(GLcharARB* szTargetExtension);
	void BindTex2DToTextureUnit(GLuint hTexture, GLenum TextureUnitID);
	void BindTexUnitToSampler(GLint TextureUnitNumber, GLcharARB * SamplerName);
	void SetUniform1f(GLfloat Value, GLcharARB *UniformName);
	void SetUniform1i(GLint Value, GLcharARB *UniformName);


	//Status flags
	bool m_bExtensionSupport;
	bool m_bStatusOK;

	//ARB Program Object handle
	GLenum m_hProgram;
	//ARB Vertex Shader Object handle
	GLenum m_hVertexShader;
	//ARB Pixel Shader Object handle
	GLenum m_hPixelShader;

	//Number of available texture units
	GLint m_iMaxTexUnits;

	//Pointers to source code strings
	GLcharARB *m_pVertexShaderSrc;
	GLcharARB *m_pPixelShaderSrc;
	//Pointers to log strings and log length variables
	GLcharARB *m_pVertexShaderLog;
	GLint m_iVSCompileFlag;
	GLcharARB *m_pPixelShaderLog;
	GLint m_iPSCompileFlag;
	GLcharARB *m_pProgramLog;
	GLint m_iProgramLinkFlag;

	//Text handling functions
	char * TextFileRead(char *fn);
	bool TextFileWrite(char *fn, char *s);

	//OpenGL extensions Function Pointers

	//Shader related functions
	PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
	PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
	PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
	PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
	PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
	PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
	PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
	PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
	PFNGLGETINFOLOGARBPROC glGetInfoLogARB;	
	PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB;
	PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
	PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
	PFNGLUNIFORM1FARBPROC glUniform1fARB;
	PFNGLUNIFORM2FARBPROC glUniform2fARB;
	PFNGLUNIFORM3FARBPROC glUniform3fARB;
	PFNGLUNIFORM4FARBPROC glUniform4fARB;
	PFNGLUNIFORM1IARBPROC glUniform1iARB;
	PFNGLUNIFORM2IARBPROC glUniform2iARB;
	PFNGLUNIFORM3IARBPROC glUniform3iARB;
	PFNGLUNIFORM4IARBPROC glUniform4iARB;
	PFNGLUNIFORM1FVARBPROC glUniform1fvARB;
	PFNGLUNIFORM2FVARBPROC glUniform2fvARB;
	PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
	PFNGLUNIFORM4FVARBPROC glUniform4fvARB;
	PFNGLUNIFORM1IVARBPROC glUniform1ivARB;
	PFNGLUNIFORM2IVARBPROC glUniform2ivARB;
	PFNGLUNIFORM3IVARBPROC glUniform3ivARB;
	PFNGLUNIFORM4IVARBPROC glUniform4ivARB;
	PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fvARB;
	PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fvARB;
	PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB;

	//Multitexturing related functions
	PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
	PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB;
	PFNGLMULTITEXCOORD1FARBPROC	glMultiTexCoord1fARB;
	PFNGLMULTITEXCOORD2FARBPROC	glMultiTexCoord2fARB;
	PFNGLMULTITEXCOORD3FARBPROC	glMultiTexCoord3fARB;
	PFNGLMULTITEXCOORD4FARBPROC	glMultiTexCoord4fARB;
};