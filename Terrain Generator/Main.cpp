/*
OpenGL application framework:
Sets up a windows application and acquires a viewport for OpenGL.
All application specific code is located in the CreateScene() and DrawScene() functions
CreateScene() setus up the scene upon starting the program and is executed only once
DrawScene() is the render loop, responsible for both issuing the actual render calls and responding to user input
Cleanup() releases any resources allocated upon quitting

From a design perspective, the render loop only concerns itself with how the rendered objects act in the scene (moving around, reacting to user input, etc.)
The objects comprising the scene are themselves responsible to define their appearance.
As such, every object is drawn to screen with a single call to their respective render method.

The framework was created by Matthew Bett.
*/

//Windows Example Code
//Matthew Bett 2005
//Shazam!

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Terrain.h"
#include "Water.h"
#include "SkyBox.h"
#include "MS3Dmodel.h"
#include "time.h"



#define COLOUR_DEPTH 24	//Colour depth

float pi = 3.14159265358979323846f;
float pi_over_2 = 1.57079632679489661923f;

HWND        ghwnd;
HDC			ghdc;
HGLRC		ghrc;			//hardware RENDERING CONTEXT
HINSTANCE	ghInstance;
RECT		gRect;
RECT		screenRect;

int S_WIDTH	 =	800;		//client area resolution
int S_HEIGHT =	600;


bool		keys[256];

typedef struct Mousestr
{
	Mousestr(){x=y=prevx=prevy=0;};
	bool rbutton, lbutton;
	int x,y,prevx,prevy;
} Mouse;

Mouse MouseStat;

clock_t timedelta,currtime,prevtime;

enum CameraStatesEnum {FREEFLY, SURFACEBOUND};

CameraStatesEnum CameraState; 

Vector4 CameraPos, CameraMove;
float alpha = 0.0f;
float beta = 0.0f;

//Light parameters
GLfloat SunLight_Ambient[4] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat SunLight_Diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat SunLight_Specular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat SunLight_Position[4]= {10.0f, 10.0f, 0.0f, 0.0f};

//Fog parameters
GLfloat FogColor[4]	= {0.0f, 0.0f, 0.0f, 1.0f};


//Application specific members
Terrain* Ground;
SkyBox* Sky;
Water* Sea;
MS3Dmodel* Tree[3];
#define NUMTREES 1000
Vector4 TreePos[NUMTREES];
float TreeRot[NUMTREES];
GLuint TreeType[NUMTREES];


//Functions prototypes
HWND CreateOurWindow(LPTSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);
bool SetPixelFormat(HDC hdc);
void ResizeGLWindow(int width, int height);
void InitializeOpenGL(int width, int height);
void Init(HWND hwnd);
void CreateScene();
void DrawScene();
void Cleanup();
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow);
void PopulateScene();



void Init(HWND hwnd)
{
	ghwnd = hwnd;
	GetClientRect(ghwnd, &gRect);	//get rect into our handy global rect
	InitializeOpenGL(gRect.right, gRect.bottom);// initialise openGL

	//OpenGL settings
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);				// Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glEnable(GL_LIGHTING);                              // Enable Lighting
	glEnable(GL_TEXTURE_2D);							// Enable texturing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);



	Sky = new SkyBox;
	Ground = new Terrain(256,256,4.0f,32,32);
	Sea = new Water(256,256,4,32,32);
	Tree[0] = new MS3Dmodel("Models\\Tree1.ms3d");
	Tree[1] = new MS3Dmodel("Models\\Tree2.ms3d");
	Tree[2] = new MS3Dmodel("Models\\Tree3.ms3d");

	//App initialization
	CreateScene();

	CameraState = FREEFLY;
	CameraPos = Vector4(0.0f,30.0f,0.0f,1.0f);
	currtime=prevtime=clock();
	srand(GLuint(currtime));
}

