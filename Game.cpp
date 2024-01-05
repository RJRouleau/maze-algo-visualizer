#include "Game.h"

#include "objects.h"

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <algorithm>



Game::Game()
{
	mEye = glm::vec3(0., 5., -5.);
	mLook = glm::vec3(0., 0., 0.);

	updateViewParameters();
}

Game::~Game()
{
}

void Game::Init()
{
	RoomShader.Init();
	bool valid = RoomShader.Create("room.vert", "room.frag");
	if (!valid)
		fprintf(stderr, "Could not create the RoomShader!\n");
	else
		fprintf(stderr, "RoomShader created!\n");

	mIShapeVB.Init();
	mIShapeVB.glBegin(GL_TRIANGLES);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			int k = IShapeIndices[i][j];
			mIShapeVB.glColor3fv(IShapeColors[k]);
			mIShapeVB.glVertex3fv(IShapeVertices[k]);
		}
	}

	mDeadEndVB.Init();
	mDeadEndVB.glBegin(GL_TRIANGLES);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 3; j++) {
			int k = DeadEndIndices[i][j];
			mDeadEndVB.glColor3fv(DeadEndColors[k]);
			mDeadEndVB.glVertex3fv(DeadEndVertices[k]);
		}
	}

	mLShapeVB.Init();
	mLShapeVB.glBegin(GL_TRIANGLES);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			int k = LShapeIndices[i][j];
			mLShapeVB.glColor3fv(LShapeColors[k]);
			mLShapeVB.glVertex3fv(LShapeVertices[k]);
		}
	}

	mTShapeVB.Init();
	mTShapeVB.glBegin(GL_TRIANGLES);
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 3; j++) {
			int k = TShapeIndices[i][j];
			mTShapeVB.glColor3fv(TShapeColors[k]);
			mTShapeVB.glVertex3fv(TShapeVertices[k]);
		}
	}

	mXShapeVB.Init();
	mXShapeVB.glBegin(GL_TRIANGLES);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			int k = XShapeIndices[i][j];
			mXShapeVB.glColor3fv(XShapeColors[k]);
			mXShapeVB.glVertex3fv(XShapeVertices[k]);
		}
	}
}

void Game::Display()
{
	glClearColor(0., 0., 0., 0.);

	updateViewParameters();

	drawMaze(mMaze);
}

void Game::Keyboard(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'd':
	case 'D':
		mEye += mRight;
		mLook += mRight;
		break;

	case 'a':
	case 'A':
		mEye -= mRight;
		mLook -= mRight;
		break;

	case 'e':
	case 'E':
		// move camera up
		mEye += mUp;
		mLook += mUp;
		break;

	case 'q':
	case 'Q':
		// move camera down
		mEye -= mUp;
		mLook -= mUp;
		break;

	default:
		break;
	}
}

void Game::SpecialKeys(int key, int x, int y) {
	glm::mat4 rotationMatrix;

	switch (key) {
	case GLUT_KEY_UP:
		mEye += mForward;
		mLook += mForward;
		break;

	case GLUT_KEY_DOWN:
		mEye -= mForward;
		mLook -= mForward;
		break;

	case GLUT_KEY_RIGHT:
		rotationMatrix = glm::rotate(glm::mat4(1.0f), D2R * -10.f, mUp);
		break;

	case GLUT_KEY_LEFT:
		rotationMatrix = glm::rotate(glm::mat4(1.0f), D2R * 10.f, mUp);
		break;
	}

	glm::vec3 lookDirection = mLook - mEye;
	lookDirection = glm::vec3(rotationMatrix * glm::vec4(lookDirection, 0.0));
	mLook = mEye + lookDirection;
}

void Game::drawMaze(std::vector<std::vector<roomInfo>> maze)
{
	RoomShader.Use();
	RoomShader.SetUniformVariable("projectionMatrix", mProjection);
	RoomShader.SetUniformVariable("viewMatrix", mView);
	// The model matrix is rotated before drawing each room in the maze.
	for (int i = 0; i < maze.size(); i++) {
		for (int j = 0; j < maze[i].size(); j++) {
			mModel = glm::translate(mModel, maze[i][j].position);
			mModel = glm::rotate(mModel, maze[i][j].rotation, glm::vec3(0., 1., 0.));			
			RoomShader.SetUniformVariable("modelMatrix", mModel);
			switch (maze[i][j].shape) {
			case roomShape::I_SHAPE:
				mIShapeVB.Draw();
				break;

			case roomShape::T_SHAPE:
				mTShapeVB.Draw();
				break;

			case roomShape::L_SHAPE:
				mLShapeVB.Draw();
				break;

			case roomShape::X_SHAPE:
				mXShapeVB.Draw();
				break;

			case roomShape::DEAD_END:
				mDeadEndVB.Draw();
				break;

			default:
				break;
			}
			// Setting the model matrix back to an identity matrix is required
			// before applying the rotation for the next room.
			mModel = glm::mat4(1.);
		}
	}
	RoomShader.UnUse();
}

void Game::setMaze(std::vector<std::vector<TileState>> layout)
{
	mMaze = alignRoom(layout);
}

void Game::updateViewParameters()
{
	mUp = glm::vec3(0., 1., 0.);
	mForward = glm::normalize(mLook - mEye);
	// right is perpendicular to both the forward and up vector.
	// cross product finds a vector perpendicular to any two vectors.
	mRight = glm::cross(mForward, mUp);

	mProjection = glm::perspective(D2R * 90., 1., 0.1, 1000.);
	mView = glm::lookAt(mEye, mLook, mUp);
	mModel = glm::mat4(1.);
}

