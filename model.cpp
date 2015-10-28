#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include "model.h"

#define modelMax 1024

static Model modelList[modelMax];

static void modelClose();

void modelInit()
{
	memset(modelList, 0, sizeof(Model)* modelMax);
	atexit(modelClose);
}

static void modelDelete(Model *model)
{
	if(!model)
	{
		return;
	}

	freeSprite(model->texture);
	if(model->vertexArray)
	{
		free(model->vertexArray);
	}
	if(model->attributeArray)
	{
		free(model->attributeArray);
	}
	if(model->triangleArray)
	{
		free(model->triangleArray);
	}
	memset(model, 0, sizeof(Model));
}

void modelFree(Model *model)
{
	if(!model)
	{
		return;
	}
	model->used--;
	if(model->used > 0)
	{
		return;
	}
	modelDelete(model);
}

static void modelClose()
{
	int i;
	for(i = 0; i < modelMax; i++)
	{
		if(modelList[i].used)
		{
			modelDelete(&modelList[i]);
		}
	}
}

Model *modelNew()
{
	int i;
	for(i = 0; i < modelMax; i++)
	{
		if(modelList[i].used == 0)
		{
			memset(&modelList[i], 0, sizeof(Model));
			modelList[i].used = 1;
			return &modelList[i];
		}
	}
	return NULL;
}

Model *modelGetByFilename(char *filename)
{
	int i;
	for(i = 0; i < modelMax; i++)
	{
		if((modelList[i].used !=0) && (strcmp(modelList[i].filename, filename) == 0))
		{
			return &modelList[i];
		}
	}
	return NULL;
}

void modelAssisgnTexture(Model *model, char *texture)
{
	Sprite *sprite;
	if(!model)
	{
		return;
	}
	sprite = loadSprite(texture, -1, -1);
	if(!sprite)
	{
		return;
	}
	model->texture = sprite;
}

int modelAllocateTriangleBuffer(Model *model, GLuint triangles)
{
	if(!model)
	{
		slog("No model specified");
		return -1;
	}
	if(model->triangleArray != NULL)
	{
		slog("model %s already has a triangle buffer");
		return -1;
	}
	if(!triangles)
	{
		slog("cannot allocate 0 triangles");
		return -1;
	}
	model->triangleArray = (GLuint *)malloc(sizeof(GLuint)*3*triangles);
	if(!model->triangleArray)
	{
		slog("failed to allocate triangle buffer");
		return -1;
	}
	memset(model->triangleArray, 0, sizeof(GLuint)*3*triangles);
	model->numTris = triangles;
	return 0;
}

int modelSetVertexBuffer(Model *model, float *vertexBuffer, GLuint count)
{
	if(modelAllocateVertexBuffer(model, count) != 0)
	{
		return -1;
	}
	memcpy(model->vertexArray, vertexBuffer, sizeof(float)*count*3);
	return 0;
}

int modelSetAttributeBuffer(Model *model, float *attributeBuffer, GLuint count)
{
	if(modelAllocateAttributeBuffer(model, count) != 0)
	{
		return -1;
	}
	memcpy(model->attributeArray, attributeBuffer, sizeof(float)*count*6);
	return 0;
}

int modelAllocateVertexBuffer(Model *model, GLuint vertices)
{
	if(!model)
	{
		slog("no model specified");
		return -1;
	}
	if(model->vertexArray != NULL)
	{
		slog("model %s already has a vertex buffer");
		return -1;
	}
	if(!vertices)
	{
		slog("cannot allocate 0 vertices");
		return -1;
	}
	model->vertexArray = (float *)malloc(sizeof(float)*3*vertices);
	if(!model->vertexArray)
	{
		slog("failed to allocate vertex buffer");
		return -1;
	}
	memset(model->vertexArray, 0, sizeof(float)*3*vertices);
	model->numVertices = vertices;
	return 0;
}

int modelAllocateAttributeBuffer(Model *model, GLuint attributes)
{
	if(!model)
	{
		slog("no model specified");
		return -1;
	}
	if(model->attributeArray != NULL)
	{
		slog("model %s already has a vertex buffer");
		return -1;
	}
	if(!attributes)
	{
		slog("cannot allocate 0 attributes");
		return -1;
	}
	model->attributeArray = (float *)malloc(sizeof(float)*6*attributes);
	if(!model->attributeArray)
	{
		slog("failed to create attribute buffer");
		return -1;
	}
	memset(model->attributeArray, 0, sizeof(float)*6*attributes);
	return 0;
}

size_t modelGetTriangleBufferSize(Model *model)
{
	if(!model)
	{
		return 0;
	}
	return (sizeof(GLshort)*model->numTris*3);
}

size_t modelGetVertexBufferSize(Model *model)
{
	if(!model)
	{
		return 0;
	}
	return (sizeof(float)*3*model->numVertices);
}

/*eol@eof*/