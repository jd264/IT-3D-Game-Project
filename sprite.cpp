#include "sprite.h"
#include "logger.h"
#include "graphics.h"
#include <SDL_image.h>
#include <stdlib.h>

#define maxSprites 1024

static Sprite spriteList[maxSprites];

void initSpriteList()
{
	int x;
	memset(spriteList, 0, sizeof(Sprite) * maxSprites);
	for(x = 0; x < maxSprites; x++)
	{
		spriteList[x].image = NULL;
	}
	atexit(closeSprites);
}

Sprite *spriteNew()
{
	int i;
	for(i = 0; i < maxSprites; i++)
	{
		if(!spriteList[i].used)
		{
			memset(&spriteList[i], 0, sizeof(Sprite));
			spriteList[i].used = 1;
			return &spriteList[i];
		}
	}
	return NULL;
}

Sprite *spriteGetByFilename(char *filename)
{
	int i;
	for(i = 0; i < maxSprites; i++)
	{
		if((spriteList[i].used > 0) && (strcmp(spriteList[i].filename, filename) == 0))
		{
			return &spriteList[i];
		}
	}
	return NULL;
}

Sprite *loadSprite(char *filename, int fW, int fH)
{
	Sprite *sprite;
	SDL_Surface *image;
	int mode = GL_RGB;

	sprite = spriteGetByFilename(filename);
	if(sprite)
	{
		return sprite;
	}

	sprite = spriteNew();
	if(!sprite)
	{
		return NULL;
	}

	image = IMG_Load(filename);
	if(!image)
	{
		slog("Failed to load sprite image file %s, re: %s", filename, SDL_GetError());
		return NULL;
	}

	sprite->image = image;
	if(fW == -1)
	{
		sprite->w = sprite->image->w;
	}
	if(fH == -1)
	{
		sprite->h = sprite->image->h;
	}

	glGenTextures(1, &sprite->texture);
	glBindTexture(GL_TEXTURE_2D, sprite->texture);

	if(sprite->image->format->BitsPerPixel == 4)
	{
		mode= GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, mode, sprite->image->w, sprite->image->h, 0, mode, GL_UNSIGNED_BYTE, sprite->image->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return sprite;
}

void deleteSprite(Sprite *sprite)
{
	if(!sprite)
	{
		return;
	}
	if(sprite->image != NULL)
	{
		SDL_FreeSurface(sprite->image);
	}
	memset(sprite, 0, sizeof(Sprite));
}

void freeSprite(Sprite *sprite)
{
	/*is it being used?*/
	if(!sprite)
	{
		return;
	}
	sprite->used--;
	if(sprite->used <= 0)
	{
		deleteSprite(sprite);
	}
}

void closeSprites()
{
	int i;
	for(i = 0; i < maxSprites; i++)
	{
		deleteSprite(&spriteList[i]);
	}
}

/*eol&eof*/