// This function processes the layout created at the menu screen. Each room's shape and rotation is determined
// by checking which adjacent rooms in the layout aren't walls. The function returns a 2D vector of roomInfo structs
// which contain the shape and rotation of each room.
std::vector<std::vector<Game::roomInfo>> Game::alignRoom(std::vector<std::vector<TileState>> layout)
{
	std::vector<std::vector<Game::roomInfo>> result;
	for (int i = 0; i < layout.size(); i++) {
		std::vector<Game::roomInfo> rows;
		for (int j = 0; j < layout[i].size(); j++) {
			std::vector<char> directionList = getAvailableMoves(layout, i, j);			
			roomInfo newRoom;
			if (layout[i][j] != TileState::WALL) {
				std::sort(directionList.begin(), directionList.end());
				// The number of moves available indicates the shape of the room
				// and which directions those are determines the rotation.
				switch (directionList.size()) {
				case 1:
					newRoom = createDeadEnd(directionList);
					break;
				// Case 2 is an exception where both L-shape and I-shape have 2 available move directions.
				case 2:
					switch (directionList[0]) {
					case 'd':
						if (directionList[1] == 'u')
							newRoom = createIShape(directionList);
						else
							newRoom = createLShape(directionList);
						break;

					case 'l':
						if (directionList[1] == 'r')
							newRoom = createIShape(directionList);
						else
							newRoom = createLShape(directionList);
						break;

					case 'r':
						newRoom = createLShape(directionList);
						break;
					}
					break;

				case 3:
					newRoom = createTShape(directionList);
					break;

				default:
					newRoom = createXShape();
					break;
				}
			}
			else {
				newRoom = createWall();
			}			
			newRoom.position = glm::vec3(static_cast<float>(2.0 * i), 0., static_cast<float>(2.0 * j));
			rows.push_back(newRoom);
		}
		result.push_back(rows);
	}
	return result;
}

Game::roomInfo Game::createDeadEnd(std::vector<char> directionList)
{
	roomInfo newRoom;
	newRoom.shape = roomShape::DEAD_END;
	// determine rotation of room based on which direction is available
	switch (directionList[0]) {
	case 'd':
		newRoom.rotation = D2R * 270.;
		break;

	case 'l':
		newRoom.rotation = D2R * 180.;
		break;

	case 'r':
		newRoom.rotation = 0;
		break;

	case 'u':
		newRoom.rotation = D2R * 90.;
		break;
	}
	return newRoom;
}

Game::roomInfo Game::createIShape(std::vector<char> directionList)
{
	roomInfo newRoom;
	newRoom.shape = roomShape::I_SHAPE;
	switch (directionList[1]) {
	case 'r':
		// l-r I shaped
		newRoom.rotation = 0.;
		break;

	case 'u':
		// up and down I shaped
		newRoom.rotation = D2R * 90.;
		break;
	}
	return newRoom;
}

Game::roomInfo Game::createLShape(std::vector<char> directionList)
{
	roomInfo newRoom;
	newRoom.shape = roomShape::L_SHAPE;
	if (directionList[0] == 'r')
		newRoom.rotation = 0.;
	else {
		switch (directionList[1]) {
		case 'l':
			// d-l L-shaped
			newRoom.rotation = D2R * 180.;
			break;

		case 'r':
			// d-r L-shaped
			newRoom.rotation = D2R * 270.;
			break;

		case 'u':
			// l-u L-shaped
			newRoom.rotation = D2R * 90.;
			break;
		}
	}	
	return newRoom;
}

Game::roomInfo Game::createTShape(std::vector<char> directionList)
{
	roomInfo newRoom;
	newRoom.shape = roomShape::T_SHAPE;
	switch (directionList[0]) {
	case 'd':
		if (directionList[2] == 'r')
			// d-l-r no rotation
			newRoom.rotation = D2R * 270;
		else if (directionList[1] == 'l')
			// d-l-u
			newRoom.rotation = D2R * 180.;
		else
			// d-r-u
			newRoom.rotation = D2R * 0.;
		break;

	default:
		// l-r-u
		newRoom.rotation = D2R * 90.;
		break;
	}
	return newRoom;
}

Game::roomInfo Game::createXShape()
{
	roomInfo newRoom;
	newRoom.shape = X_SHAPE;
	newRoom.rotation = 0;
	return newRoom;
}

Game::roomInfo Game::createWall()
{
	roomInfo newRoom;
	newRoom.shape = WALL;
	newRoom.rotation = 0.;
	return newRoom;
}

std::vector<char> Game::getAvailableMoves(std::vector<std::vector<TileState>> layout, int row, int col)
{
	std::vector<char> directionList;
	// Check if up is available
	if (row != layout.size() - 1) {
		if (layout[row + 1][col] != TileState::WALL) {
			directionList.push_back('u');
		}
	}
	// Check if left is available
	if (col != 0) {
		if (layout[row][col - 1] != TileState::WALL) {
			directionList.push_back('l');
		}
	}
	// Check if right is available
	if (col != layout[row].size() - 1) {
		if (layout[row][col + 1] != TileState::WALL) {
			directionList.push_back('r');
		}
	}
	// Check if down is available
	if (row != 0) {
		if (layout[row - 1][col] != TileState::WALL) {
			directionList.push_back('d');
		}
	}
	return directionList;
}
