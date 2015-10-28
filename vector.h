#pragma once

#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef struct
{
	float x, y;
}Vec2D;

typedef struct
{
	float x, y, z;
}Vec3D;

typedef struct
{
	float x, y, z, w;
}Vec4D;

Vec2D vec2D(float x, float y);
Vec3D vec3D(float x, float y, float z);
Vec4D vec4D(float x, float y, float z, float w);

#define vec2DAdd(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y)
#define vec3DAdd(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y,dst.z = a.z+b.z)
#define vec4DAdd(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y,dst.z = a.z+b.z,dst.w = a.w+b.w)

#endif