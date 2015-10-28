#include <stdio.h>
#include <stdlib.h>
#include "obj.h"
#include "logger.h"

#define objMax 1024

static Obj objList[objMax];

static void objClose();

void objInit()
{
	memset(objList, 0, sizeof(Obj)*objMax);
	atexit(objClose);
}

Obj *objNew()
{
	int i;
	for(i = 0; i < objMax; i++)
	{
		if(objList[i].used == 0)
		{
			memset(&objList[i], 0, sizeof(Obj));
			objList[i].used = 1;
			return &objList[i];
		}
	}
	return NULL;
}

Obj *objGetByFilename(char *filename)
{
	int i;
	for(i = 0; i <objMax; i++)
	{
		if((objList[i].used != 0) && (strcmp(objList[i].filename, filename) == 0))
		{
			return &objList[i];
		}
	}
	return NULL;
}

static void objDelete(Obj *obj)
{
	if(!obj)
	{
		return;
	}

	if(obj->vertexArray)
	{
		free(obj->vertexArray);
	}
	if(obj->texelArray)
	{
		free(obj->texelArray);
	}
	if(obj->normalArray)
	{
		free(obj->normalArray);
	}
	if(obj->triangleArray)
	{
		free(obj->triangleArray);
	}
	memset(obj, 0, sizeof(Model));
}

void objFree(Obj *obj)
{
	if(!obj)
	{
		return;
	}
	obj->used--;
	if(obj->used > 0)
	{
		return;
	}
	objDelete(obj);
}

static void objClose()
{
	int i;
	for(i = 0; i < objMax; i++)
	{
		if(objList[i].used)
		{
			objDelete(&objList[i]);
		}
	}
}

void objFileGetCounts(Obj* model, FILE* file)
{
	char buff[1024];
	int numVertices = 0;
	int numNormals = 0;
	int numTextCoords = 0;
	int numFaces = 0;

	if((file == NULL) || (model == NULL))
	{
		return;
	}
	while(fscanf(file, "%s", buff) != EOF)
    {
        switch(buff[0])
        {
            case 'v':
                switch(buff[1])
                {
                    case '\0':
                        fgets(buff, sizeof(buff), file);
                        numVertices++;
                        break;
                    case 'n':
                        fgets(buff, sizeof(buff), file);
                        numNormals++;
                        break;
                    case 't':
                        fgets(buff, sizeof(buff), file);
                        numTextCoords++;
                        break;
                    default:
                        break;
                }
                break;
            case 'f':
                fgets(buff, sizeof(buff), file);
                numFaces++;
                break;
            default:
                fgets(buff, sizeof(buff), file);
                break;
        }
    }
	model->numTris = numFaces;
	model->numVertices = numVertices;
	model->numTexels = numTextCoords;
	model->numNormals = numNormals;
}

void objAllocate(Obj *model)
{
	if(!model)
	{
		return;
	}
	if(model->numVertices)
	{
		model->vertexArray = (float *)malloc(sizeof(float)*3*model->numVertices);
		if (model->vertexArray)
        {
			memset(model->vertexArray,0,sizeof(float)*3*model->numVertices);
        }
	}
	if(model->numNormals)
	{
		model->normalArray = (float *) malloc(sizeof(float)*3*model->numNormals);
		if(model->normalArray)
		{
			memset(model->normalArray, 0, sizeof(float)*3*model->numNormals);
		}
	}
	if(model->numTexels)
	{
		model->texelArray = (float *)malloc(sizeof(float)*2*model->numTexels);
		if(model->texelArray)
		{
			memset(model->texelArray, 0, sizeof(float)*2*model->numTexels);
		}
	}
	if(model->numTris)
	{
		model->triangleArray = (ObjTriangle *)malloc(sizeof(ObjTriangle)*model->numTris);
		if(model->triangleArray)
		{
			memset(model->triangleArray, 0, sizeof(ObjTriangle)*model->numTris);
		}
	}
}

