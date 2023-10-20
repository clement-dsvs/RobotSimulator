#include "robot.h"

#include "utils.h"

#include <raylib.h>
#include <raymath.h>

#define ANGLE_MAX 270
#define ANGLE_STEP 5

void DrawRobot(Robot* robot)
{
	if (robot->draw_rays)
	{
		for (int angle = 0; angle <= ANGLE_MAX; angle += ANGLE_STEP)
		{
			int temp_angle = angle - ANGLE_MAX / 2 + robot->orientation;
			float theta = (float)temp_angle * PI / 180;
			Vector2 ray = (Vector2){cosf(theta), sinf(theta)};
			ray = Vector2Scale(ray, robot->view_distance);
			Vector2 target = Vector2Add(robot->position, ray);
			Vector2 intersection;
			Vector2 temp_intersection;
			bool collision = false;
			float min_distance = HUGE_VALF;
			for (int i = 0; i < robot->environment->size; i++)
			{
				if (CheckCollisionRayObstacle(robot->position, target, robot->environment->obstacles[i],
				                              &temp_intersection))
				{
					collision = true;
					float temp_distance = Vector2Distance(robot->position, temp_intersection);
					if (temp_distance < min_distance)
					{
						min_distance = temp_distance;
						intersection = temp_intersection;
					}
				}
			}
			if (collision)
			{
				DrawLineEx(robot->position, intersection, 2, RED);
			}
			else
			{
				DrawLineEx(robot->position, target, 2, GRAY);
			}
		}
	}

	DrawCircleV(robot->position, robot->size, robot->color);
	float test = robot->orientation * PI / 180;
	Vector2 indicateur = {cosf(test), sinf(test)};
	indicateur = Vector2Scale(indicateur, robot->size); 
	indicateur = Vector2Add(robot->position, indicateur);
	DrawLineEx(robot->position, indicateur, 2, BLUE);

}
