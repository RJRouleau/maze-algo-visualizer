#pragma once

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"

#include <vector>

#include "vertexbufferobject.h"
#include "glslprogram.h"
extern const float D2R;
// A 2D square
static GLuint SquareIndices[][3] =
{
	{0, 1, 2},
	{3, 0, 2}
};

static GLfloat SquareVertices[][3] =
{
	{-1., -1., 0.},
	{1., -1., 0.},
	{1., 1., 0.},
	{-1., 1., 0.}
};

static GLfloat SquareColors[][3] =
{
	{0., 0., 0.},
	{0., 0., 0.},
	{0., 0., 0.},
	{0., 0., 0.}
};

enum TileState {
	WALL,
	INCLUDED,
	START,
	END
};

struct Tile {
	GLfloat x, y;
	TileState state;
};

struct Grid {
	int r;
	int	c;
	std::vector<std::vector<Tile>> tiles;
};

const float TILE_WIDTH = 2.0;

enum Moves {
	MOVE_UP,
	MOVE_DOWN,
	MOVE_RIGHT,
	MOVE_LEFT
};

class Menu
{
private:
	VertexBufferObject					mSquareVB;
	Grid								mGrid;
	std::vector<std::vector<TileState>> mLayout;
	Tile								mSelectedTile;
	bool								mStartTileSet;
	bool								mEndTileSet;

	glm::vec3							mEye;
	glm::vec3							mLook;
	glm::vec3							mUp;

	glm::mat4							mProjection;
	glm::mat4							mView;
	glm::mat4							mModel;
	
public:
	GLSLProgram							GridShader;

	Menu();
	~Menu();

	void Init();
	void Display(float time);
	void Keyboard(unsigned char c, int x, int y);
	void SpecialKeys(int key, int x, int y);

	std::vector<std::vector<TileState>> getLayout();

private:
	void drawTile(glm::vec3 color, int selected, float time, float posX, float posY);

	Grid createGrid(int r, int c);

	void toggleTileLayout();
	void moveSelectedTile(int direction);

};
