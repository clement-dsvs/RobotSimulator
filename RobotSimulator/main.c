#include <raylib.h>

#include <raymath.h>
#include <stdlib.h>

#include <raygui.h>

#include "menu.h"
#include "utils.h"
#include "obstacles.h"
#include "robot.h"

int main(void)
{
	const int screenWidth = 1600;
	const int screenHeight = 900;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	InitWindow(screenWidth, screenHeight, "Robot Simulator");
	SetExitKey(KEY_NULL);

	if (!IsWindowState(FLAG_VSYNC_HINT)) SetTargetFPS(60);

	Camera3D camera;
    camera.position = (Vector3){ .0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Vector3 cubePosition = { 0.0f, 1.0f, 0.0f };
	RenderTexture screen = LoadRenderTexture(screenWidth - 215, screenHeight - 10);
	Rectangle screenRect = {0, 0, screen.texture.width, -screen.texture.height};

	ObstacleList list_obstacles = {0, 0};
	Robot robot = {(Vector2){600, 300}, 15, 180, GREEN, &list_obstacles, true, 100};
	robot.environment = &list_obstacles;
	//AddObstacle(&list_obstacles, (Obstacle){(Vector2){0, 0}, (Vector2){50, 50}, false, PURPLE});
	//AddObstacle(&list_obstacles, (Obstacle){(Vector2){screenRect.width-50, screenRect.height-50}, (Vector2){50, 50}, false, BLUE});

	Menu menu = {
		true, false, 0, &list_obstacles.edit_mode, false, false, false, false, (Vector2){5, 5}, (Vector2){10, 35},
		(Vector2){10, 210}, (Vector2){10, 410}, &list_obstacles.obstacles[1], &list_obstacles, &robot
	};

	bool afficher_les_rayons = true;
	bool lock_obstacles = false;
	bool edit = false;
	int active = 0;

	while (!WindowShouldClose())
	{
		// Update
		const float delta = GetFrameTime();
		if (list_obstacles.edit_mode)
		{
			list_obstacles.selected_index = -1;
			for (int i = 0; i < list_obstacles.size; i++)
			{
				UpdateObstacle(&(list_obstacles.obstacles[i]), delta);
				if (list_obstacles.obstacles[i].selected)
				{
					list_obstacles.selected_index = i;
					break;
				}
			}
		}

		//UpdateCamera(&camera, CAMERA_THIRD_PERSON);

		//UpdateRobot(&robot, delta);

		BeginTextureMode(screen);
		{
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);
			{

				DrawLine3D((Vector3){-2, 0, -2}, (Vector3){1, 0, 1}, GREEN);
				DrawCube(cubePosition, 2.f, 2.f, 2.f, RED);
				DrawGrid(10, .5f);
			}
			EndMode3D();
		}
		EndTextureMode();

		// Draw
		BeginDrawing();
		{
			ClearBackground(BLACK);

			DrawMenu(&menu);
			

			DrawTextureRec(screen.texture, screenRect, (Vector2){210, 5}, WHITE);
		}
		EndDrawing();
	}

	// De-Initialization

	UnloadRenderTexture(screen);

	CloseWindow();
	free(list_obstacles.obstacles);

	return 0;
}
