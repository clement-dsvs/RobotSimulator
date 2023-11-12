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

	bool l_afficherListeObstacles = false;
	ImGuiIO& io = ImGui::GetIO();

	// Main loop
	while (!WindowShouldClose())
	{
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

		BeginDrawing();
		ClearBackground(RAYWHITE);

		BeginMode3D(camera);

		DrawGrid(100, 1);

		float l_nearestHit = HUGE_VALF;
		Obstacle* l_selectedObstacle = nullptr;


		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !io.WantCaptureMouse)
		{
			std::cout << "Searching for collision" << std::endl;
			for (auto& l_Obstacle : map.getObstacleList())
			{
				RayCollision l_collision = GetRayCollisionMesh(GetMouseRay(GetMousePosition(), camera), l_Obstacle.getMesh(), l_Obstacle.getModel().transform);

				if (l_collision.hit && l_collision.distance < l_nearestHit)
				{
					if (l_selectedObstacle)
					{
						l_selectedObstacle->unselect();
					}
					l_selectedObstacle = &l_Obstacle;
					l_Obstacle.select();
					l_nearestHit = l_collision.distance;
				}
				else
				{
					l_Obstacle.unselect();
				}
			}
		}

		for (auto l_Obstacle : map.getObstacleList())
		{
			l_Obstacle.render();
		}

		EndMode3D();

		// start ImGui Content
		rlImGuiBegin();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Fichier"))
			{
				if (ImGui::MenuItem("Nouveau"))
				{
					map = Map();
				}

				if (ImGui::MenuItem("Ouvrir"))
				{
					char* fileName = OpenImportDialog();
					if (fileName != nullptr)
					{
						std::cout << fileName << std::endl;
						map = Map::fromJSON(fileName);
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

			if (ImGui::BeginMenu("Simulation"))
			{
				if (ImGui::MenuItem("Commencer"))
				{
					
				}

				if (ImGui::MenuItem("Arreter"))
				{
					
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Afficher"))
			{
				if (ImGui::BeginMenu("Projection"))
				{
					if (ImGui::MenuItem("Perspective", nullptr, (camera.projection == CAMERA_PERSPECTIVE)))
					{
						camera.projection = CAMERA_PERSPECTIVE;
						camera.position = Vector3 {0, 10.f, 10.f};
					}
					if (ImGui::MenuItem("Orthographique", nullptr, (camera.projection == CAMERA_ORTHOGRAPHIC)))
					{
						camera.projection = CAMERA_ORTHOGRAPHIC;
						camera.position = Vector3 {0, 10.f, .1f};
					}
					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Liste des obstacles", NULL, l_afficherListeObstacles))
				{
					l_afficherListeObstacles = !l_afficherListeObstacles;
				}

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (l_afficherListeObstacles)
		{
			if (ImGui::Begin("Liste Obstacles", &l_afficherListeObstacles, ImGuiWindowFlags_MenuBar))
			{
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("Ajouter"))
					{
						if (ImGui::MenuItem("Cube"))
						{
							Obstacle l_NvObstacle;
							l_NvObstacle.setMesh(GenMeshCube(2, 2, 2));
							map.addObstacle(l_NvObstacle);
						}

						if (ImGui::MenuItem("Cylindre"))
						{
							
						}

						ImGui::EndMenu();
					}

					ImGui::EndMenuBar();
				}

				int counter = 0;
				for (auto& l_Obstacle : map.getObstacleList())
				{
					ImGui::Text("Cube");
					char labelX[7];
					sprintf(labelX, "X##%d", counter);
					ImGui::InputFloat(labelX, &l_Obstacle.getPosition().x);
					ImGui::InputFloat("Y", &l_Obstacle.getPosition().y);
					ImGui::InputFloat("Z", &l_Obstacle.getPosition().z);
					counter++;
				}

			}
			ImGui::End();
		}

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
