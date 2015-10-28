#include <SDL.h>
#include "sprite.h"
#include "vector.h"

#ifndef __MODEL_H__
#define __MODEL_H__

typedef struct modelS
{
	char filename[512];

	Uint32 numTris;
	Uint32 numVertices;

	GLuint vertexBO;
	GLuint faceEAB;

	float *vertexArray;
	float *attributeArray;

	GLuint *triangleArray;

	Sprite *texture;
	Uint8 used;
}Model;

void modelInit();

Model *modelNew();

void modelFree(Model *model);

Model *modelGetByFilename(char *filename);

void modelAssignTexture(Model *model, char *texture);

size_t modelGetTriangleBufferSize(Model *model);

size_t modelGetVertexBufferSize(Model *model);

int modelAllocateTriangleBuffer(Model *model, GLuint triangles);

int modelAllocateAttributeBuffer(Model *model, GLuint attributes);

int modelAllocateVertexBuffer(Model *model, GLuint vertices);

#endif