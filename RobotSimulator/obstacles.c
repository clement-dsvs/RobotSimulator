#include "obstacles.h"

#include <raymath.h>
#include <malloc.h>

void UpdateObstacle(Obstacle* obstacle, float delta)
{
	obstacle->selected = CheckCollisionPointRec(GetMousePosition(), (Rectangle){
		                                            obstacle->position.x, obstacle->position.y, obstacle->size.x,
		                                            obstacle->size.y
	                                            });

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && obstacle->selected)
	{
		const Vector2 movement = GetMouseDelta();
		obstacle->position = Vector2Add(obstacle->position, movement);
	}
}

void DrawObstacle(Obstacle* obstacle)
{
	DrawRectangleV(obstacle->position, obstacle->size, obstacle->color);

	if (obstacle->selected)
	{
		DrawRectangleLines(obstacle->position.x - 5, obstacle->position.y - 5, obstacle->size.x + 10,
		                   obstacle->size.y + 10, RED);
	}
}

bool ExpandList(Obstacle** list, int length)
{
	int new_length = length + 1;
	Obstacle* temp = realloc(*list, new_length * sizeof(Obstacle) );

	if (temp != NULL)
	{
		*list = temp;
		return true;
	}
	return false;
}

bool AddObstacle(ObstacleList* list, const Obstacle obstacle)
{
	if (! ExpandList(&(list->obstacles), list->size))
	{
		return false;
	}

	list->size += 1;
	list->obstacles[list->size - 1] = obstacle;
	return true;
}

bool RemoveObstacle(ObstacleList* list, const unsigned int index)
{
	Obstacle* old = list->obstacles;

	for (unsigned int i = index + 1; i < list->size; i++)
	{
		list->obstacles[i - 1] = list->obstacles[i];
	}

	if ((list->obstacles = realloc(list->obstacles, list->size-- * sizeof(Obstacle))) == NULL)
	{
		// realloc failed, restore previous state
		list->obstacles = old;
		return false;
	}

	return true;
}
