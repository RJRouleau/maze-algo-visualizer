#pragma once

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"

#include <vector>

#include "vertexbufferobject.h"
#include "glslprogram.h"
#include "menu.h"
extern const float D2R;
class Game
{
private:
	VertexBufferObject					mIShapeVB;
	VertexBufferObject					mDeadEndVB;
	VertexBufferObject					mLShapeVB;
	VertexBufferObject					mTShapeVB;
	VertexBufferObject					mXShapeVB;

	enum roomShape {
		I_SHAPE,
		T_SHAPE,
		L_SHAPE,
		X_SHAPE,
		DEAD_END,
		WALL
	};
	struct roomInfo {
		roomShape	shape;
		float		rotation;
		glm::vec3	position;
	};
	std::vector<std::vector<roomInfo>>	mMaze;

	glm::vec3							mEye;
	glm::vec3							mLook;
	glm::vec3							mUp;

	glm::vec3							mForward;
	glm::vec3							mRight;

	glm::mat4							mProjection;
	glm::mat4							mView;
	glm::mat4							mModel;
	glm::mat4							mRotation;

public:
	GLSLProgram							GridShader;
	GLSLProgram							RoomShader;

	Game();
	~Game();

	void Init();
	void Display();
	void Keyboard(unsigned char c, int x, int y);
	void SpecialKeys(int key, int x, int y);

	void setMaze(std::vector<std::vector<TileState>> layout);

private:
	void initRoomVB(int numFaces, roomShape shape, VertexBufferObject& vbo);
	void updateViewParameters();
	void drawMaze(std::vector<std::vector<roomInfo>> maze);

	std::vector<std::vector<roomInfo>> alignRoom(std::vector<std::vector<TileState>> layout);
	roomInfo createDeadEnd(std::vector<char> directionList);
	roomInfo createIShape(std::vector<char> directionList);
	roomInfo createLShape(std::vector<char> directionList);
	roomInfo createTShape(std::vector<char> directionList);
	roomInfo createXShape();
	roomInfo createWall();

	std::vector<char> getAvailableMoves(std::vector<std::vector<TileState>> layout, int row, int col);

};
