#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "rlImGui.h"

#include "dialogs.h"

#include <iostream>
#include <string>

#include "IHM.h"
#include "Map.h"

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	constexpr int screenWidth = 1280;
	constexpr int screenHeight = 800;

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Simulateur Robot");
	SetExitKey(KEY_NULL);
	SetTargetFPS(144);
	rlImGuiSetup(true);

	Camera3D camera;
    camera.position = Vector3{ 0.0f, 10.0f, 10.0f };	// Camera position
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };		// Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };			// Camera up vector (rotation towards target)
    camera.fovy = 45.0f;										// Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;						// Camera mode type

	Map map(&camera);
	map.setSize(Vector2{ 10, 10 });
	IHM l_ihm(&map, &camera);

	// Main loop
	while (!WindowShouldClose())
	{
		// Update Camera
		if (camera.projection == CAMERA_PERSPECTIVE)
		{
			if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
			{
				UpdateCamera(&camera, CAMERA_THIRD_PERSON);
			}
			else
			{
				const float mouseWheelMovement = GetMouseWheelMove();
				Vector3 mouse = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
				camera.position = Vector3Add(camera.position, Vector3Scale(mouse, mouseWheelMovement));
			}
		}

		map.o_update();

		// Draw
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);

			BeginMode3D(camera);
			{
				DrawGrid(map.getSize().x, 1.f);
				DrawGrid(map.getSize().y, 1.f);

				map.o_draw();
			}
			EndMode3D();

			rlImGuiBegin();
			{
				l_ihm.o_draw();
			}
			rlImGuiEnd();
		}
		EndDrawing();
		//----------------------------------------------------------------------------------
	}
	
	// De-Initialization
	//--------------------------------------------------------------------------------------
	rlImGuiShutdown();	// Shut down ImGui
	CloseWindow();		// Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
