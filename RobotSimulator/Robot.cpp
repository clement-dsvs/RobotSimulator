#include "Robot.h"

#include <raymath.h>
#include <thread>

#define K_ROBOT_MODEL "D:\\code\\C\\RobotSimulator\\assets\\robot\\robot.m3d"
//#define K_ROBOT_MODEL "C:\\Users\\clement\\code\\C++\\robotsimulator\\assets\\robot\\robot.m3d"

Robot::Robot() : Robot(nullptr)
{
}

Robot::Robot(std::vector<Obstacle>* a_obstacleList) : m_angle(0), m_pasAngleScan(10)
{
	m_model = LoadModel(K_ROBOT_MODEL);
	m_position = Vector3{ 0, 0.19f, 0 };
	m_drawRays = true;

	m_angleScan = 180;

	m_obstacleList = a_obstacleList;
}

void Robot::o_draw() const
{
	DrawModelEx(m_model, m_position, Vector3{ 0, 1, 0 }, RAD2DEG * m_angle, Vector3{ 1, 1, 1 }, WHITE);

	if (m_drawRays)
	{
		for (const auto& l_ray : m_rayList)
		{
			DrawLine3D(m_position, l_ray.point, GREEN);
		}
	}
}

void Robot::o_update()
{
	if (m_deltaTime < 1.f / 60)
	{
		m_deltaTime += GetFrameTime();
		return;
	}
	m_deltaTime = 0;

	m_rayList.clear();
	std::vector<std::thread> l_threadList;

	const int l_halfAngleScan = (m_angleScan / 2);
	const float l_angleDepart = (RAD2DEG * -m_angle + 90) - l_halfAngleScan;
	const float l_angleFin = (RAD2DEG * -m_angle + 90) + l_halfAngleScan;

	for (int l_angle = l_angleDepart; l_angle <= l_angleFin; l_angle += m_pasAngleScan)
	{
		l_threadList.emplace_back(&Robot::o_computeRay, std::ref(*this), l_angle);
	}

	for (auto& l_thread : l_threadList)
	{
		l_thread.join();
	}

}

Vector3& Robot::o_getPosition()
{
	return m_position;
}

float& Robot::o_getAngle()
{
	return m_angle;
}

void Robot::o_computeRay(int a_angle)
{
	float l_theta = DEG2RAD * a_angle;
	Ray l_ray;
	l_ray.position = m_position;
	l_ray.direction = Vector3{ cosf(l_theta), 0.f, sinf(l_theta) };
	RayCollision l_collision;
	l_collision.hit = false;
	l_collision.distance = HUGE_VALF;

	Vector2 l_robotOrientation{ cosf(m_angle) + m_position.x, sinf(m_angle) + m_position.z };
	l_robotOrientation = Vector2Normalize(l_robotOrientation);

	for (Obstacle& l_obstacle : *m_obstacleList)
	{
		if (l_obstacle.getObstacleType() == E_OBSTACLE_FLOOR) continue;

		const Vector3& l_obstaclePosition = l_obstacle.getPosition();
		const Vector2 l_produitVectoriel = Vector2Subtract(l_robotOrientation, Vector2Normalize(Vector2{ l_obstaclePosition.x, l_obstaclePosition.z }));

		//if (l_produitVectoriel.x * l_produitVectoriel.y > 0) continue;

		const Model& l_model = l_obstacle.o_getModel();

		const Matrix l_rotationMatrix = MatrixRotateY(DEG2RAD * static_cast<float>(l_obstacle.o_getRotation()));
		const Matrix l_translationMatrix = MatrixTranslate(l_obstacle.getPosition().x, l_obstacle.getPosition().y, l_obstacle.getPosition().z);
		const Matrix l_transformMatrix = MatrixMultiply(l_rotationMatrix, l_translationMatrix);
		const RayCollision l_tempRay = GetRayCollisionMesh(l_ray, l_model.meshes[0], l_transformMatrix);

		if (l_tempRay.hit && l_tempRay.distance < l_collision.distance)
		{
			l_collision = l_tempRay;
		}
	}

	if (l_collision.hit)
	{
		m_rayList.push_back(l_collision);
	}
}
