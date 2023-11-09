#include "Map.h"

#include <fstream>
#include <cjson/cJSON.h>

Map::Map()
{
	m_FileName = "Nouveau.json";
	m_FilePath = nullptr;
	m_Size = Vector2{ 10, 10 };
}

Map Map::fromJSON(const char* a_FileName)
{
	Map l_Map;

	// Parse JSON

	return l_Map;
}

bool Map::toJSON(const char* a_FileName)
{
	// Write JSON
	cJSON* l_jsonMap = cJSON_CreateObject();

	cJSON* l_jsonSize = cJSON_AddObjectToObject(l_jsonMap, "Size");
	cJSON_AddNumberToObject(l_jsonSize, "X", m_Size.x);
	cJSON_AddNumberToObject(l_jsonSize, "Y", m_Size.y);

	cJSON* l_jsonListeObstacle = cJSON_AddArrayToObject(l_jsonMap, "Obstacles");

	for(auto l_Obstacle : this->m_ObstacleList)
	{
		cJSON* l_jsonObstacle = l_Obstacle.toJSON();
		cJSON_AddItemToArray(l_jsonListeObstacle, l_jsonObstacle);
	}

	std::string output_string = cJSON_Print(l_jsonMap);

	// Open file
	std::ofstream file;
	file.open(a_FileName);
	file << output_string;
	file.close();

	cJSON_Delete(l_jsonMap);
	return true;
}

std::vector<Obstacle>* Map::getObstacleList()
{
	return &this->m_ObstacleList;
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

Vector2 Map::getSize()
{
	return this->m_Size;
}

void Map::setSize(const Vector2 a_NewSize)
{
	this->m_Size = a_NewSize;
}

const char* Map::getFilePath()
{
	return this->m_FilePath;
}

void Map::setFilePath(const char* a_NewFilePath)
{
	this->m_FilePath = a_NewFilePath;
}
