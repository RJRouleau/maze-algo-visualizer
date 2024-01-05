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

static GLfloat IShapeColors[][3] =
{
	{ 1., 1., 1. },
	{ 1., 1., 1. },
	{ 1., 1., 1. },
	{ 1., 1., 1. },
	{ 0.55, 0.35, 0.05 },
	{ 0.55, 0.35, 0.05 },
	{ 0.55, 0.35, 0.05 },
	{ 0.55, 0.35, 0.05 }
};

// dead end room
static GLuint DeadEndIndices[][3] =
{
	{0, 2, 3},
	{0, 3, 1},
	{1, 3, 7},
	{1, 7, 5},
	{0, 4, 6},
	{0, 6, 2},
	{2, 6, 7},
	{2, 7, 3},
	{0, 1, 5},
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

static GLfloat DeadEndColors[][3] =
{
	{ 1., 1., 1. },
	{ 1., 1., 1. },
	{ 1., 1., 1. },
	{ 1., 1., 1. },
	{ 0.55, 0.35, 0.05 },
	{ 0.55, 0.35, 0.05 },
	{ 0.55, 0.35, 0.05 },
	{ 0.55, 0.35, 0.05 }
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

static GLfloat LShapeColors[][3] =
{
	{ 1., 1., 1. },
	{ 1., 1., 1. },
	{ 1., 1., 1. },
	{ 1., 1., 1. },
	{ 0.55, 0.35, 0.05 },
	{ 0.55, 0.35, 0.05 },
	{ 0.55, 0.35, 0.05 },
	{ 0.55, 0.35, 0.05 }
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

static GLfloat TShapeColors[][3] =
{
	{ 1., 1., 1. },
	{ 1., 1., 1. },
	{ 1., 1., 1. },
	{ 1., 1., 1. },
	{ 0.55, 0.35, 0.05 },
	{ 0.55, 0.35, 0.05 },
	{ 0.55, 0.35, 0.05 },
	{ 0.55, 0.35, 0.05 }
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

static GLfloat XShapeColors[][3] =
{
	{ 1., 1., 1. },
	{ 1., 1., 1. },
	{ 1., 1., 1. },
	{ 1., 1., 1. },
	{ 0.55, 0.35, 0.05 },
	{ 0.55, 0.35, 0.05 },
	{ 0.55, 0.35, 0.05 },
	{ 0.55, 0.35, 0.05 }
};