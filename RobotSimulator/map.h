#pragma once
#include <vector>
#include <raylib.h>
#include <string>

#include "Obstacle.h"
#include "Robot.h"

class Map
{
public:
	Map(Camera* a_camera);

	void o_update();

	void o_draw() const;

	void fromJSON(const char* a_FileName, Camera* camera);
	bool toJSON(const char* a_FileName) const;

	std::vector<Obstacle>& getObstacleList();
	bool addObstacle(const Obstacle a_Obstacle);

	Vector2& getSize();
	void setSize(const Vector2 a_NewSize);
	void updateSize();

	std::string& getFilePath();
	void setFilePath(const char* a_NewFilePath);

	bool hasSelectedObstacle();
	Obstacle* getSelectedObstacle() { return m_selectedObstacle; }

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
