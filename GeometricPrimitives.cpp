#include "GeometricPrimitives.h"

int vertCount = 72;
GLfloat cubeVerts[] = {
	//BACK
	-1.f,-1.f,-1.f,
	1.f,-1.f,-1.f,
	1.f,1.f,-1.f,
	-1.f,1.f,-1.f,

	//FRONT
	-1.f,-1.f,1.f,
	-1.f,1.f,1.f,
	1.f,1.f,1.f,
	1.f,-1.f,1.f,

	//TOP
	-1.f,1.f,-1.f,
	-1.f,1.f,1.f,
	 1.f,1.f,1.f,
	 1.f,1.f,-1.f,

	 //BOTTOM
	 -1.f,-1.f,-1.f,
	 -1.f,-1.f,1.f,
	 1.f,-1.f,1.f,
	 1.f,-1.f,-1.f,

	 //RIGHT
	 1.f,-1.f,-1.f,
	 1.f,1.f,-1.f,
	 1.f,1.f,1.f,
	 1.f,-1.f,1.f,

	 //LEFT
	 -1.f,-1.f,-1.f,
	 -1.f,1.f,-1.f,
	 -1.f,1.f,1.f,
	 -1.f,-1.f,1.f,
};

int cubeIndicesCount = 36;
GLuint cubeIndices[] = {
	0,1,2, 0,2,3,
	4,5,6, 4,6,7,
	8,9,10, 8,10,11,
	12,13,14, 12,14,15,
	16,17,18, 16,18,19,
	20,21,22, 20,22,23
};

int UVCount = 48;
GLfloat cubeUV[] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
};

GLfloat cubeNormals[] = {
	//BACK
	 0, 0, -1,
	 0, 0, -1,
	 0, 0, -1,
	 0, 0, -1,

	 //FRONT
	  0, 0, 1,
	 0, 0, 1,
	 0, 0, 1,
	 0, 0, 1,

	 //TOP
	  0, 1,0,
	  0, 1,0,
	  0, 1,0,
	  0, 1,0,

	  //BOTTOM
	  0, -1,0,
	  0, -1,0,
	  0, -1,0,
	  0, -1,0,

	  //RIGHT
	  1,  0, 0,
	  1,  0, 0,
	  1,  0, 0,
	  1,  0, 0,

	  //LEFT
	  -1,  0, 0,
	  -1,  0, 0,
	  -1,  0, 0,
	  -1,  0, 0,
};



GLfloat planeVerts[] = {
	-10,-10,-1,
	10,-10,-1,
	10,10,-1,
	-10,10,-1,
};

GLuint planeIndices[] = {
	0,1,2, 0,2,3
};

GLfloat planeNormals[] = {
	0,0,1,
	0,0,1,
	0,0,1,
	0,0,1,
};

GLfloat planeUV[] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f
};