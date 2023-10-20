#pragma once

#include "obstacles.h"
#include "robot.h"

#include <stdbool.h>
#include <raylib.h>

typedef struct Menu
{
	bool menuActive;
	bool dropDownEditMode;
	int selected_shape;
	bool* edit_mode;

	bool spinnerHeightEditMode;
	bool spinnerWidthEditMode;
	bool spinnerViewDistanceEditMode;
	bool spinnerStepEditMode;

	Vector2 anchor01;
	Vector2 anchor02;
	Vector2 anchor03;
	Vector2 anchor04;

	Obstacle* selected_obstacle;
	ObstacleList* environment;
	Robot* robot;
} Menu;

extern void DrawMenu(Menu* menu);