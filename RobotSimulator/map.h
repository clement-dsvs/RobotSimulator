#pragma once
#include <vector>
#include <raylib.h>
#include <string>

#include "Obstacle.h"
#include "Robot.h"

class Map
{
public:
	explicit Map(Camera* a_camera);

	void o_update();

	void o_draw() const;

	void o_fromJson(const char* a_fileName, Camera* a_camera);
	bool o_toJson(const char* a_fileName) const;

	std::vector<Obstacle>& o_getObstacleList();
	bool o_addObstacle(const Obstacle& a_obstacle);

	Vector2& o_getSize();
	void o_setSize(const Vector2 a_newSize);

	std::string& o_getFilePath();
	void o_setFilePath(const char* a_newFilePath);

	bool o_hasSelectedObstacle() const;
	Obstacle* o_getSelectedObstacle() const { return m_selectedObstacle; }

	bool o_showObstacleEditWindow() const { return m_showObstacleEditWindow; }

	Robot* o_getRobot() { return &m_robot; }

private:
	std::vector<Obstacle> m_ObstacleList;
	Vector2 m_Size;
	std::string m_FilePath;

	bool m_showObstacleEditWindow = false;

	Camera* m_camera;
	Obstacle* m_selectedObstacle = nullptr;
	Robot m_robot;
};
