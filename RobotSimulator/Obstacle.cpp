#include "Obstacle.h"

#include <cjson/cJSON.h>
#include <raylib.h>
#include <raymath.h>

#include "ModelManager.h"
#include "utils.h"

Obstacle::Obstacle()
{
	this->m_Position = Vector3 {0, 0, 0};
	m_Color = RED;
	m_Selected = false;
	m_Rotation = 0;
}

cJSON* Obstacle::o_toJson() const
{
	cJSON* l_jsonObject = cJSON_CreateObject();

	cJSON_AddStringToObject(l_jsonObject, "Filename", m_modelFileName.c_str());
	cJSON_AddNumberToObject(l_jsonObject, "Rotation", m_Rotation);

	cJSON* l_jsonPosition = cJSON_AddObjectToObject(l_jsonObject, "Position");

	cJSON_AddNumberToObject(l_jsonPosition, "X", m_Position.x);
	cJSON_AddNumberToObject(l_jsonPosition, "Y", m_Position.y);
	cJSON_AddNumberToObject(l_jsonPosition, "Z", m_Position.z);

	return l_jsonObject;
}

Obstacle Obstacle::fromJSON(const cJSON* a_jsonObstacle)
{
	Obstacle l_Obstacle;
	//const cJSON* l_jsonObstacle = a_jsonObstacle;

	cJSON* l_jsonFilename = getJSONChild(a_jsonObstacle, "Filename");
	if (l_jsonFilename == nullptr) return l_Obstacle;

	const char* l_modelFileName = cJSON_GetStringValue(l_jsonFilename);

	ModelManager* l_manager = ModelManager::os_GetInstance();
	Model l_model = l_manager->o_GetModel(l_modelFileName);
	l_Obstacle.setModel(l_model);
	l_Obstacle.setModelFileName(l_modelFileName);

	cJSON* l_jsonRotation = getJSONChild(a_jsonObstacle, "Rotation");
	if (l_jsonRotation != nullptr)
	{
		l_Obstacle.m_Rotation = cJSON_GetNumberValue(l_jsonRotation);
	}

	cJSON* l_jsonPosition = getJSONChild(a_jsonObstacle, "Position");
	cJSON* l_jsonX = getJSONChild(l_jsonPosition, "X");
	cJSON* l_jsonY = getJSONChild(l_jsonPosition, "Y");
	cJSON* l_jsonZ = getJSONChild(l_jsonPosition, "Z");
	if (l_jsonX == nullptr || l_jsonY == nullptr || l_jsonZ == nullptr) return l_Obstacle;

	l_Obstacle.setPosition(cJSON_GetNumberValue(l_jsonX), cJSON_GetNumberValue(l_jsonY), cJSON_GetNumberValue(l_jsonZ));

	return l_Obstacle;
}

void Obstacle::o_draw() const
{
	DrawModelEx(m_Model, m_Position, Vector3{0, 1, 0}, m_Rotation, Vector3{1, 1, 1}, WHITE);
	if (m_Selected)
	{
		BoundingBox l_box = GetMeshBoundingBox(m_Mesh);
		l_box.min = Vector3Add(l_box.min, m_Position);
		l_box.max = Vector3Add(l_box.max, m_Position);
		DrawBoundingBox(l_box, GREEN);
	}
}

void Obstacle::setMesh(const Mesh& a_Mesh)
{
	this->m_Mesh = a_Mesh;
	this->m_Model = LoadModelFromMesh(m_Mesh);
}

const Mesh& Obstacle::getMesh()
{
	return m_Mesh;
}

void Obstacle::setModel(const Model& a_model)
{
	m_Model = a_model;
	m_Mesh = a_model.meshes[0];
}

void Obstacle::setModelFileName(const char* a_fileName)
{
	m_modelFileName = a_fileName;
}

void Obstacle::setPosition(const Vector3& a_Position)
{
	this->m_Position = a_Position;
}

void Obstacle::setPosition(const float a_x, const float a_y, const float a_z)
{
	this->m_Position.x = a_x;
	this->m_Position.y = a_y;
	this->m_Position.z = a_z;
}

ObstacleType Obstacle::getObstacleType()
{
	return m_ObstacleType;
}

void Obstacle::o_rotate(int angle)
{
	m_Rotation += angle;
	m_Rotation %= 360;
}

bool Obstacle::operator==(const Obstacle& a_other) const
{
	return this == &a_other;
}
