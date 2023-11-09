#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "rlImGui.h"

#include "dialogs.h"

#include <iostream>
#include <string>

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

	Map map;
	map.setSize(Vector2{ 10, 10 });

	// Main loop
	while (!WindowShouldClose())
	{
		const float mouseWheelMovement = GetMouseWheelMove();
		Vector3 mouse = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
		camera.position = Vector3Add(camera.position, Vector3Scale(mouse, mouseWheelMovement));
		BeginDrawing();
		ClearBackground(RAYWHITE);

		BeginMode3D(camera);

		DrawGrid(100, 1);

		EndMode3D();

		// start ImGui Content
		rlImGuiBegin();
		
		if (ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Fichier"))
				{
					if (ImGui::MenuItem("Nouveau"))
					{
						// Initialiser nouvelle carte
					}

					if (ImGui::MenuItem("Ouvrir"))
					{
						char* fileName = OpenImportDialog();
						if (fileName != nullptr)
						{
							std::cout << fileName << std::endl;
							free(fileName);
						}
					}

					if (ImGui::MenuItem("Enregistrer"))
					{
						if (map.getFilePath())
						{
							map.toJSON(map.getFilePath());
						}
						else
						{
							// TODO: remplacer par méthode "enregistrer-sous"
							char* fileName = OpenExportDialog();
							if (fileName != nullptr)
							{
								std::cout << fileName << std::endl;
								map.toJSON(fileName);
								free(fileName);
							}
						}
					}
					
					if(ImGui::MenuItem("Enregistrer Sous"))
					{
						char* fileName = OpenExportDialog();
						if (fileName != nullptr)
						{
							std::cout << fileName << std::endl;
							map.toJSON(fileName);
							free(fileName);
						}
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Afficher"))
				{
					if (ImGui::BeginMenu("Projection"))
					{
						if (ImGui::MenuItem("Perspective"))
						{
							camera.projection = CAMERA_PERSPECTIVE;
						}
						if (ImGui::MenuItem("Orthographique"))
						{
							camera.projection = CAMERA_ORTHOGRAPHIC;
						}
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
		}
		ImGui::End();

		// end ImGui Content
		rlImGuiEnd();

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
	rlImGuiShutdown();

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
