#pragma once

#include "obstacles.h"

#include <raylib.h>
#include <stdbool.h>

float clamp(float min, float max, float value);

bool CheckCollisionRayObstacle(Vector2 origin, Vector2 ray, Obstacle rect, Vector2* result);
