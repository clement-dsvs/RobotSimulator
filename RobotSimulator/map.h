#pragma once
#include <vector>
#include <raylib.h>
#include <string>

#include "Obstacle.h"

class Map
{
public:
	Map(Camera* a_camera);

	void o_update();

	void o_draw() const;

	static Map fromJSON(const char* a_FileName, Camera* camera);
	bool toJSON(const char* a_FileName) const;

	std::vector<Obstacle>& getObstacleList();
	bool addObstacle(const Obstacle a_Obstacle);

	Vector2& getSize();
	void setSize(const Vector2 a_NewSize);

	const char* getFilePath();
	void setFilePath(const char* a_NewFilePath);

private:
	std::vector<Obstacle> m_ObstacleList;
	Vector2 m_Size;
	const char* m_FileName;
	const char* m_FilePath;

	Camera* m_camera;
};
