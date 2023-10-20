#include "utils.h"

#include "obstacles.h"

#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>

float clamp(float min, float max, float value)
{
	const float t = value < min ? min : value;
	return t > max ? max : t;
}

int GetNearestPointIndex(Vector2 origin, Vector2* points, int nbPoints)
{
	float min_distance = HUGE_VALF;
	int min_index = -1;
	for (int i = 0; i < nbPoints; i++)
	{
		float distance = Vector2Distance(origin, points[i]);
		if (distance < min_distance)
		{
			min_distance = distance;
			min_index = i;
		}
	}
	return min_index;
}

bool CheckCollisionRayObstacle(Vector2 origin, Vector2 ray, Obstacle obstacle, Vector2* result)
{
	Vector2 end = (Vector2){ray.x, ray.y};
	Vector2* corners = malloc(sizeof(Vector2) * 4);
	Rectangle rect = {obstacle.position.x, obstacle.position.y, obstacle.size.x, obstacle.size.y};

	corners[0] = (Vector2){rect.x, rect.y};
	corners[1] = (Vector2){rect.x + rect.width, rect.y};
	corners[2] = (Vector2){rect.x + rect.width, rect.y + rect.height};
	corners[3] = (Vector2){rect.x, rect.y + rect.height};

	Vector2 collision_point;

	// chercher le coin le plus proche de l'origine
	int nearest_point = GetNearestPointIndex(origin, corners, 4);
	int next_corner = nearest_point + 1;
	if (next_corner > 3) next_corner = 0;

	Vector2* collisions = malloc(sizeof(Vector2) * 4);
	int nbCollisions = 0;


	if (CheckCollisionLines(origin, end, corners[nearest_point], corners[next_corner], &collision_point))
	{
		collisions[nbCollisions] = collision_point;
		nbCollisions++;
	}

	next_corner = nearest_point - 1;
	if (next_corner < 0) next_corner = 3;
	if (CheckCollisionLines(origin, end, corners[nearest_point], corners[next_corner], &collision_point))
	{
		collisions[nbCollisions] = collision_point;
		nbCollisions++;
	}

	int nearestCollision = GetNearestPointIndex(origin, collisions, nbCollisions);
	*result = collisions[nearestCollision];

	return nbCollisions > 0;
}
