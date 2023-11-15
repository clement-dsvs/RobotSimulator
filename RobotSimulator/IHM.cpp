#include "IHM.h"

#include <cstdio>
#include <imgui_impl_raylib.h>
#include <iostream>
#include <raylib.h>

#include "dialogs.h"
#include "Obstacle.h"

IHM::IHM(Map* a_map, Camera* a_camera)
{
	m_map = a_map;
	m_camera = a_camera;

	m_afficherListeObstacles = false;
	m_afficherEditeurCarte = false;
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
					//m_map = Map::fromJSON(fileName);
					free(fileName);
				}
			}

			if (ImGui::MenuItem("Enregistrer"))
			{
				if (m_map->getFilePath())
				{
					m_map->toJSON(m_map->getFilePath());
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

				if (ImGui::MenuItem("Editeur de carte"))
				{
					m_afficherEditeurCarte = !m_afficherEditeurCarte;
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
		if (ImGui::Begin("Liste Obstacles", &m_afficherListeObstacles, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Ajouter"))
				{
					if (ImGui::MenuItem("Cube"))
					{
						Obstacle l_nvObstacle = Obstacle::GenCube(2, 2, 2);
						m_map->addObstacle(l_nvObstacle);
					}

					if (ImGui::MenuItem("Cylindre"))
					{

					}

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			int counter = 0;
			for (auto& l_Obstacle : m_map->getObstacleList())
			{
				ImGui::Text("Cube");

				char labelX[7];
				char labelY[7];
				char labelZ[7];
				sprintf(labelX, "X##%d", counter);
				sprintf(labelY, "Y##%d", counter);
				sprintf(labelZ, "Z##%d", counter);

				float step = .5f;
				float fast_step = 1.f;

				ImGui::InputScalar(labelX, ImGuiDataType_Float, &l_Obstacle.getPosition().x, &step, &fast_step);
				ImGui::InputScalar(labelY, ImGuiDataType_Float, &l_Obstacle.getPosition().y, &step, &fast_step);
				ImGui::InputScalar(labelZ, ImGuiDataType_Float, &l_Obstacle.getPosition().z, &step, &fast_step);

				counter++;
			}

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
}
