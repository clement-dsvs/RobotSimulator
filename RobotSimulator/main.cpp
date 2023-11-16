#include "raylib.h"
#include "raymath.h"

#include <rlImGui.h>
#include <imgui.h>

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
	//SetTraceLogLevel(LOG_WARNING);

	Camera3D l_camera;
    l_camera.position = Vector3{ 0.0f, 10.0f, 10.0f };		// Camera position
    l_camera.target = Vector3{ 0.0f, 0.0f, 0.0f };			// Camera looking at point
    l_camera.up = Vector3{ 0.0f, 1.0f, 0.0f };				// Camera up vector (rotation towards target)
    l_camera.fovy = 45.0f;											// Camera field-of-view Y
    l_camera.projection = CAMERA_PERSPECTIVE;						// Camera mode type

	Map l_map(&l_camera);
	l_map.setSize(Vector2{ 10, 10 });
	IHM l_ihm(&l_map, &l_camera);

	ImGuiIO& l_IO = ImGui::GetIO();

	// Main loop
	while (!WindowShouldClose())
	{
		// Update Camera
		if (l_camera.projection == CAMERA_PERSPECTIVE && !l_IO.WantCaptureMouse)
		{
			if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
			{
				UpdateCamera(&l_camera, CAMERA_THIRD_PERSON);
			}
			else
			{
				const float l_mouseWheelMovement = GetMouseWheelMove();
				Vector3 mouse = Vector3Normalize(Vector3Subtract(l_camera.target, l_camera.position));
				l_camera.position = Vector3Add(l_camera.position, Vector3Scale(mouse, l_mouseWheelMovement));
			}
		}

		l_map.o_update();

		// Draw
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);

			BeginMode3D(l_camera);
			{
				DrawGrid(static_cast<int>(l_map.getSize().x), 1.f);
				DrawGrid(static_cast<int>(l_map.getSize().y), 1.f);

				l_map.o_draw();
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
