#include "IHM.h"

#include <filesystem>
#include <imgui_impl_raylib.h>
#include <iostream>
#include <raylib.h>

#include "dialogs.h"
#include "Obstacle.h"

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
					m_map->fromJSON(fileName, m_camera);
					free(fileName);
				}
			}

			if (ImGui::MenuItem("Enregistrer"))
			{
				if (!m_map->getFilePath().empty())
				{
					m_map->toJSON(m_map->getFilePath().c_str());
				}
				else
				{
					// TODO: remplacer par méthode "enregistrer-sous"
					char* fileName = OpenExportDialog();
					if (fileName != nullptr)
					{
						std::cout << fileName << std::endl;
						m_map->toJSON(fileName);
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
					m_map->toJSON(fileName);
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
			//const char* path = "D:/code/C/RobotSimulator/assets/models";
			const char* path = "C:\\Users\\clement\\code\\C++\\robotsimulator\\assets\\models";
			o_listFileInDir(path);
		}
		ImGui::End();
	}

	if (m_afficherEditeurCarte)
	{
		if (ImGui::Begin("Editeur de carte", &m_afficherEditeurCarte))
		{
			ImGui::Text("Taille de la carte :");
			ImGui::SliderFloat("X", &m_map->getSize().x, 1, 255, "%.0f");
			ImGui::SliderFloat("Y", &m_map->getSize().y, 1, 255, "%.0f");
		}
		ImGui::End();
	}

	if (m_afficherEditeurRobot)
	{
		if (ImGui::Begin("Robot", &m_afficherEditeurRobot))
		{
			ImGui::InputFloat("X", &m_robot->o_getPosition().x);
			ImGui::InputFloat("Y", &m_robot->o_getPosition().y);
			ImGui::InputFloat("Z", &m_robot->o_getPosition().z);
			ImGui::SliderAngle("Angle", &m_robot->o_getAngle());
			ImGui::SliderInt("Angle Mesure", &m_robot->o_getAngleMesure(), 0, 180);
			ImGui::SliderInt("Pas Mesure", &m_robot->o_getPasAngleMesure(), 1, 360);
		}
		ImGui::End();
	}

	if (m_map->o_showObstacleEditWindow())
	{
		if (ImGui::Begin("Editeur d'obstacle", &m_afficherEditeurCarte))
		{
			ImGui::InputFloat("X", &m_map->getSelectedObstacle()->getPosition().x);
			ImGui::InputFloat("Y", &m_map->getSelectedObstacle()->getPosition().y);
			ImGui::InputFloat("Z", &m_map->getSelectedObstacle()->getPosition().z);
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
				m_map->addObstacle(l_obstacle);
			}
		}
	}
}