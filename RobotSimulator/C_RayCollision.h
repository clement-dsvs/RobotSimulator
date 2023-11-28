#pragma once
#include "D:\tools\vcpkg\installed\x64-windows\include\raylib.h"
#include "Obstacle.h"

struct C_RayCollision : RayCollision
{
    Obstacle* m_obstacleCollided;
};

