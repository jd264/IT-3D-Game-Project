#include "model.h"
#include "vector.h"

#ifndef __OBJ_H__
#define __OBJ_H__

typedef struct triPointS
{
	GLuint vIndex;
	GLuint nIndex;
	GLuint tIndex;
}TriPoint;

typedef struct
{
	TriPoint p[3];
}ObjTriangle;

typedef struct
{
	char filename[512];

	Uint32 numTris;
	Uint32 numVertices;
	Uint32 numTexels;
	Uint32 numNormals;

	float *vertexArray;
	float *texelArray;
	float *normalArray;
	ObjTriangle *triangleArray;

	Uint8 used;
}Obj;

void objInit();

Model *objLoadModel(char *filename);

Obj *objLoad(char *filename);

void objDraw(Obj *obj, Vec3D position, Vec3D rotation, Vec3D scale, Vec4D color, Sprite *texture);

#endif