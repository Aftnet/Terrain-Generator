#include "BaseGeometry.h"

BaseGeometry::BaseGeometry(void)
{
	//Get required extensions function pointers
	glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");
	glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
	glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");					
	glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");					
	glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");

	//Initialize members
	m_uiNumSubsets = 0;
	m_pSubsets = NULL;

	//Initialize vertices
	m_uiNumVertices = 0;
	m_pVertices = NULL;
	m_hVBOVertexBuffer = 0;

}

BaseGeometry::~BaseGeometry(void)
{
	GLuint i;
	if(m_pVertices != NULL)
	{
		delete[] m_pVertices;
	}
	glDeleteBuffersARB(1,&m_hVBOVertexBuffer);
	for(i=0;i<m_uiNumSubsets;i++)
	{
		glDeleteBuffersARB(1,&m_pSubsets[i].hVBOIndexBuffer);
	}
	if(m_pSubsets != NULL)
	{
		delete[] m_pSubsets;
	}
}

void BaseGeometry::SetNumVertices(GLuint numvertices)
{
	if(m_pVertices != NULL)
	{
		delete[] m_pVertices;
	}
	m_uiNumVertices = numvertices;
	m_pVertices = new VertexStr[m_uiNumVertices];
}

void BaseGeometry::GenerateVertexBuffer(void)
{
	glDeleteBuffersARB(1,&m_hVBOVertexBuffer);
	glGenBuffersARB(1,&m_hVBOVertexBuffer);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_hVBOVertexBuffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,m_uiNumVertices*sizeof(VertexStr),m_pVertices,GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
}

void BaseGeometry::SetNumSubsets(GLuint numsubsets)
{
	GLuint i;
	if(m_pSubsets != NULL)
	{
		for(i=0;i<m_uiNumSubsets;i++)
		{
			glDeleteBuffersARB(1,&m_pSubsets[i].hVBOIndexBuffer);
		}
		delete[] m_pSubsets;
	}
	m_uiNumSubsets = numsubsets;
	m_pSubsets = new SubsetStr[m_uiNumSubsets];
}

void BaseGeometry::ResizeSubset(GLuint numsubset, GLuint numtriangles)
{
	if(numsubset < m_uiNumSubsets)
	{
		if(m_pSubsets[numsubset].pIndices != NULL) 
		{
			glDeleteBuffersARB(1,&m_pSubsets[numsubset].hVBOIndexBuffer);
			delete[] m_pSubsets[numsubset].pIndices;
		}
		m_pSubsets[numsubset].NumTriangles = numtriangles;
		m_pSubsets[numsubset].pIndices = new GLuint[numtriangles*3];
	}
}

void BaseGeometry::GenerateSubsetIndicesBuffer(GLuint numsubset)
{
	if(numsubset < m_uiNumSubsets)
	{
		glDeleteBuffersARB(1,&m_pSubsets[numsubset].hVBOIndexBuffer);
		glGenBuffersARB(1,&m_pSubsets[numsubset].hVBOIndexBuffer);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_pSubsets[numsubset].hVBOIndexBuffer);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,3*m_pSubsets[numsubset].NumTriangles*sizeof(GLuint),m_pSubsets[numsubset].pIndices,GL_STATIC_DRAW_ARB);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);
	}
}

void BaseGeometry::DrawSubset(GLuint numsubset) const
{
	if(numsubset < m_uiNumSubsets)
	{
		//Set material properties
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_pSubsets[numsubset].Material.Specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, m_pSubsets[numsubset].Material.Emissive);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_pSubsets[numsubset].Material.Diffuse);
		glMaterialfv(GL_FRONT, GL_AMBIENT, m_pSubsets[numsubset].Material.Ambient);
		glMaterialfv(GL_FRONT, GL_SHININESS, m_pSubsets[numsubset].Material.Shininess);

		//Setup array usage states
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glClientActiveTextureARB(GL_TEXTURE0_ARB); 
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//Setup arrays
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_hVBOVertexBuffer);
		glVertexPointer(4,GL_FLOAT,sizeof(VertexStr),0);
		glNormalPointer(GL_FLOAT,sizeof(VertexStr),(GLvoid*)(sizeof(Vector4)));
		glTexCoordPointer(2,GL_FLOAT,sizeof(VertexStr),(GLvoid*)(2*sizeof(Vector4)));
		//Draw		
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_pSubsets[numsubset].hVBOIndexBuffer);
		glDrawElements(GL_TRIANGLES,3*m_pSubsets[numsubset].NumTriangles,GL_UNSIGNED_INT,0);
		//Restore previous state
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		// bind with 0, so, switch back to normal pointer operation
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	}
}