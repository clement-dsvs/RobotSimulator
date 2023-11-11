#pragma once

#include <raylib.h>
#include <cjson/cJSON.h>

enum ObstacleType
{
	E_OBSTACLE_UNDEFINED = -1,
	E_OBSTACLE_CUBE = 0,
	E_OBSTACLE_CYLINDER
};

class Obstacle
{
public:
	Obstacle();

	cJSON* toJSON();

	static Obstacle fromJSON(cJSON* a_jsonObstacle);

private:
	Mesh m_Mesh;
	Vector3 m_Position;
	ObstacleType m_ObstacleType;
};

