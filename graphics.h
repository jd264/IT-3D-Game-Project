#include <SDL.h>
#include <GL\glew.h>

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__


#define EPSILON		1e-6f

#define TWOPI	6.283185308

/* radians to degrees */
#define RADTODEG	57.295779513082

/*degress to radians */
#define DEGTORAD	0.017453292519943295769236907684886

/**
 * @brief initialize the 3d graphics system
 * @param sw the screen width you want
 * @param sh the screen height you want
 * @param fullscreen set true to initialize to fullscreen
 * @param project the name to appear on the game window
 * @param frameDelay the desired delay (in ms) between frames
 * @return -1 on error, 0 otherwise
 */
int graphicsInit(int sw, int sh, int fullscreen, const char *project, Uint32 frameDelay);

/**
 * @brief get the active shader program index
 * @return 0 on error or the index of the active shader program
 */

GLuint graphicsGetShaderProgram();

/**
 * @brief clear the drawing context and ready the next frame
 */
void graphicsFrameBegin();

/**
 * @brief advance the frame, taking into account desired frame rate
 */
void graphicsNextFrame();


#endif