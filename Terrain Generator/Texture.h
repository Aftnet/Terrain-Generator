/*
A modified version of the texture loader from NeHe's tutorial that doesn't require GLaux 
(the library has been deprecated and is no longer included in Visual Studio 2008)
I have added a switch to set whether the loaded texture has to be wrapped or stretched.

The original can be found on NeHe's website here: http://nehe.gamedev.net/counter.asp?file=files/misc/nehe_glaux_replacement.zip
*/

#pragma once

#include <windows.h>		// Header File For Windows - has structures for BMP format
#include <stdio.h>	    	// Header File For Standard Input/Output
#include <stdlib.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Glext.h"

bool NeHeLoadBitmap(LPTSTR szFileName, GLuint &texid, bool wrap);