void objFileParse(Obj * model, FILE* file)
{
	int numVertices = 0;
	int numNormals = 0;
	int numTextCoords = 0;
	int numFaces = 0;
	char buff[128];
	float x, y, z;
	GLuint t1, t2, t3, v1, v2, v3, n1, n2, n3;

	if(model == NULL)
	{
		return;
	}
	if(file == NULL)
	{
		return;
	}
	rewind(file);
	while(fscanf(file, "%s", buff) != EOF)
	{
		 switch(buff[0])
        {
            case 'v':
                switch(buff[1])
                {
                    case '\0':
                        fscanf(
                            file,
                            "%f %f %f",
                            &x,
                            &y,
                            &z
                        );
						model->vertexArray[numVertices++] = x;
                        model->vertexArray[numVertices++] = y;
                        model->vertexArray[numVertices++] = z;
                        break;
                    case 'n':
                        fscanf(
                            file,
                            "%f %f %f",
                            &x,
                            &y,
                            &z
                        );
                        model->normalArray[numNormals++] = x;
                        model->normalArray[numNormals++] = y;
                        model->normalArray[numNormals++] = z;
                        break;
                    case 't':
                        fscanf(
                            file,
                            "%f %f",&x,&y
                        );
                        model->texelArray[numTextCoords++] = x;
                        model->texelArray[numTextCoords++] = 1 - y;
                        break;
                    default:
                        break;
                }
                break;
            case 'f':
                fscanf(
                    file,
                    "%u/%u/%u %u/%u/%u %u/%u/%u",
                    &v1,
                    &t1,
                    &n1,
                    &v2,
                    &t2,
                    &n2,
                    &v3,
                    &t3,
                    &n3);
                model->triangleArray[numFaces].p[0].vIndex = v1 - 1;
                model->triangleArray[numFaces].p[0].tIndex = t1 - 1;
                model->triangleArray[numFaces].p[0].nIndex = n1 - 1;
                model->triangleArray[numFaces].p[1].vIndex = v2 - 1;
                model->triangleArray[numFaces].p[1].tIndex = t2 - 1;
                model->triangleArray[numFaces].p[1].nIndex = n2 - 1;
                model->triangleArray[numFaces].p[2].vIndex = v3 - 1;
                model->triangleArray[numFaces].p[2].tIndex = t3 - 1;
                model->triangleArray[numFaces].p[2].nIndex = n3 - 1;
                numFaces++;
                break;
            default:
                /*chomp the line*/
                fgets(buff, sizeof(buff), file);
                break;
		 }
	}
}

int objVASearch(float *v, float *a, int count, float vtarget[3], float atarget[6])
{
	int i;
	if(!v || !a)
	{
		slog("error, missing a buffer to search");
		return -1;
	}
	for(i = 0; i < count; i++)
	{
		if(memcmp(&v[i*3], vtarget, sizeof(float)*3) !=0)
		{
			continue;
		}
		if(memcmp(&a[i*3], atarget, sizeof(float)*6) != 0)
		{
			continue;
		}
		return i;
	}
	return -1;
}

int objFileConvertToModel(Obj *objFile, Model *model)
{
	int i, j, count = 0;
	int search = 0;
	float *vertices;
	float *attributes;
	float *vtarget, atarget[6];
	if ((!model) || (!objFile))
	{
		slog("passed null data");
		return -1;
	}
	vertices = (float *)malloc(sizeof(float) * objFile->numTris * 3 * 3);
	attributes = (float *)malloc(sizeof(float) * objFile->numTris * 3 * 3 * 2);/*packing in normals and uvs*/
    if ((!vertices) || (!attributes))
    {
        slog("failed to alloacte vertex / attribute buffers!");
        if (vertices)free(vertices);
        if (attributes)free(attributes);
        return -1;
    }
    
    memset(atarget,0,sizeof(float)*6);
    if (modelAllocateTriangleBuffer(model, objFile->numTris) != 0)
    {
        return -1;
    }
    
    /*for each triangle...*/
    for (i = 0; i < objFile->numTris;i++)
    {
        /*for each triangle vertex...*/
        for (j = 0;j < 3;j++)
        {
            /*get a pointer to the vertex data*/
            vtarget = &objFile->vertexArray[objFile->triangleArray[i].p[j].vIndex];

            /*copy the normal data*/
            memcpy(atarget,&objFile->normalArray[objFile->triangleArray[i].p[j].nIndex],sizeof(float)*3);

            /*copy the texture data*/
            memcpy(&atarget[3],&objFile->texelArray[objFile->triangleArray[i].p[j].tIndex],sizeof(float)*2);
            
            /*check if we already had this vertex/normal/uv set before*/
            search = objVASearch(vertices,attributes,count,vtarget,atarget);
            if (search == -1)
            {
                /*new index*/
                memcpy(&vertices[count*3],vtarget,sizeof(float)*3);
                memcpy(&attributes[count*6],atarget,sizeof(float)*6);
                model->triangleArray[(i * 3)+j] = (GLuint)count;
                count++;
            }
            else
            {
                model->triangleArray[(i * 3)+j] = (GLuint)search;
            }
        }   
    }
    
    return 0;
}

