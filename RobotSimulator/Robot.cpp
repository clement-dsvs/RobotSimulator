#include "Robot.h"

#include <raymath.h>

#define K_ROBOT_MODEL "D:\\code\\C\\RobotSimulator\\assets\\robot\\robot.m3d"

Robot::Robot()
{
	m_deltaTime = 0;
}

Robot::Robot(std::vector<Obstacle>* a_obstacleList): m_angle(0), m_pasAngleScan(10)
{
	m_model = LoadModel(K_ROBOT_MODEL);
	m_Position = Vector3{0, 0.19f, 0};
	m_drawRays = true;

	m_angleScan = 180;

	m_obstacleList = a_obstacleList;
}

void Robot::o_draw() const
{
	DrawModelEx(m_model, m_Position, Vector3{0, 1, 0}, RAD2DEG * m_angle, Vector3{1, 1, 1}, WHITE);

	for (const auto l_ray : m_rayList)
	{
		DrawLine3D(m_Position, l_ray.point, GREEN);
	}
}

void Robot::o_update()
{
	m_rayList.clear();
	for (int l_angle = 0; l_angle < m_angleScan; l_angle += m_pasAngleScan)
	{
		//float l_theta = DEG2RAD * l_angle + (m_angle - m_angleScan / 2);
		float l_theta = DEG2RAD * l_angle;
		Ray l_ray;
		l_ray.position = m_Position;
		l_ray.direction = Vector3 {cosf(l_theta), 0.f, sinf(l_theta)};
		RayCollision l_collision;
		l_collision.hit = false;
		l_collision.distance = HUGE_VALF;

		for (Obstacle& l_obstacle : *m_obstacleList)
		{
			try
			{
				Model& l_model = l_obstacle.o_getModel();

				Matrix l_rotationMatrix = MatrixRotateY(DEG2RAD * static_cast<float>(l_obstacle.o_getRotation()));
				Matrix l_translationMatrix = MatrixTranslate(l_obstacle.getPosition().x, l_obstacle.getPosition().y, l_obstacle.getPosition().z);
				Matrix l_transformMatrix = MatrixMultiply(l_rotationMatrix, l_translationMatrix);
				RayCollision l_tempRay = GetRayCollisionMesh(l_ray, l_model.meshes[0], l_transformMatrix);

				if (l_tempRay.hit && l_tempRay.distance < l_collision.distance)
				{
					l_collision = l_tempRay;
				}
			}
			catch (...) {}

		}

		if (l_collision.hit)
		{
			m_rayList.push_back(l_collision);
		}

	}

}

Vector3& Robot::o_getPosition()
{
	return m_Position;
}

float& Robot::o_getAngle()
{
	return m_angle;
}
