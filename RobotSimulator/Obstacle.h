#pragma once

#include <raylib.h>
#include <cjson/cJSON.h>

enum ObstacleType
{
	E_OBSTACLE_UNDEFINED = -1,
	E_OBSTACLE_CUBE = 0,
	E_OBSTACLE_CYLINDER
};

class Obstacle
{
public:
	Obstacle();

	cJSON* toJSON();

	static Obstacle fromJSON(cJSON* a_jsonObstacle);

	static Obstacle GenCube(float a_width, float a_height, float a_length);

	void o_draw() const;

	void setMesh(const Mesh& a_Mesh);
	const Mesh& getMesh();

	const Model& getModel();
	void setModel(const Model& a_model);

	Vector3& getPosition() {return m_Position;}
	void setPosition(const Vector3& a_Position);
	void setPosition(const float a_x = 0, const float a_y = 0, const float a_z = 0);

	ObstacleType getObstacleType();

	void select() { m_Selected = true;}
	void unselect() { m_Selected = false;}

private:
	Mesh m_Mesh;
	Model m_Model;
	Color m_Color;

	Vector3 m_Position;

	bool m_Selected;

	ObstacleType m_ObstacleType = E_OBSTACLE_UNDEFINED;
};

