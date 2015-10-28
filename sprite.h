#include "graphics.h"
#include <SDL.h>

#ifndef __SPRITE__
#define __SPRITE__

#define MAXSPRITEFILENAME 512

typedef struct spriteT
{
	SDL_Surface *image;
	GLuint texture;
	char filename[MAXSPRITEFILENAME];
	int w, h;
	int framesPerLine;
	int color1, color2, color3;
	Uint8 used;
}Sprite;

void initSpriteList();

void freeSprite(Sprite *img);

Sprite *loadSprite(char *filename, int sizeX, int sizeY);

void drawSprite(Sprite *sprite, SDL_Surface *surface, int sX, int sY, int frame);

void closeSprites();

#endif