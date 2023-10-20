#pragma once

#include <raylib.h>

typedef struct
{
	Vector2 position;
	Vector2 size;
	bool selected;
	Color color;
} Obstacle;

typedef struct
{
	unsigned int size;
	Obstacle* obstacles;
	int selected_index;
	bool edit_mode;
} ObstacleList;

extern void UpdateObstacle(Obstacle* obstacle, float delta);

extern void DrawObstacle(Obstacle* obstacle);

extern bool AddObstacle(ObstacleList* list, const Obstacle obstacle);

extern bool RemoveObstacle(ObstacleList* list, const unsigned int index);
