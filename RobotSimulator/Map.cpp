#include "Map.h"

#include "utils.h"

#include <fstream>
#include <imgui.h>
#include <sstream>
#include <cjson/cJSON.h>

#include <raymath.h>

Map::Map(Camera* a_camera)
{
	m_camera = a_camera;
	m_Size = Vector2{ 10, 10 };
}

void Map::o_update()
{
	const ImGuiIO& io = ImGui::GetIO();

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !io.WantCaptureMouse)
	{
		float l_nearestHit = HUGE_VALF;
		m_selectedObstacle = nullptr;

		for (auto& l_obstacle : m_ObstacleList)
		{
			const RayCollision l_collision = GetRayCollisionMesh(GetMouseRay(GetMousePosition(), *m_camera), l_obstacle.getMesh(), MatrixTranslate(l_obstacle.getPosition().x, l_obstacle.getPosition().y, l_obstacle.getPosition().z));
			if (l_collision.hit && l_collision.distance < l_nearestHit)
			{
				if (m_selectedObstacle)
				{
					m_selectedObstacle->unselect();
				}
				m_selectedObstacle = &l_obstacle;
				l_obstacle.select();
				l_nearestHit = l_collision.distance;
			}
			else
			{
				l_obstacle.unselect();
			}
		}
	}

	if (m_selectedObstacle)
	{
		if (IsKeyPressed(KEY_DELETE))
		{
			m_ObstacleList.erase(std::remove(m_ObstacleList.begin(), m_ObstacleList.end(), *m_selectedObstacle));
		}
		else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !Vector2Equals(GetMouseDelta(),Vector2Zero()))
		{
			Ray l_mouseRay = GetMouseRay(GetMousePosition(), *m_camera);
			Mesh l_plane = GenMeshPlane(m_Size.x, m_Size.y, 1, 1);
			RayCollision l_collision = GetRayCollisionMesh(l_mouseRay, l_plane, MatrixTranslate(0, 0, 0));
			if (l_collision.hit)
			{
				m_selectedObstacle->setPosition(l_collision.point);
			}
			UnloadMesh(l_plane);
		}
	}
}

void Map::o_draw() const
{
	for (const auto& l_obstacle : m_ObstacleList)
	{
		l_obstacle.o_draw();
	}
}

Map Map::fromJSON(const char* a_FileName, Camera* a_camera)
{
	Map l_Map(a_camera);
	l_Map.setFilePath(a_FileName);

	// Parse JSON
	std::ifstream file(a_FileName);
	std::stringstream buffer;

	buffer << file.rdbuf();
	std::string temp_string = buffer.str();
	cJSON* l_jsonFile = cJSON_Parse(temp_string.c_str());

	cJSON* l_jsonSize = getJSONChild(l_jsonFile, "Size");
	if (l_jsonSize != nullptr)
	{
		Vector2 l_MapSize;
		cJSON* l_jsonX = getJSONChild(l_jsonSize, "X");
		if (l_jsonX != nullptr)
		{
			l_MapSize.x = l_jsonX->valueint;
		}
		cJSON* l_jsonY = getJSONChild(l_jsonSize, "Y");
		if (l_jsonY != nullptr)
		{
			l_MapSize.y = l_jsonY->valueint;
		}
		l_Map.setSize(l_MapSize);
	}

	cJSON* l_jsonObstacleList = getJSONChild(l_jsonFile, "Obstacles");
	cJSON* l_jsonObstacle = l_jsonObstacleList->child;

	while (l_jsonObstacle != nullptr)
	{
		l_Map.addObstacle(Obstacle::fromJSON(l_jsonObstacle));
		l_jsonObstacle = l_jsonObstacle->next;
	}

	cJSON_Delete(l_jsonFile);
	return l_Map;
}

bool Map::toJSON(const char* a_FileName) const
{
	// Write JSON
	cJSON* l_jsonMap = cJSON_CreateObject();
	if (l_jsonMap == nullptr) return false;

	cJSON* l_jsonSize = cJSON_AddObjectToObject(l_jsonMap, "Size");
	if (l_jsonSize == nullptr) return false;
	cJSON_AddNumberToObject(l_jsonSize, "X", m_Size.x);
	cJSON_AddNumberToObject(l_jsonSize, "Y", m_Size.y);

	cJSON* l_jsonListeObstacle = cJSON_AddArrayToObject(l_jsonMap, "Obstacles");
	if (l_jsonListeObstacle == nullptr) return false;

	for(auto l_Obstacle : this->m_ObstacleList)
	{
		cJSON* l_jsonObstacle = l_Obstacle.toJSON();
		cJSON_AddItemToArray(l_jsonListeObstacle, l_jsonObstacle);
	}

	const std::string l_outputString = cJSON_Print(l_jsonMap);

	// Open file
	std::ofstream l_file;
	l_file.open(a_FileName);
	l_file << l_outputString;
	l_file.close();

	cJSON_Delete(l_jsonMap);
	return true;
}

std::vector<Obstacle>& Map::getObstacleList()
{
	return this->m_ObstacleList;
}

bool Map::addObstacle(const Obstacle a_Obstacle)
{
	try
	{
		this->m_ObstacleList.push_back(a_Obstacle);
		return true;
	}
	catch (const std::exception& e)
	{
		// Log error
		return false;
	}
}

Vector2& Map::getSize()
{
	return this->m_Size;
}

void Map::setSize(const Vector2 a_NewSize)
{
	this->m_Size = a_NewSize;
}

void Map::updateSize()
{
}

std::string& Map::getFilePath()
{
	return m_FilePath;
}

void Map::setFilePath(const char* a_NewFilePath)
{
	this->m_FilePath = a_NewFilePath;
}