void CreateScene()
{
	GLuint i;
	float height;
	Vector4 terrainsize, norm;
	Vector4 upvec(0.0f,1.0f,0.0f,1.0f);
	
	Ground->Reset();
	Ground->Fault(250,5.0f,0.999f);
	Ground->RandomNoise(5.0f);
	Ground->Smooth(1,1);

	terrainsize = Ground->GetGridSize();
	height = Ground->GetMedianHeight();
	for(i=0;i<NUMTREES;i++)
	{
		TreePos[i] = Vector4((((float(rand())/float(RAND_MAX))-0.5f)*terrainsize.x), 0.0f, (((float(rand())/float(RAND_MAX))-0.5f)*terrainsize.z), 1.0f);
		TreePos[i].y = Ground->GetPointHeight(TreePos[i].x,TreePos[i].z) - 4.0f;
		//norm = Ground->GetPointNormal(TreePos[i].x,TreePos[i].z);
		//slope = norm.Dot4(Vector4(0.0f,1.0f,0.0f,0.0f));
		while(TreePos[i].y < height)
		{
			TreeRot[i] = (float(rand())/float(RAND_MAX))*360.0f;
			TreePos[i] = Vector4((((float(rand())/float(RAND_MAX))-0.5f)*terrainsize.x), 0.0f, (((float(rand())/float(RAND_MAX))-0.5f)*terrainsize.z), 1.0f);
			TreePos[i].y = Ground->GetPointHeight(TreePos[i].x,TreePos[i].z) - 0.2f;
		}
		TreeType[i] = rand()%3;
	}

	Sea->Reset(height-2.0f);
}

void DrawScene() 
{	
	GLuint i;

	//Get time delta

	currtime = clock();
	timedelta = currtime - prevtime;
	prevtime = currtime;

	//Camera Control
	if(MouseStat.lbutton)
	{
		alpha += 1.0f* float((MouseStat.x - MouseStat.prevx));
		beta += 1.0f* float((MouseStat.y - MouseStat.prevy));
		MouseStat.prevx = MouseStat.x;
		MouseStat.prevy = MouseStat.y;

		if(alpha>360.0f)
		{
			alpha-=360.0f;
		}
		if(alpha<0)
		{
			alpha+=360.0f;
		}
		if(beta>89.9f)
		{
			beta=89.9f;
		}
		if(beta<-89.9f)
		{
			beta=(-89.9f);
		}
	}

	if(keys[VK_NUMPAD1])
	{
		CameraState = FREEFLY;
	}
	else if(keys[VK_NUMPAD2])
	{
		CameraState = SURFACEBOUND;
	}
	
	CameraMove = Vector4(0.0f,0.0f,0.0f,0.0f);


	if(keys['W'])
	{
		CameraMove.x += 1.0f*sinf(alpha/180.0f*pi)*cosf(beta/180.0f*pi);
		CameraMove.y += -1.0f*sinf(beta/180.0f*pi);
		CameraMove.z += -1.0f*cosf(alpha/180.0f*pi)*cosf(beta/180.0f*pi);
		CameraMove.NormaliseSelf();
	}

	if(keys['S'])
	{
		CameraMove.x += -1.0f*sinf(alpha/180.0f*pi)*cosf(beta/180.0f*pi);
		CameraMove.y += 1.0f*sinf(beta/180.0f*pi);
		CameraMove.z += 1.0f*cosf(alpha/180.0f*pi)*cosf(beta/180.0f*pi);
		CameraMove.NormaliseSelf();
	}

	if(keys['A'])
	{
		CameraMove.x += -1.0f*cosf(alpha/180.0f*pi)*cosf(beta/180.0f*pi);
		CameraMove.z += -1.0f*sinf(alpha/180.0f*pi)*cosf(beta/180.0f*pi);
		CameraMove.NormaliseSelf();
	}

	if(keys['D'])
	{
		CameraMove.x += +1.0f*cosf(alpha/180.0f*pi)*cosf(beta/180.0f*pi);
		CameraMove.z += +1.0f*sinf(alpha/180.0f*pi)*cosf(beta/180.0f*pi);
		CameraMove.NormaliseSelf();
	}
	CameraMove = CameraMove * 0.05f * float(timedelta);
	CameraPos += CameraMove;

	if(CameraPos.x > Ground->GetGridSize().x*0.5f)
	{
		CameraPos.x = Ground->GetGridSize().x*0.5f;
	}
	if(CameraPos.x < Ground->GetGridSize().x*(-0.5f))
	{
		CameraPos.x = Ground->GetGridSize().x*(-0.5f);
	}
	if(CameraPos.z > Ground->GetGridSize().z*0.5f)
	{
		CameraPos.z = Ground->GetGridSize().z*0.5f;
	}
	if(CameraPos.z < Ground->GetGridSize().z*(-0.5f))
	{
		CameraPos.z = Ground->GetGridSize().z*(-0.5f);
	}

	if(CameraState == SURFACEBOUND)
	{
		CameraPos.y = Ground->GetPointHeight(CameraPos.x,CameraPos.z) + 10.0f;
	}

	if(keys['N'])
	{
		CreateScene();
	}



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Clear The Screen And The Depth Buffer

	//Setup Light
	glLightfv(GL_LIGHT0, GL_AMBIENT,  SunLight_Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  SunLight_Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR,  SunLight_Specular);
	glLightfv(GL_LIGHT0, GL_POSITION, SunLight_Position);
	glEnable(GL_LIGHT0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();// load Identity Matrix
	glPushMatrix();
	glRotatef(beta, 1,0,0);
	glRotatef(alpha, 0,1,0);
	Sky->Render();
	glPopMatrix();

	glPushMatrix();
	glRotatef(beta, 1,0,0);
	glRotatef(alpha, 0,1,0);
	glTranslatef(0.0f - CameraPos.x,0.0f - CameraPos.y,0.0f - CameraPos.z);
	Ground->Render();
	glPopMatrix();

	for(i=0; i<NUMTREES; i++)
	{
		glPushMatrix();
		glRotatef(beta,1,0,0);
		glRotatef(alpha,0,1,0);
		glTranslatef(TreePos[i].x - CameraPos.x, TreePos[i].y - CameraPos.y, TreePos[i].z - CameraPos.z);
		glRotatef(TreeRot[i],0,1,0);
		glDisable(GL_CULL_FACE);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER,0.1f);
		Tree[TreeType[i]]->Render();
		glDisable(GL_ALPHA_TEST);
		glEnable(GL_CULL_FACE);
		glPopMatrix();
	}

	glPushMatrix();
	glRotatef(beta, 1,0,0);
	glRotatef(alpha, 0,1,0);
	glTranslatef(0.0f - CameraPos.x,0.0f - CameraPos.y,0.0f - CameraPos.z);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	Sea->Render(float(currtime));
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glPopMatrix();

	SwapBuffers(ghdc);// Swap the frame buffers.
}		

