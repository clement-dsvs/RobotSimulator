#pragma once
#include <vector>

#include "Obstacle.h"

class Robot
{
public:
	Robot();
	Robot(std::vector<Obstacle>* a_obstacleList);

	void o_draw() const;
	void o_update();

	Vector3& o_getPosition();
	float& o_getAngle();

	int& o_getAngleMesure() { return m_angleScan; }
	int& o_getPasAngleMesure() { return m_pasAngleScan; }

private:
	Model m_model;
	Vector3 m_Position;
	float m_angle;

	float m_deltaTime = 0;

	bool m_drawRays;

	int m_angleScan;
	int m_pasAngleScan;

	std::vector<Obstacle>* m_obstacleList;
	std::vector<RayCollision> m_rayList;

};

