/**
*
* 3D Game
*
**/

#include "logger.h"
#include "shader.h"
#include "obj.h"
#include "vector.h"
#include "sprite.h"
#include <math.h>

void setCamera(Vec3D position, Vec3D rotation);

int main(int argc, char *argv[])
{
	float bgSpeed = 0;
	int bgPlace =140;
	bool isGrounded = true;
	float jumpPos = 0;
	int xSpeed = 0;

	GLuint vao;
	float r= 0;
	GLuint triangleBufferObject;
	char bGameLoopRunning = 1;
	Vec3D cameraPosition = {0, -10, 0.3};
	Vec3D cameraRotation = {90, 0, 0};
	SDL_Event e;
	Obj *obj, *bgobj;
	Sprite *texture, *bgtext;
	const float triangleVertices[] = {
		0.0f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.366f, 0.0f, 1.0f,
        -0.5f, -0.366f, 0.0f, 1.0f,
        //next part contains vertex colors
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f  
    };

	loggerInit("3D_Game.log");
	if(graphicsInit(1024, 768, 1, "3D Game", 33) !=0)
	{
		return -1;
	}
	modelInit();
	objInit();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &triangleBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, triangleBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	obj = objLoad("cube.obj");
	texture = loadSprite("cube_text.png", 1024, 1024);

	bgobj = objLoad("hallway.obj");
	bgtext = loadSprite("mountain_text.jpg", 1024, 1024);

	while(bGameLoopRunning)
	{
		while ( SDL_PollEvent(&e) ) 
        {
            if (e.type == SDL_QUIT)
            {
                bGameLoopRunning = 0;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    bGameLoopRunning = 0;
                }
                else if (e.key.keysym.sym == SDLK_SPACE && isGrounded == true)
                {
					isGrounded = false;
                }
				else if (e.key.keysym.sym == SDLK_d)
                {
					xSpeed = 2;
                }
                else if (e.key.keysym.sym == SDLK_a)
                {
                    xSpeed = -2;
                }
                else if (e.key.keysym.sym == SDLK_LEFT)
                {
                    cameraRotation.z += 1;
                }
                else if (e.key.keysym.sym == SDLK_RIGHT)
                {
                    cameraRotation.z -= 1;
                }
                else if (e.key.keysym.sym == SDLK_UP)
                {
                    cameraRotation.x += 1;
                }
                else if (e.key.keysym.sym == SDLK_DOWN)
                {
                    cameraRotation.x -= 1;
                }
            }
			else if (e.type == SDL_KEYUP)
			{
				if (e.key.keysym.sym == SDLK_d)
                {
                    xSpeed = 0;
                }
                else if (e.key.keysym.sym == SDLK_a)
                {
                    xSpeed = 0;
                }
			}
		}

		graphicsFrameBegin();
		
		glPushMatrix();
		setCamera(cameraPosition, cameraRotation);

		if(isGrounded == false && jumpPos <= 5)
		{
			jumpPos+= 0.5;
		}
		else if(jumpPos <= 0)
		{
			jumpPos = 0;
		}
		else
		{
			isGrounded = true;
			jumpPos-=0.5;
		}

		objDraw(bgobj, vec3D(0, 140+ (bgSpeed-= 0.5), 2), vec3D(90, 0, 0), vec3D(5, 5, 15), vec4D(1, 1, 1, 1), bgtext);

		objDraw(obj, vec3D(cameraPosition.x + xSpeed, cameraPosition.y + 10 , cameraPosition.z-2 + jumpPos), vec3D(90, r++, 0), vec3D(0.5, 0.5, 0.5), vec4D(1, 1, 1, 1), texture);

		if(r > 360)
		{
			r -= 360;
		}
		glPopMatrix();
		graphicsNextFrame();
	}
	return 0;
}

void setCamera(Vec3D position, Vec3D rotation)
{
	glRotatef(-rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(-rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(-rotation.z, 0.0f, 0.0f, 1.0f);
    glTranslatef(-position.x, -position.y, -position.z);
}

/*eol@eof*/