void Cleanup()
{
	int i;
	delete Ground;
	delete Sea;
	delete Sky;
	for(i=0;i<3;i++)
	{
		delete Tree[i];
	}
	if (ghrc)
	{
		wglMakeCurrent(NULL, NULL);	// free rendering memory
		wglDeleteContext(ghrc);		// Delete our OpenGL Rendering Context
	}

	if (ghdc) 
		ReleaseDC(ghwnd, ghdc);			// Release our HDC from memory

	UnregisterClass(L"FirstWindowClass", ghInstance);// Free the window class

	PostQuitMessage (0);		// Post a QUIT message to the window
}

HWND CreateOurWindow(LPTSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
{
	HWND hwnd;

	WNDCLASS wcex;

	memset(&wcex, 0, sizeof(WNDCLASS));
	wcex.style			= CS_HREDRAW | CS_VREDRAW;		
	wcex.lpfnWndProc	= WndProc;		
	wcex.hInstance		= hInstance;						
	wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);; 
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);		
	wcex.hbrBackground	= (HBRUSH) (COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;	
	wcex.lpszClassName	= L"FirstWindowClass";	


	RegisterClass(&wcex);// Register the class

	dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	ghInstance	= hInstance;// Assign our global hInstance to the window's hInstance

	//Set the Client area of the window to be our resolution.
	RECT glwindow;
	glwindow.left		= 0;		
	glwindow.right		= width;	
	glwindow.top		= 0;		
	glwindow.bottom		= height;	

	AdjustWindowRect( &glwindow, dwStyle, false);

	//Create the window
	hwnd = CreateWindow(L"FirstWindowClass", 
		strWindowName, 
		dwStyle, 
		0, 
		0,
		glwindow.right  - glwindow.left,
		glwindow.bottom - glwindow.top, 
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if(!hwnd) return NULL;// If we could get a handle, return NULL

	ShowWindow(hwnd, SW_SHOWNORMAL);	
	UpdateWindow(hwnd);					
	SetFocus(hwnd);						

	return hwnd;
}

bool SetPixelFormat(HDC hdc) 
{ 
	PIXELFORMATDESCRIPTOR pfd = {0}; 
	int pixelformat; 

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// Set the size of the structure
	pfd.nVersion = 1;							// Always set this to 1
	// Pass in the appropriate OpenGL flags
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
	pfd.dwLayerMask = PFD_MAIN_PLANE;			// standard mask (this is ignored anyway)
	pfd.iPixelType = PFD_TYPE_RGBA;				// RGB and Alpha pixel type
	pfd.cColorBits = COLOUR_DEPTH;				// Here we use our #define for the color bits
	pfd.cDepthBits = COLOUR_DEPTH;				// Ignored for RBA
	pfd.cAccumBits = 0;							// nothing for accumulation
	pfd.cStencilBits = 0;						// nothing for stencil

	//Gets a best match on the pixel format as passed in from device
	if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == false ) 
	{ 
		MessageBox(NULL, L"ChoosePixelFormat failed", L"Error", MB_OK); 
		return false; 
	} 

	//sets the pixel format if its ok. 
	if (SetPixelFormat(hdc, pixelformat, &pfd) == false) 
	{ 
		MessageBox(NULL, L"SetPixelFormat failed", L"Error", MB_OK); 
		return false; 
	} 

	return true;
}

