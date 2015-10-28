#include "graphics.h"
#include "logger.h"
#include "shader.h"
#include <GL\GLU.h>
#include <stdlib.h>

static SDL_GLContext	graphicsGLContext;
static SDL_Window  *	graphicsWindow = NULL;
static GLuint			graphicsShaderProgram;
static Uint32			graphicsFrameDelay = 33;

void graphicsClose();

GLuint graphicsGetShadeProgram()
{
	return graphicsShaderProgram;
}

void graphicsSetupDefLight();

int graphicsInit(int sw, int sh, int fullscreen, const char *project, Uint32 frameDelay)
{
	const unsigned char *version;
	GLenum glewStatus;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		slog("Failed to initialize SDL!");
		return -1;
	}
	atexit(SDL_Quit);
	graphicsFrameDelay = frameDelay;

	graphicsWindow = SDL_CreateWindow(project?project:"3D Game",
										SDL_WINDOWPOS_UNDEFINED,
										SDL_WINDOWPOS_UNDEFINED,
										sw, sh,
										SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	graphicsGLContext = SDL_GL_CreateContext(graphicsWindow);
	if(graphicsGLContext == NULL)
	{
		slog("There was an error creating the OpenGL context!\n");
		return -1;
	}

	version = glGetString(GL_VERSION);
	if(version == NULL)
	{
		slog("There was an error creating the OpenGL context!\n");
		return -1;
	}

	SDL_GL_MakeCurrent(graphicsWindow, graphicsGLContext);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	//MUST make a context AMD make it current BEFORE glewInit()!
	glewExperimental = GL_TRUE;
	glewStatus = glewInit();
	if(glewStatus != 0)
	{
		slog("Error: %s", glewGetErrorString(glewStatus));
		return -1;
	}

	graphicsShaderProgram = buildShaderProgram("shader/vs1.glsl", "shaders/fs1.glsl");
	if(graphicsShaderProgram == -1)
	{
		return -1;
	}

	glViewport(0, 0, sw, sh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/*view angle, aspect ratio, near clip distance, far clip distance*/
    /*TODO: put near/far clip in graphics view config*/
	gluPerspective(40, (float)sw/(float)sh, 0.1, 2000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	/*Enables alpha testing*/
	/*glAlphaFunc(GL_GREATER,0);
    glEnable(GL_ALPHA_TEST);*/

	graphicsSetupDefLight();
	atexit(graphicsClose);
	return 0;
}

void graphicsClose()
{
	SDL_GL_DeleteContext(graphicsGLContext);
}

void graphicsFrameBegin()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();
}

void graphicsNextFrame()
{
	static Uint32 then = 0;
	Uint32 now;
	glPopMatrix();
	SDL_GL_SwapWindow(graphicsWindow);
	now = SDL_GetTicks();
	if((now - then) < graphicsFrameDelay)
	{
		SDL_Delay(graphicsFrameDelay - (now - then));
	}
	then = now;
}

void graphicsSetupDefLight()
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { -10.0, -10.0, 10.0, 0.0 };
    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    
    GLfloat light1_ambient[] = { 1.2, 1.2, 1.2, 1.0 };
    GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light1_position[] = { -20.0, 2.0, 1.0, 1.0 };
    GLfloat spot_direction[] = { -1.0, -1.0, 0.0 };
    
    GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    
    //glShadeModel (GL_SMOOTH);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.9);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.7);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.3);
    
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 80.0);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
    
}

/*eol@eof*/