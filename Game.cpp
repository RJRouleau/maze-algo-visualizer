#include "Game.h"

#include "objects.h"

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <algorithm>

#include "keytime.cpp"

Game::Game()
{
	glm::vec3 bottom_left = glm::vec3(0., 0., 0.);
	glm::vec3 top_left = glm::vec3(8., 0., 0.);
	glm::vec3 top_right = glm::vec3(8., 0., 8.);
	float roomDistance = 2.0;

	mEye = glm::vec3(0., 4., -1.);
	mLook = glm::vec3(0., 0., 1.);

	updateViewParameters();
	mTestSolution.push(glm::vec3(0., 0., 0.));
	mTestSolution.push(glm::vec3(0., 0., 2.));
	mTestSolution.push(glm::vec3(0., 0., 4.));
	mTestSolution.push(glm::vec3(0., 0., 6.));
	mTestSolution.push(glm::vec3(0., 0., 8.));
	mTestSolution.push(glm::vec3(2., 0., 8.));
	mTestSolution.push(glm::vec3(4., 0., 8.));
	mTestSolution.push(glm::vec3(6., 0., 8.));
	mTestSolution.push(glm::vec3(8., 0., 8.));
	mTestSolution.push(glm::vec3(8., 0., 6.));
	mTestSolution.push(glm::vec3(8., 0., 4.));
	mTestSolution.push(glm::vec3(8., 0., 2.));
	mTestSolution.push(glm::vec3(8., 0., 0.));
	mTestSolution.push(glm::vec3(6., 0., 0.));
	mTestSolution.push(glm::vec3(4., 0., 0.));
	mTestSolution.push(glm::vec3(2., 0., 0.));
	mTestSolution.push(glm::vec3(0., 0., 0.));
	setSolutionKeytimes(mTestSolution);
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

	initRoomVB(3, roomShape::I_SHAPE, mIShapeVB);
	initRoomVB(4, roomShape::DEAD_END, mDeadEndVB);
	initRoomVB(3, roomShape::L_SHAPE, mLShapeVB);
	initRoomVB(2, roomShape::T_SHAPE, mTShapeVB);
	initRoomVB(1, roomShape::X_SHAPE, mXShapeVB);
}

void Game::Display(float time)
{
	glClearColor(0., 0., 0., 0.);
	
	moveViewPosition(time);
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
		rotateView(D2R * -10.f);
		break;

	case GLUT_KEY_LEFT:
		rotateView(D2R * 10.f);
		break;
	}
}

void Game::drawMaze(std::vector<std::vector<roomInfo>> maze)
{
	RoomShader.Use();
	RoomShader.SetUniformVariable("projectionMatrix", mProjection);
	RoomShader.SetUniformVariable("viewMatrix", mView);
	// The model matrix is rotated before drawing each room in the maze.
	for (int i = 0; i < maze.size(); i++) {
		for (int j = 0; j < maze[i].size(); j++) {
			RoomShader.Use();
			mModel = glm::translate(mModel, maze[i][j].position);
			mModel = glm::rotate(mModel, maze[i][j].rotation, glm::vec3(0., 1., 0.));
			RoomShader.SetUniformVariable("modelMatrix", mModel);
			RoomShader.SetUniformVariable("uColor", glm::vec3(1.0, 1., 1.));
			if (maze[i][j].start == true) {
				RoomShader.SetUniformVariable("uColor", glm::vec3(0.0, 1., 0.));
			}
			if (maze[i][j].end == true) {
				RoomShader.SetUniformVariable("uColor", glm::vec3(1., 0., 0.));
			}
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
	mEye = glm::vec3(mStartPosition.x, 5., mStartPosition.z -1.);
	mLook = glm::vec3(mStartPosition.x, mStartPosition.y, mStartPosition.z + 1.);
	updateViewParameters();
}

void Game::setSolutionKeytimes(std::queue<glm::vec3> keytimePositions)
{
	mSolutionKeytimesX.Init();
	mSolutionKeytimesZ.Init();
	float timeIncrement = 30.f / (keytimePositions.size() - 1);
	float currentTimeValue = 0.f;	
	while (!keytimePositions.empty()) {
		glm::vec3 position = keytimePositions.front();
		mSolutionKeytimesX.AddTimeValue(currentTimeValue, position.x);
		mSolutionKeytimesZ.AddTimeValue(currentTimeValue, position.z);
		keytimePositions.pop();
		currentTimeValue += timeIncrement;
	}
}

void Game::moveViewPosition(float time)
{
	mEye = glm::vec3(mSolutionKeytimesX.GetValue(time), 5., mSolutionKeytimesZ.GetValue(time) - 1.);
	mLook = glm::vec3(mSolutionKeytimesX.GetValue(time), 0., mSolutionKeytimesZ.GetValue(time) + 1.);
	updateViewParameters();
}


void Game::rotateView(float angle)
{
	glm::mat4 rotationMatrix = glm::mat4(1.0);
	rotationMatrix = glm::rotate(rotationMatrix, angle, mUp);
	
	glm::vec3 lookDirection = mLook - mEye;
	lookDirection = glm::vec3(rotationMatrix * glm::vec4(lookDirection, 0.0));
	mLook = mEye + lookDirection;
}

void Game::initRoomVB(int numFaces, roomShape shape, VertexBufferObject& vbo)
{
	int k = 0;
	vbo.Init();
	vbo.glBegin(GL_TRIANGLES);
	for (int i = 0; i < numFaces * 2; i++) {
		for (int j = 0; j < 3; j++) {
			switch (shape) {
			case I_SHAPE:
				k = IShapeIndices[i][j];
				vbo.glNormal3fv(IShapeNormals[i]);
				vbo.glVertex3fv(IShapeVertices[k]);
				break;

			case DEAD_END:
				k = DeadEndIndices[i][j];
				vbo.glNormal3fv(DeadEndNormals[i]);
				vbo.glVertex3fv(DeadEndVertices[k]);
				break;

			case L_SHAPE:
				k = LShapeIndices[i][j];
				vbo.glNormal3fv(LShapeNormals[i]);
				vbo.glVertex3fv(LShapeVertices[k]);
				break;

			case T_SHAPE:
				k = TShapeIndices[i][j];
				vbo.glNormal3fv(TShapeNormals[i]);
				vbo.glVertex3fv(TShapeVertices[k]);
				break;

			case X_SHAPE:
				k = XShapeIndices[i][j];
				vbo.glNormal3fv(XShapeNormals[i]);
				vbo.glVertex3fv(XShapeVertices[k]);
				break;
			}
		}
	}
	vbo.glEnd();
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
			newRoom.start = false;
			newRoom.end = false;
			if (layout[i][j] == TileState::START) {
				std::cout << "Setting start\n";
				newRoom.start = true;
				mStartPosition = newRoom.position;
			}
			if (layout[i][j] == TileState::END) {
				std::cout << "Setting end\n";
				newRoom.end = true;
				mEndPosition = newRoom.position;
			}
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
