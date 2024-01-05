#include "menu.h"

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

#ifndef KEY_CONSTANTS
#define KEY_CONSTANTS
const int ESCAPE = 0x1b;
const int ENTER = 0xd;
const int SPACE = 0x20;
#endif


Menu::Menu()
{
	mProjection = glm::perspective(D2R * 90., 1., 0.1, 1000.);

	mEye = glm::vec3(0., 0., 25.);
	mLook = glm::vec3(0., 0., 0.);
	mUp = glm::vec3(0., 1., 0.);
	mView = glm::lookAt(mEye, mLook, mUp);

	glm::mat4 model(1.);

	mLayout = std::vector<std::vector<TileState>>(5, std::vector<TileState>(5, TileState::WALL));
	mGrid = createGrid(5, 5);
	mSelectedTile.x = 4 * TILE_WIDTH;
	mSelectedTile.y = 4 * TILE_WIDTH;

	mStartTileSet = false;
	mEndTileSet =	false;
}

Menu::~Menu()
{
}

void Menu::Init()
{
	GridShader.Init();
	bool valid = GridShader.Create("grid.vert", "grid.frag");
	if (!valid)
		fprintf(stderr, "Could not create the GridShader!\n");
	else
		fprintf(stderr, "GridShader created!\n");

	mSquareVB.Init();
	mSquareVB.glBegin(GL_TRIANGLES);				
	for (int i = 0; i < 2; i++) {		
		for (int j = 0; j < 3; j++) {	
			int k = SquareIndices[i][j];
			mSquareVB.glColor3fv(SquareColors[k]);
			mSquareVB.glVertex3fv(SquareVertices[k]);
		}
	}
	mSquareVB.glEnd();
}

void Menu::Display(float time)
{
	glClearColor(0.5, 0.5, 0.5, 1.);

	for (int i = 0; i < mGrid.r; i++) {
		for (int j = 0; j < mGrid.c; j++) {
			glm::vec3 tileColor(0.2, 0.2, 0.2);
			switch (mGrid.tiles[i][j].state) {
			case TileState::WALL:
				break;

			case TileState::INCLUDED:
				tileColor = glm::vec3(1., 1., 1.);
				break;

			case TileState::START:
				tileColor = glm::vec3(0., 1., 0.);
				break;

			case TileState::END:
				tileColor = glm::vec3(1., 0., 0.);
				break;
			}

			if (mSelectedTile.x == mGrid.tiles[i][j].x && mSelectedTile.y == mGrid.tiles[i][j].y)
				drawTile(tileColor, 1, time, mGrid.tiles[i][j].x, mGrid.tiles[i][j].y);
			else
				drawTile(tileColor, 0, time, mGrid.tiles[i][j].x, mGrid.tiles[i][j].y);
		}
	}	
}

void Menu::Keyboard(unsigned char c, int x, int y)
{
	switch (c)
	{
	case SPACE:
		toggleTileLayout();
		break;

	case ENTER:
		break;

	default:
		break;
	}
}

void Menu::SpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		moveSelectedTile(Moves::MOVE_UP);
		break;

	case GLUT_KEY_DOWN:
		moveSelectedTile(Moves::MOVE_DOWN);
		break;

	case GLUT_KEY_RIGHT:
		moveSelectedTile(Moves::MOVE_RIGHT);
		break;

	case GLUT_KEY_LEFT:
		moveSelectedTile(Moves::MOVE_LEFT);
		break;
	}
}

std::vector<std::vector<TileState>> Menu::getLayout()
{
	return mLayout;
}

void Menu::drawTile(glm::vec3 color, int selected, float time, float posX, float posY)
{
	GridShader.Use();

	GridShader.SetUniformVariable("projectionMatrix", mProjection);
	GridShader.SetUniformVariable("viewMatrix", mView);

	mModel = glm::translate(mModel, glm::vec3(posX, posY, 0.));
	GridShader.SetUniformVariable("modelMatrix", mModel);
	mModel = glm::mat4(1.);

	GridShader.SetUniformVariable("uColor", color);
	GridShader.SetUniformVariable("uPosX", posX);
	GridShader.SetUniformVariable("uPosY", posY);

	GridShader.SetUniformVariable("uSelected", selected);
	GridShader.SetUniformVariable("uTime", time);

	mSquareVB.Draw();

	GridShader.UnUse();
}

// Create and initialize the grid that holds the position and state of each tile.
// The state of the tile is set by indexing into mLayout.
Grid Menu::createGrid(int r, int c) {
	Grid grid;
	grid.r = r;
	grid.c = c;
	grid.tiles.resize(r, std::vector<Tile>(c));
	
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			grid.tiles[i][j].x = static_cast<float>(j * 2);
			grid.tiles[i][j].y = static_cast<float>(i * 2);
			grid.tiles[i][j].state = mLayout[i][j];
		}
	}
	return grid;
}

// Change state of tile in layout
void Menu::toggleTileLayout() {
	int row = static_cast<int>(mSelectedTile.y / TILE_WIDTH);
	int col = static_cast<int>(mSelectedTile.x / TILE_WIDTH);

	TileState& layoutState = mLayout[row][col];
	TileState& gridState = mGrid.tiles[row][col].state;

	switch (layoutState) {
	case TileState::WALL:
		layoutState = TileState::INCLUDED;
		gridState = TileState::INCLUDED;
		break;

	case TileState::INCLUDED:
		if (!mStartTileSet) {
			layoutState = TileState::START;
			gridState = TileState::START;
			mStartTileSet = true;
		}
		else if (!mEndTileSet) {
			layoutState = TileState::END;
			gridState = TileState::END;
			mEndTileSet = true;
		}
		else {
			layoutState = TileState::WALL;
			gridState = TileState::WALL;
		}
		break;

	case TileState::START:
		if (!mEndTileSet) {
			layoutState = TileState::END;
			gridState = TileState::END;
			mEndTileSet = true;
		}
		else {
			layoutState = TileState::WALL;
			gridState = TileState::WALL;
		}
		mStartTileSet = false;
		break;

	case TileState::END:
		layoutState = TileState::WALL;
		gridState = TileState::WALL;
		mEndTileSet = false;
		break;
	}
}

// Move the position of the currently selected tile
void Menu::moveSelectedTile(int direction) {
	float bound_right = 4 * TILE_WIDTH, bound_left = 0 * TILE_WIDTH;
	float bound_up = 4 * TILE_WIDTH, bound_down = 0 * TILE_WIDTH;
	switch (direction) {
	case Moves::MOVE_UP:
		if (mSelectedTile.y + TILE_WIDTH <= bound_up)
			mSelectedTile.y += TILE_WIDTH;
		break;

	case Moves::MOVE_DOWN:
		if (mSelectedTile.y - TILE_WIDTH >= bound_down)
			mSelectedTile.y -= TILE_WIDTH;
		break;

	case Moves::MOVE_RIGHT:
		if (mSelectedTile.x + TILE_WIDTH <= bound_right)
			mSelectedTile.x += TILE_WIDTH;
		break;

	case Moves::MOVE_LEFT:
		if (mSelectedTile.x - TILE_WIDTH >= bound_left)
			mSelectedTile.x -= TILE_WIDTH;
		break;
	}
}