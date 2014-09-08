#include "TechBase.h"

TechBase::TechBase(char * VScodeFileName, char * PScodeFileName)
{
	m_bExtensionSupport = true;
	m_bStatusOK = false;

	//Check for extensions support
	if(m_bExtensionSupport) {m_bExtensionSupport = IsExtensionSupported("GL_ARB_shader_objects");}
	if(m_bExtensionSupport) {m_bExtensionSupport = IsExtensionSupported("GL_ARB_shading_language_100");}
	if(m_bExtensionSupport) {m_bExtensionSupport = IsExtensionSupported("GL_ARB_vertex_shader");}
	if(m_bExtensionSupport) {m_bExtensionSupport = IsExtensionSupported("GL_ARB_fragment_shader");}
	if(m_bExtensionSupport) {m_bExtensionSupport = IsExtensionSupported("GL_ARB_multitexture");}

	//if all extensions needed are supported initialize
	if(m_bExtensionSupport)
	{
		m_bExtensionSupport = true;

		//Initialize member variables
		m_pVertexShaderSrc = NULL;
		m_pPixelShaderSrc = NULL;
		//Read vertex and pixel shaders source code from plaintext files into member strings
		m_pVertexShaderSrc = TextFileRead(VScodeFileName);
		m_pPixelShaderSrc = TextFileRead(PScodeFileName);

		//Get needed OpenGL extensions function pointers

		//Shader related functionality
		glCreateShaderObjectARB	= (PFNGLCREATESHADEROBJECTARBPROC) wglGetProcAddress("glCreateShaderObjectARB");
		glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC) wglGetProcAddress("glCreateProgramObjectARB");
		glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC) wglGetProcAddress("glShaderSourceARB");
		glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC) wglGetProcAddress("glCompileShaderARB");
		glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC) wglGetProcAddress("glAttachObjectARB");
		glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC) wglGetProcAddress("glLinkProgramARB");
		glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC) wglGetProcAddress("glUseProgramObjectARB");
		glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC) wglGetProcAddress("glDeleteObjectARB");
		glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC) wglGetProcAddress("glGetInfoLogARB");
		glGetObjectParameterfvARB = (PFNGLGETOBJECTPARAMETERFVARBPROC) wglGetProcAddress("glGetObjectParameterfvARB");
		glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC) wglGetProcAddress("glGetObjectParameterivARB");
		glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC) wglGetProcAddress("glGetUniformLocationARB");
		glUniform1fARB = (PFNGLUNIFORM1FARBPROC) wglGetProcAddress("glUniform1fARB");
		glUniform2fARB = (PFNGLUNIFORM2FARBPROC) wglGetProcAddress("glUniform2fARB");
		glUniform3fARB = (PFNGLUNIFORM3FARBPROC) wglGetProcAddress("glUniform3fARB");
		glUniform4fARB = (PFNGLUNIFORM4FARBPROC) wglGetProcAddress("glUniform4fARB");
		glUniform1iARB = (PFNGLUNIFORM1IARBPROC) wglGetProcAddress("glUniform1iARB");
		glUniform2iARB = (PFNGLUNIFORM2IARBPROC) wglGetProcAddress("glUniform2iARB");
		glUniform3iARB = (PFNGLUNIFORM3IARBPROC) wglGetProcAddress("glUniform3iARB");
		glUniform4iARB = (PFNGLUNIFORM4IARBPROC) wglGetProcAddress("glUniform4iARB");
		glUniform1fvARB = (PFNGLUNIFORM1FVARBPROC) wglGetProcAddress("glUniform1fvARB");
		glUniform2fvARB = (PFNGLUNIFORM2FVARBPROC) wglGetProcAddress("glUniform2fvARB"); 
		glUniform3fvARB = (PFNGLUNIFORM3FVARBPROC) wglGetProcAddress("glUniform3fvARB"); 
		glUniform4fvARB = (PFNGLUNIFORM4FVARBPROC) wglGetProcAddress("glUniform4fvARB"); 
		glUniform1ivARB = (PFNGLUNIFORM1IVARBPROC) wglGetProcAddress("glUniform1ivARB"); 
		glUniform2ivARB = (PFNGLUNIFORM2IVARBPROC) wglGetProcAddress("glUniform2ivARB"); 
		glUniform3ivARB = (PFNGLUNIFORM3IVARBPROC) wglGetProcAddress("glUniform3ivARB"); 
		glUniform4ivARB = (PFNGLUNIFORM4IVARBPROC) wglGetProcAddress("glUniform4ivARB");
		glUniformMatrix2fvARB = (PFNGLUNIFORMMATRIX2FVARBPROC) wglGetProcAddress("glUniformMatrix2fvARB");
		glUniformMatrix3fvARB = (PFNGLUNIFORMMATRIX3FVARBPROC) wglGetProcAddress("glUniformMatrix3fvARB");
		glUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVARBPROC) wglGetProcAddress("glUniformMatrix4fvARB");

		//Multitexturing related functionality (needed to pass tectures to shaders)
		glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
		glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");
		glMultiTexCoord1fARB = (PFNGLMULTITEXCOORD1FARBPROC) wglGetProcAddress("glMultiTexCoord1fARB");
		glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) wglGetProcAddress("glMultiTexCoord2fARB");
		glMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC) wglGetProcAddress("glMultiTexCoord3fARB");
		glMultiTexCoord4fARB = (PFNGLMULTITEXCOORD4FARBPROC) wglGetProcAddress("glMultiTexCoord4fARB");

		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&m_iMaxTexUnits);

		if(m_pVertexShaderSrc && m_pPixelShaderSrc)
		{
			//Get Program, VS and PS handles
			m_hProgram = glCreateProgramObjectARB();
			m_hVertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
			m_hPixelShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
			//Load Shader Sources
			glShaderSourceARB(m_hVertexShader, 1, (const GLcharARB**)&m_pVertexShaderSrc, NULL);
			glShaderSourceARB(m_hPixelShader, 1, (const GLcharARB**)&m_pPixelShaderSrc, NULL);
			//Compile The Shaders
			glCompileShaderARB(m_hVertexShader);
			glCompileShaderARB(m_hPixelShader);
			//Attach The Shader Objects To The Program Object
			glAttachObjectARB(m_hProgram, m_hVertexShader);
			glAttachObjectARB(m_hProgram, m_hPixelShader);
			//Link The Program Object
			glLinkProgramARB(m_hProgram);
			//Get vertex shader status and log
			GLsizei LogLen;
			glGetObjectParameterivARB(m_hVertexShader,GL_OBJECT_COMPILE_STATUS_ARB,&m_iVSCompileFlag);
			glGetObjectParameterivARB(m_hVertexShader,GL_OBJECT_INFO_LOG_LENGTH_ARB,&LogLen);
			m_pVertexShaderLog = new GLcharARB[LogLen];
			glGetInfoLogARB(m_hVertexShader, LogLen, NULL, m_pVertexShaderLog);
			//Get pixel shader status and log
			glGetObjectParameterivARB(m_hPixelShader,GL_OBJECT_COMPILE_STATUS_ARB,&m_iPSCompileFlag);
			glGetObjectParameterivARB(m_hPixelShader,GL_OBJECT_INFO_LOG_LENGTH_ARB,&LogLen);
			m_pPixelShaderLog = new GLcharARB[LogLen];
			glGetInfoLogARB(m_hPixelShader, LogLen, NULL, m_pPixelShaderLog);
			//Get program status and log
			glGetObjectParameterivARB(m_hProgram,GL_OBJECT_LINK_STATUS_ARB,&m_iProgramLinkFlag);
			glGetObjectParameterivARB(m_hProgram,GL_OBJECT_INFO_LOG_LENGTH_ARB,&LogLen);
			m_pProgramLog = new GLcharARB[LogLen];
			glGetInfoLogARB(m_hProgram, LogLen, NULL, m_pProgramLog);

			if(m_iVSCompileFlag == 1 && m_iPSCompileFlag == 1 && m_iProgramLinkFlag == 1)
			{
				m_bStatusOK = true;
			}
		}
	}
}

