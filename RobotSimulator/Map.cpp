#include "Map.h"

#include "utils.h"

#include <fstream>
#include <imgui.h>
#include <sstream>
#include <cjson/cJSON.h>

#include <raymath.h>
#include <thread>

Map::Map(Camera* a_camera)
{
	m_camera = a_camera;
	m_Size = Vector2{ 10, 10 };
	m_robot = Robot(&m_ObstacleList);
}

void Map::o_update()
{
	m_robot.o_update();
	const ImGuiIO& io = ImGui::GetIO();

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !io.WantCaptureMouse)
	{
		float l_nearestHit = HUGE_VALF;
		m_selectedObstacle = nullptr;

		for (auto& l_obstacle : m_ObstacleList)
		{
			Matrix l_rotationMatrix = MatrixRotateY(DEG2RAD * static_cast<float>(l_obstacle.o_getRotation()));
			Matrix l_translationMatrix = MatrixTranslate(l_obstacle.getPosition().x, l_obstacle.getPosition().y, l_obstacle.getPosition().z);
			Matrix l_transformMatrix = MatrixMultiply(l_rotationMatrix, l_translationMatrix);

			const RayCollision l_collision = GetRayCollisionMesh(GetMouseRay(GetMousePosition(), *m_camera), l_obstacle.getMesh(), l_transformMatrix);
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
		Ray l_mouseRay = GetMouseRay(GetMousePosition(), *m_camera);
		const RayCollision l_collision = GetRayCollisionMesh(GetMouseRay(GetMousePosition(), *m_camera), m_selectedObstacle->getMesh(), MatrixTranslate(m_selectedObstacle->getPosition().x, m_selectedObstacle->getPosition().y, m_selectedObstacle->getPosition().z));
		if (GetMouseWheelMove() != 0.f)
		{
			m_selectedObstacle->o_rotate(GetMouseWheelMove());
		}

		if (l_collision.hit && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			m_showObstacleEditWindow = !m_showObstacleEditWindow;
		}
	}
	else
	{
		m_showObstacleEditWindow = false;
	}
}

void Map::o_draw() const
{
	for (const auto& l_obstacle : m_ObstacleList)
	{
		l_obstacle.o_draw();
	}

	m_robot.o_draw();

}

void Map::o_fromJson(const char* a_fileName, Camera* a_camera)
{
	o_setFilePath(a_fileName);

	// Parse JSON
	const std::ifstream l_file(a_fileName);
	std::stringstream l_buffer;

	l_buffer << l_file.rdbuf();
	const std::string l_tempString = l_buffer.str();
	cJSON* l_jsonFile = cJSON_Parse(l_tempString.c_str());

	const cJSON* l_jsonSize = getJSONChild(l_jsonFile, "Size");
	if (l_jsonSize != nullptr)
	{
		Vector2 l_MapSize{0, 0};
		const cJSON* l_jsonX = getJSONChild(l_jsonSize, "X");
		if (l_jsonX != nullptr)
		{
			l_MapSize.x = static_cast<int>(l_jsonX->valueint);
		}
		cJSON* l_jsonY = getJSONChild(l_jsonSize, "Y");
		if (l_jsonY != nullptr)
		{
			l_MapSize.y = l_jsonY->valueint;
		}
		o_setSize(l_MapSize);
	}

	const cJSON* l_jsonObstacleList = getJSONChild(l_jsonFile, "Obstacles");
	const cJSON* l_jsonObstacle = l_jsonObstacleList->child;

	m_ObstacleList.clear();
	while (l_jsonObstacle != nullptr)
	{
		o_addObstacle(Obstacle::fromJSON(l_jsonObstacle));
		l_jsonObstacle = l_jsonObstacle->next;
	}

	cJSON_Delete(l_jsonFile);
}

bool Map::o_toJson(const char* a_fileName) const
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

	for(const auto& l_obstacle : this->m_ObstacleList)
	{
		cJSON* l_jsonObstacle = l_obstacle.o_toJson();
		cJSON_AddItemToArray(l_jsonListeObstacle, l_jsonObstacle);
	}

	const std::string l_outputString = cJSON_Print(l_jsonMap);

	// Open file
	std::ofstream l_file;
	l_file.open(a_fileName);
	l_file << l_outputString;
	l_file.close();

	cJSON_Delete(l_jsonMap);
	return true;
}

std::vector<Obstacle>& Map::o_getObstacleList()
{
	return this->m_ObstacleList;
}

bool Map::o_addObstacle(const Obstacle& a_obstacle)
{
	try
	{
		this->m_ObstacleList.push_back(a_obstacle);
		return true;
	}
	catch (const std::exception& l_exception)
	{
		// Log error
		return false;
	}
}

Vector2& Map::o_getSize()
{
	return this->m_Size;
}

void Map::o_setSize(const Vector2 a_newSize)
{
	this->m_Size = a_newSize;
}

std::string& Map::o_getFilePath()
{
	return m_FilePath;
}

void Map::o_setFilePath(const char* a_newFilePath)
{
	this->m_FilePath = a_newFilePath;
}

bool Map::o_hasSelectedObstacle() const
{
	return m_selectedObstacle != nullptr;
}