Obj *objLoad(char *filename)
{
	FILE *file;
	Obj *objFile;

	objFile = objGetByFilename(filename);
	if(objFile)
	{
		objFile->used++;
		return objFile;
	}

	objFile = objNew();
	if(!objFile)
	{
		return  NULL;
	}

	file = fopen(filename, "r");
	if(file == NULL)
	{
		slog("failed to open file %s", filename);
		return NULL;
	}

	objFileGetCounts(objFile, file);

	slog("verices: %i", objFile->numVertices);
	slog("normals: %i", objFile->numNormals);
	slog("texels: %i", objFile->numTexels);
	slog("faces: %i", objFile->numTris);

	objAllocate(objFile);
	objFileParse(objFile, file);

	fclose(file);
	return objFile;
}

void objDraw(Obj *obj, Vec3D position, Vec3D rotation, Vec3D scale, Vec4D color, Sprite *texture)
{
	int i;
	ObjTriangle* triangle;
	float trans[4];

	if(obj == NULL)
	{
		slog("cannot draw obj, none specified");
		return;
	}
	glPushMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(texture != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture->texture);
	}

	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, trans);
	glDepthFunc(GL_LEQUAL);

	glTranslatef(position.x, position.y, position.z);
	glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
	glScalef(scale.x, scale.y, scale.z);

	glColor3f(color.x, color.y, color.z);
	glBegin(GL_TRIANGLES);
	for(i = 0; i < obj->numTris; i++)
	{
		triangle = &obj->triangleArray[i];
		if(obj->normalArray)
		{
			glNormal3f(obj->normalArray[triangle->p[0].nIndex * 3], obj->normalArray[triangle->p[0].nIndex * 3 + 1], obj->normalArray[triangle->p[0].nIndex * 3 + 2]);
		}
		 if (obj->texelArray)
        {
            glTexCoord2f(
                obj->texelArray[triangle->p[0].tIndex * 2],
                obj->texelArray[triangle->p[0].tIndex * 2 + 1]
            );
        }
        glVertex3f(
            obj->vertexArray[triangle->p[0].vIndex * 3],
            obj->vertexArray[triangle->p[0].vIndex * 3 + 1],
            obj->vertexArray[triangle->p[0].vIndex * 3 + 2]
        );
        
        if (obj->normalArray)
        {
            glNormal3f(
                obj->normalArray[triangle->p[1].nIndex * 3],
                obj->normalArray[triangle->p[1].nIndex * 3 + 1],
                obj->normalArray[triangle->p[1].nIndex * 3 + 2]
            );
        }
        if (obj->texelArray)
        {
            glTexCoord2f(
                obj->texelArray[triangle->p[1].tIndex * 2],
                obj->texelArray[triangle->p[1].tIndex * 2 + 1]
            );
        }
        glVertex3f(
            obj->vertexArray[triangle->p[1].vIndex * 3],
            obj->vertexArray[triangle->p[1].vIndex * 3 + 1],
            obj->vertexArray[triangle->p[1].vIndex * 3 + 2]);
        
        if (obj->normalArray)
        {
            glNormal3f(
                obj->normalArray[triangle->p[2].nIndex * 3],
                obj->normalArray[triangle->p[2].nIndex * 3 + 1],
                obj->normalArray[triangle->p[2].nIndex * 3 + 2]
            );
        }
        if (obj->texelArray)
        {
            glTexCoord2f(
                obj->texelArray[triangle->p[2].tIndex * 2],
                obj->texelArray[triangle->p[2].tIndex * 2 + 1]
            );
        }
        glVertex3f(
            obj->vertexArray[triangle->p[2].vIndex * 3],
            obj->vertexArray[triangle->p[2].vIndex * 3 + 1],
            obj->vertexArray[triangle->p[2].vIndex * 3 + 2]);
        
    }
    glEnd();
    
    glColor4f(1,1,1,1);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glDisable(GL_COLOR_MATERIAL);
    if(texture != NULL)
    {
        glDisable(GL_TEXTURE_2D);
    }    
    glPopMatrix();
}

Model *objLoadModel(char *filename)
{
    Model *model;
    Obj *objFile;
    model = modelGetByFilename(filename);
    if (model)
    {
        return model;
    }    
    objFile = objLoad(filename);
    if (!objFile)
    {
        return NULL;
    }
    
    model = modelNew();
    if (!model)
    {
        return NULL;
    }
        
    if (objFileConvertToModel(objFile,model) != 0)
    {
        modelFree(model);        
        return NULL;
    }
    
    return model;
}

/*eol@eof*/