TechBase::~TechBase()
{
	if(m_bExtensionSupport)
	{
		delete [] m_pVertexShaderSrc;
		delete [] m_pPixelShaderSrc;
		glDeleteObjectARB(m_hProgram);
		glDeleteObjectARB(m_hVertexShader);
		glDeleteObjectARB(m_hPixelShader);
		delete [] m_pVertexShaderLog;
		delete [] m_pPixelShaderLog;
		delete [] m_pProgramLog;
	}
}

bool TechBase::IsExtensionSupported(GLcharARB* szTargetExtension)
{
	const unsigned char *pszExtensions = NULL;
	const unsigned char *pszStart;
	unsigned char *pszWhere, *pszTerminator;

	// Extension names should not have spaces
	pszWhere = (unsigned char *) strchr( szTargetExtension, ' ' );
	if( pszWhere || *szTargetExtension == '\0' )
		return false;

	// Get Extensions String
	pszExtensions = glGetString( GL_EXTENSIONS );

	// Search The Extensions String For An Exact Copy
	pszStart = pszExtensions;
	for(;;)
	{
		pszWhere = (unsigned char *) strstr( (const char *) pszStart, szTargetExtension );
		if( !pszWhere )
			break;
		pszTerminator = pszWhere + strlen( szTargetExtension );
		if( pszWhere == pszStart || *( pszWhere - 1 ) == ' ' )
			if( *pszTerminator == ' ' || *pszTerminator == '\0' )
				return true;
		pszStart = pszTerminator;
	}
	return false;
}

