#pragma once

#include <raylib.h>
#include <cjson/cJSON.h>

class Obstacle
{
public:
	Obstacle();

	cJSON* toJSON();

private:
	Mesh m_Mesh;
	Vector3 m_Position;
};

