#pragma once
#include "raylib.h"
#include "Obstacle.h"

struct C_RayCollision : RayCollision
{
    Obstacle* m_obstacleCollided;
};

