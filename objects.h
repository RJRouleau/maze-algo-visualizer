#pragma once

#include "glew.h"


// I-shaped room
static GLuint IShapeIndices[][3] =
{
	{1, 3, 7},
	{1, 7, 5},
	{0, 4, 6},
	{0, 6, 2},
	{2, 6, 7},
	{2, 7, 3},
	{0, 1, 5},
	{0, 5, 4}
};


static GLfloat IShapeVertices[][3] =
{
	{-1., -1., -1.},
	{1., -1., -1.},
	{-1., 1., -1.},
	{1., 1., -1.},
	{-1., -1., 1.},
	{1., -1., 1.},
	{-1., 1., 1.},
	{1., 1., 1.}
};


// Normals are aligned with triangles in Indices.
// There are 2 triangles per face of the cube and 1 normal per face.
static GLfloat IShapeNormals[][3] =
{
	{-1., 0., 0.},
	{-1., 0., 0.},
	{1., 0., 0.},
	{1., 0., 0.},
	{0., -1., 0.},
	{0., -1., 0.},
	{0., 1., 0.},
	{0., 1., 0.}
};


// dead end room
static GLuint DeadEndIndices[][3] =
{
	{0, 2, 3}, // back
	{0, 3, 1},
	{1, 3, 7}, // right
	{1, 7, 5},
	{0, 4, 6}, // left
	{0, 6, 2},
	{2, 6, 7}, // top
	{2, 7, 3},
	{0, 1, 5}, // bottom
	{0, 5, 4}
};


static GLfloat DeadEndVertices[][3] =
{
	{-1., -1., -1.},
	{1., -1., -1.},
	{-1., 1., -1.},
	{1., 1., -1.},
	{-1., -1., 1.},
	{1., -1., 1.},
	{-1., 1., 1.},
	{1., 1., 1.}
};


static GLfloat DeadEndNormals[][3] =
{
	{0., 0., 1.},
	{0., 0., 1.},
	{-1., 0., 0.},
	{-1., 0., 0.},
	{1., 0., 0.},
	{1., 0., 0.},
	{0., -1., 0.},
	{0., -1., 0.},
	{0., 1., 0.},
	{0., 1., 0.}
};


// L-shaped room
static GLuint LShapeIndices[][3] =
{
	{0, 2, 3},
	{0, 3, 1},
	{0, 4, 6},
	{0, 6, 2},
	{2, 6, 7},
	{2, 7, 3},
	{0, 1, 5},
	{0, 5, 4}
};


static GLfloat LShapeVertices[][3] =
{
	{-1., -1., -1.},
	{1., -1., -1.},
	{-1., 1., -1.},
	{1., 1., -1.},
	{-1., -1., 1.},
	{1., -1., 1.},
	{-1., 1., 1.},
	{1., 1., 1.}
};


static GLfloat LShapeNormals[][3] =
{
	{0., 0., 1.},
	{0., 0., 1.},
	{1., 0., 0.},
	{1., 0., 0.},
	{0., -1., 0.},
	{0., -1., 0.},
	{0., 1., 0.},
	{0., 1., 0.}
};


// T-shaped room
static GLuint TShapeIndices[][3] =
{
	{0, 2, 3},
	{0, 3, 1},
	{2, 6, 7},
	{2, 7, 3},
	{0, 1, 5},
	{0, 5, 4}
};


static GLfloat TShapeVertices[][3] =
{
	{-1., -1., -1.},
	{1., -1., -1.},
	{-1., 1., -1.},
	{1., 1., -1.},
	{-1., -1., 1.},
	{1., -1., 1.},
	{-1., 1., 1.},
	{1., 1., 1.}
};


static GLfloat TShapeNormals[][3] =
{
	{0., 0., 1.},
	{0., 0., 1.},
	{0., -1., 0.},
	{0., -1., 0.},
	{0., 1., 0.},
	{0., 1., 0.},
};


// X-shaped (all directions available to move in)
static GLuint XShapeIndices[][3] =
{
	{2, 6, 7},
	{2, 7, 3},
	{0, 1, 5},
	{0, 5, 4}
};


static GLfloat XShapeVertices[][3] =
{
	{-1., -1., -1.},
	{1., -1., -1.},
	{-1., 1., -1.},
	{1., 1., -1.},
	{-1., -1., 1.},
	{1., -1., 1.},
	{-1., 1., 1.},
	{1., 1., 1.}
};


static GLfloat XShapeNormals[][3] =
{
	{0., -1., 0.},
	{0., -1., 0.},
	{0., 1., 0.},
	{0., 1., 0.},
};