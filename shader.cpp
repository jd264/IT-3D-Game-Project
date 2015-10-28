#include "shader.h"
#include "logger.h"
#include <stdio.h>

GLuint buildShaderProgram(const char *vsPath, const char *fsPath)
{
	GLint infoLogLength;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint tempProgram;
	GLint status;
	GLchar strInfoLog[4096];

	vertexShader = createShader(GL_VERTEX_SHADER, vsPath);
	fragmentShader = createShader(GL_FRAGMENT_SHADER, fsPath);

	tempProgram = glCreateProgram();

	glAttachShader(tempProgram, vertexShader);
	glAttachShader(tempProgram, fragmentShader);

	glLinkProgram(tempProgram);

	glGetProgramiv(tempProgram, GL_LINK_STATUS, &status);

	if(status == GL_FALSE)
	{
		glGetProgramiv(tempProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

		glGetProgramInfoLog(tempProgram, infoLogLength, NULL, strInfoLog);

		slog("Shader Link failure: %s", strInfoLog);
		return 0;
	}

	glDetachShader(tempProgram, vertexShader);
	glDetachShader(tempProgram, fragmentShader);

	return tempProgram;
}

GLuint createShader(GLenum eShaderType, const char *strShaderFile)
{
	char shaderSource[4096];
	char inChar;
	FILE *shaderFile;
	int i = 0;
	GLuint shader;
	GLint status;
	GLint infoLogLength;
	GLchar strInfoLog[4096];
	char strShaderType[16];
	const char *sS;

	shaderFile = fopen(strShaderFile, "r");
	if(!shaderFile)
	{
		slog("Failed to open shader file: %s", strShaderFile);
		return 0;
	}
	while(fscanf(shaderFile, "%c", &inChar) >0)
	{
		shaderSource[i++] = inChar; /*loading character to an array*/
	}
	shaderSource[i-1] = '\0';
	fclose(shaderFile);

	shader = glCreateShader(eShaderType);
	sS = shaderSource;
	glShaderSource(shader, 1, &sS, NULL);

	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		switch(eShaderType)
		{
		case GL_VERTEX_SHADER:
			sprintf(strShaderType, "vertex");
			break;
		case GL_GEOMETRY_SHADER:
			sprintf(strShaderType, "geometry");
			break;
		case GL_FRAGMENT_SHADER:
			sprintf(strShaderType, "fragment");
			break;
		}

		slog("Compile failure in %s shader: \n%s", strShaderType, strInfoLog);
		return 0;
	}

	return shader;
}

/*eol@eof*/