void ResizeGLWindow(int width, int height)// Initialize The GL Window
{
	if (height==0)// Prevent A Divide By Zero error
	{
		height=1;// Make the Height Equal One
	}

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//calculate aspect ratio
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 1 ,10000.0f);

	glMatrixMode(GL_MODELVIEW);// Select The Modelview Matrix
	glLoadIdentity();// Reset The Modelview Matrix
}

void InitializeOpenGL(int width, int height) 
{  
	ghdc = GetDC(ghwnd);//  sets  global HDC

	if (!SetPixelFormat(ghdc))//  sets  pixel format
		PostQuitMessage (0);


	ghrc = wglCreateContext(ghdc);	//  creates  rendering context from  hdc
	wglMakeCurrent(ghdc, ghrc);		//	Use this HRC.

	ResizeGLWindow(width, height);	// Setup the Screen
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{


	switch (message)											
	{														
	case WM_CREATE:	
		break;

	case WM_SIZE:
		//resize the open gl window when the window is resized
		ResizeGLWindow(LOWORD(lParam),HIWORD(lParam));
		GetClientRect(hwnd, &gRect);
		break;	

	case WM_KEYDOWN:
		keys[wParam]=true;
		break;

	case WM_KEYUP:
		keys[wParam]=false;
		break;

	case WM_MOUSEMOVE:
		if(MK_LBUTTON & wParam) {MouseStat.lbutton = true;} else  {MouseStat.lbutton = false;}
		if(MK_RBUTTON & wParam) {MouseStat.rbutton = true;} else  {MouseStat.rbutton = false;}
		MouseStat.prevx = MouseStat.x;
		MouseStat.prevy = MouseStat.y;
		MouseStat.x = int(LOWORD(lParam));
		MouseStat.y = int(HIWORD(lParam));
		break;

	case WM_PAINT:


		break;		

	case WM_DESTROY:	

		PostQuitMessage(0);	

		break;				
	}													

	return DefWindowProc (hwnd, message, wParam, lParam);		

}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PSTR szCmdLine, int nCmdShow)			
{	
	HWND		hwnd;
	MSG         msg;	

	//initialise and create window
	hwnd = CreateOurWindow(L"Terrain generator", S_WIDTH, S_HEIGHT, 0, false, hInstance);	
	if(hwnd == NULL) return true;

	//initialise opengl and other settings
	Init(hwnd);

	while(true)					
	{							
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message==WM_QUIT)
				break;
			TranslateMessage (&msg);							
			DispatchMessage (&msg);
		}

		else
		{		
			//any intensive proccessing for the app,  do it here. 
			DrawScene();
		}
	}

	return msg.wParam ;										
}



