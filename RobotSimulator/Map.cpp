﻿#include "Map.h"

#include "utils.h"

#include <fstream>
#include <sstream>
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
