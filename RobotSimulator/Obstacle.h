#pragma once

#include <raylib.h>
#include <string>
#include <cjson/cJSON.h>

enum ObstacleType
{
	E_OBSTACLE_UNDEFINED = -1,
	E_OBSTACLE_FLOOR = 0,
};

class Obstacle
{
public:
	Obstacle();

	cJSON* o_toJson() const;

	static Obstacle fromJSON(const cJSON* a_jsonObstacle);

	static Obstacle GenCube(float a_width, float a_height, float a_length);

	void o_draw() const;

	void setMesh(const Mesh& a_Mesh);
	const Mesh& getMesh();

	Model& o_getModel() { return m_Model; }
	void setModel(const Model& a_model);

	void setModelFileName(const char* a_fileName);

	Vector3& getPosition() {return m_Position;}
	void setPosition(const Vector3& a_Position);
	void setPosition(const float a_x = 0, const float a_y = 0, const float a_z = 0);

	ObstacleType getObstacleType();

	void select() { m_Selected = true;}
	void unselect() { m_Selected = false;}

	void o_rotate(int angle);
	int o_getRotation() const { return m_Rotation;}

	bool operator==(const Obstacle&) const;

private:
	Mesh m_Mesh;
	Model m_Model;
	std::string m_modelFileName;
	Color m_Color;

	int m_Rotation = 0;

	Vector3 m_Position;

	bool m_Selected;

	ObstacleType m_ObstacleType = E_OBSTACLE_UNDEFINED;
};