void TechBase::Set()
{
	if(m_bExtensionSupport)
	{
		if(m_bStatusOK)
		{
			glUseProgramObjectARB(m_hProgram);
		}
	}
}

void TechBase::BindTex2DToTextureUnit(GLuint hTexture, GLenum TextureUnitID)
{
	if(m_bExtensionSupport)
	{
		glActiveTextureARB(TextureUnitID);	
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hTexture);
	}
}

void TechBase::BindTexUnitToSampler(GLint TextureUnitNumber, GLcharARB * SamplerName)
{
	if(m_bExtensionSupport)
	{
		if(TextureUnitNumber >= 0 && TextureUnitNumber < m_iMaxTexUnits)
		{
			GLint hSampler = glGetUniformLocationARB(m_hProgram, SamplerName);
			glUniform1iARB(hSampler, TextureUnitNumber);
		}
	}
}

void TechBase::SetUniform1f(GLfloat Value, GLcharARB *UniformName)
{
	if(m_bExtensionSupport)
	{
		GLint hUniform = glGetUniformLocationARB(m_hProgram, UniformName);
		glUniform1fARB(hUniform, Value);
	}
}

void TechBase::SetUniform1i(GLint Value, GLcharARB *UniformName)
{
	if(m_bExtensionSupport)
	{
		GLint hUniform = glGetUniformLocationARB(m_hProgram, UniformName);
		glUniform1iARB(hUniform, Value);
	}
}

char* TechBase::TextFileRead(char *fn)
{
	FILE *fp;
	char* content;
	int count=0;
	if (fn != NULL) 
	{
		fopen_s(&fp,fn,"rt");
		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count > 0) 
			{
				content = new char[count+1];
				//content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

bool TechBase::TextFileWrite(char *fn, char *s) 
{
	FILE *fp;
	bool succeeded = FALSE;
	if (fn != NULL) {
		fopen_s(&fp,fn,"w");
		if (fp != NULL) 
		{
			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				succeeded = TRUE;
			fclose(fp);
		}
	}
	return(succeeded);
}
