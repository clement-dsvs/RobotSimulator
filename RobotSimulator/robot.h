#pragma once

#include "obstacles.h"

#include <raylib.h>

typedef struct Robot
{
	Vector2 position;
	float size;
	float orientation;
	Color color;
	ObstacleList* environment;
	bool draw_rays;
	int view_distance;
	int angle_step;
	int angle_max;
} Robot;

void UpdateRobot(Robot* robot, float delta);

void DrawRobot(Robot* robot);
