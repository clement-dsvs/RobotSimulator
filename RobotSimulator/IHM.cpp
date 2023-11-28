#include "IHM.h"

#include <filesystem>
#include <imgui_impl_raylib.h>
#include <iostream>
#include <raylib.h>

#include "dialogs.h"
#include "Obstacle.h"
#include "rlImGui.h"

namespace fs = std::filesystem;

IHM::IHM(Map* a_map, Camera* a_camera)
{
	m_map = a_map;
	m_robot = a_map->o_getRobot();
	m_camera = a_camera;

	m_afficherListeObstacles = false;
	m_afficherEditeurCarte = false;
	m_afficherEditeurRobot = false;
	m_afficherFPS = false;
	m_robotViewTexture = { 0 };
}

void IHM::o_draw()
{
	if (m_afficherFPS)
	{
		DrawFPS(10, 25);
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Fichier"))
		{
			if (ImGui::MenuItem("Nouveau"))
			{
				*m_map = Map(m_camera);
			}

			if (ImGui::MenuItem("Ouvrir"))
			{
				char* fileName = OpenImportDialog();
				if (fileName != nullptr)
				{
					std::cout << fileName << std::endl;
					m_map->o_fromJson(fileName, m_camera);
					free(fileName);
				}
			}

			if (ImGui::MenuItem("Enregistrer"))
			{
				if (!m_map->o_getFilePath().empty())
				{
					m_map->o_toJson(m_map->o_getFilePath().c_str());
				}
				else
				{
					// TODO: remplacer par méthode "enregistrer-sous"
					char* fileName = OpenExportDialog();
					if (fileName != nullptr)
					{
						std::cout << fileName << std::endl;
						m_map->o_toJson(fileName);
						free(fileName);
					}
				}
			}

			if (ImGui::MenuItem("Enregistrer Sous"))
			{
				char* fileName = OpenExportDialog();
				if (fileName != nullptr)
				{
					std::cout << fileName << std::endl;
					m_map->o_toJson(fileName);
					free(fileName);
				}
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Simulation"))
		{
			if (ImGui::MenuItem("Commencer", nullptr, false, !m_map->o_isSimulationRunning()))
			{
				m_map->o_startSimulation();
			}

			if (ImGui::MenuItem("Arreter", nullptr, false, m_map->o_isSimulationRunning()))
			{
				m_map->o_stopSimulation();
			}

			if (ImGui::MenuItem("Recommencer"))
			{

			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Afficher"))
		{
			if (ImGui::BeginMenu("Projection"))
			{
				if (ImGui::MenuItem("Perspective", nullptr, (m_camera->projection == CAMERA_PERSPECTIVE)))
				{
					m_camera->projection = CAMERA_PERSPECTIVE;
					m_camera->position = Vector3{ 0, 10.f, 10.f };
				}
				if (ImGui::MenuItem("Orthographique", nullptr, (m_camera->projection == CAMERA_ORTHOGRAPHIC)))
				{
					m_camera->projection = CAMERA_ORTHOGRAPHIC;
					m_camera->position = Vector3{ 0, 10.f, .1f };
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Fenetres"))
			{
				if (ImGui::MenuItem("Liste des obstacles", NULL, m_afficherListeObstacles))
				{
					m_afficherListeObstacles = !m_afficherListeObstacles;
				}

				if (ImGui::MenuItem("Editeur de carte", NULL, m_afficherEditeurCarte))
				{
					m_afficherEditeurCarte = !m_afficherEditeurCarte;
				}

				if (ImGui::MenuItem("Robot", NULL, m_afficherEditeurRobot))
				{
					m_afficherEditeurRobot = !m_afficherEditeurRobot;
				}

				if (ImGui::MenuItem("Vue du robot", NULL, m_afficherVueRobot))
				{
					m_afficherVueRobot = !m_afficherVueRobot;
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("FPS", nullptr, m_afficherFPS))
			{
				m_afficherFPS = !m_afficherFPS;
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (m_afficherListeObstacles)
	{
		if (ImGui::Begin("Liste Obstacles", &m_afficherListeObstacles))
		{
			const char* path = "D:/code/C/RobotSimulator/assets/models";
			//const char* path = "C:\\Users\\clement\\code\\C++\\robotsimulator\\assets\\models";
			o_listFileInDir(path);
		}
		ImGui::End();
	}

	if (m_afficherEditeurCarte)
	{
		if (ImGui::Begin("Editeur de carte", &m_afficherEditeurCarte))
		{
			ImGui::Text("Taille de la carte :");
			ImGui::SliderFloat("X", &m_map->o_getSize().x, 1, 255, "%.0f");
			ImGui::SliderFloat("Y", &m_map->o_getSize().y, 1, 255, "%.0f");
		}
		ImGui::End();
	}

	if (m_afficherEditeurRobot)
	{
		if (ImGui::Begin("Robot", &m_afficherEditeurRobot))
		{
			ImGui::Checkbox("Afficher les rayons", &m_robot->o_getDrawRays());
			ImGui::InputFloat("X", &m_robot->o_getPosition().x);
			ImGui::InputFloat("Y", &m_robot->o_getPosition().y);
			ImGui::InputFloat("Z", &m_robot->o_getPosition().z);
			ImGui::SliderAngle("Angle", &m_robot->o_getAngle());
			ImGui::SliderInt("Angle Mesure", &m_robot->o_getAngleMesure(), 0, 180);
			ImGui::SliderFloat("Pas Mesure", &m_robot->o_getPasAngleMesure(), 1, 360, "%.f");
			ImGui::InputFloat("Vitesse", &m_robot->o_getVelocity());
		}
		ImGui::End();
	}

	if (m_afficherVueRobot) o_afficherVueRobot();

	if (m_map->o_showObstacleEditWindow())
	{
		if (ImGui::Begin("Editeur d'obstacle", &m_afficherEditeurCarte))
		{
			ImGui::InputFloat("X", &m_map->o_getSelectedObstacle()->getPosition().x);
			ImGui::InputFloat("Y", &m_map->o_getSelectedObstacle()->getPosition().y);
			ImGui::InputFloat("Z", &m_map->o_getSelectedObstacle()->getPosition().z);
		}
		ImGui::End();
	}
}

void IHM::o_listFileInDir(const char* a_path)
{
	for (const auto& l_entry : fs::directory_iterator(a_path))
	{
		if (l_entry.is_directory())
		{
			if (ImGui::CollapsingHeader(l_entry.path().filename().string().c_str()))
			{
				o_listFileInDir(l_entry.path().string().c_str());
			}
		}
		else
		{
			if (ImGui::Button(l_entry.path().stem().stem().string().c_str()))
			{
				Model l_model = LoadModel(l_entry.path().string().c_str());
				Obstacle l_obstacle;
				l_obstacle.setModel(l_model);
				l_obstacle.setModelFileName(l_entry.path().string().c_str());
				m_map->o_addObstacle(l_obstacle);
			}
		}
	}
}

float Lerp(float a, float b, float t)
{
	return (1.0f - t) * a + b * t;
}

float InvLerp(float a, float b, float v)
{
	return (v - a) / (b - a);
}

float mapValue(const float a_input, const float a_inputMin, const float a_inputMax, const float a_outputMin, const float a_outputMax)
{
	float t = InvLerp(a_inputMin, a_inputMax, a_input);
	return Lerp(a_outputMin, a_outputMax, t);
}

void IHM::o_afficherVueRobot()
{
	if (ImGui::Begin("Vue robot", &m_afficherVueRobot))
	{
		Image l_image = GenImageColor(m_map->o_getSize().x, m_map->o_getSize().y, RAYWHITE);

		for(const auto& l_collision : m_robot->o_getRayList())
		{
			//const float l_x = mapValue(l_collision.point.x, -(m_map->o_getSize().x/2), m_map->o_getSize().x/2, 0, l_image.width);
			//const float l_y = mapValue(l_collision.point.y, -(m_map->o_getSize().y/2), m_map->o_getSize().y/2, 0, l_image.height);
			const float l_x = l_collision.point.x;
			const float l_y = l_collision.point.y;

			ImageDrawPixel(&l_image, l_y, l_x, BLUE);
		}
		m_robotViewTexture = LoadTextureFromImage(l_image);

		//UnloadImage(l_image);

		rlImGuiImageSize(&m_robotViewTexture, 300, 300);

	}
	ImGui::End();
}
