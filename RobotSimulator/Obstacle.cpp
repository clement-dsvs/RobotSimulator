#include "Obstacle.h"

#include <cjson/cJSON.h>
#include <raylib.h>

Obstacle::Obstacle()
{
	this->m_Position = Vector3 {0, 0, 0};
	Model l_model = LoadModelFromMesh(m_Mesh);
	m_Color = RED;
	m_Selected = false;
}

cJSON* Obstacle::toJSON()
{
	return NULL;
}

Obstacle Obstacle::fromJSON(cJSON* a_jsonObstacle)
{
	Obstacle l_Obstacle;

	//TODO: Parse JSON

	return l_Obstacle;
}

void Obstacle::render() const
{
	DrawModel(m_Model, m_Position, 1, RED);
	if (m_Selected)
	{
		DrawBoundingBox(GetMeshBoundingBox(m_Mesh), GREEN);
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

const Model& Obstacle::getModel()
{
	return m_Model;
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
