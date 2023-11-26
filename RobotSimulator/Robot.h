#pragma once
#include <vector>

#include "Obstacle.h"

class Robot
{
public:
	Robot();
	explicit Robot(std::vector<Obstacle>* a_obstacleList);

	void o_draw() const;
	void o_update();

	Vector3& o_getPosition();
	float& o_getAngle();

	int& o_getAngleMesure() { return m_angleScan; }
	float& o_getPasAngleMesure() { return m_pasAngleScan; }

	bool& o_GetDrawRays() { return m_drawRays; }

private:

	void o_computeRay(int a_angle);

	Model m_model;
	Vector3 m_position;
	float m_angle;

	float m_deltaTime = 0;

	bool m_drawRays = false;

	int m_angleScan;
	float m_pasAngleScan;

	std::vector<Obstacle>* m_obstacleList;
	std::vector<RayCollision> m_rayList;

};

