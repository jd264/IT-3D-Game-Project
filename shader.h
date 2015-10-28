#include <GL\glew.h>

#ifndef __SHADER_H__
#define __SHADER_H__

GLuint buildShaderProgram(const char *vsPath, const char *fsPath);
GLuint createShader(GLenum eShaderType, const char *strShaderFile);

#endif