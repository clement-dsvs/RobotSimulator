#include "Robot.h"

#include <raymath.h>
#include <thread>

#include "C_RayCollision.h"

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

	const float l_halfAngleScan = static_cast<float>(m_angleScan) / 2;
	const float l_angleDepart = (RAD2DEG * -m_angle + 90) - l_halfAngleScan;
	const float l_angleFin = (RAD2DEG * -m_angle + 90) + l_halfAngleScan;

	std::vector<std::thread> l_threadList;

	for (float l_angle = l_angleDepart; l_angle <= l_angleFin; l_angle += m_pasAngleScan)
	{
		l_threadList.emplace_back(&Robot::o_computeRay, std::ref(*this), l_angle);
	}

	for (auto& l_thread : l_threadList)
	{
		l_thread.join();
	}

	const float l_cosTheta = cosf(-m_angle+PI/2);
	const float l_sinTheta = sinf(-m_angle+PI/2);
	const Vector2 l_robotOrientation = {l_cosTheta, l_sinTheta};

	if (!m_rayList.empty())
	{
		const RayCollision* l_closestCollision = &m_rayList.front();

		for(const auto& l_ray : m_rayList)
		{
			if (l_ray.distance < l_closestCollision->distance)
			{
				l_closestCollision = &l_ray;
			}
		}

		if (l_closestCollision->distance < m_distanceMinCollision)
		{
			const Vector2 l_point = Vector2{l_closestCollision->point.x, l_closestCollision->point.z};
			const Vector2 l_robot = {l_robotOrientation.x + m_position.x, l_robotOrientation.y + m_position.z};

			const float l_prodVectoriel = (l_robot.x * l_point.x) - (l_robot.y * l_point.y);

			if (l_prodVectoriel > 0)
			{
				m_angle -= PI / 6;
			}
			else
			{
				m_angle += PI / 6;
			}

		}

	}

	// Move
	const Vector2 l_movement = Vector2Scale(Vector2{l_cosTheta, l_sinTheta}, m_velocity);
	m_position.x += l_movement.x;
	m_position.z += l_movement.y;

}

Vector3& Robot::o_getPosition()
{
	return m_position;
}

float& Robot::o_getAngle()
{
	return m_angle;
}

void Robot::o_computeRay(const float a_angle)
{
	const float l_theta = DEG2RAD * a_angle;
	Ray l_ray;
	l_ray.position = m_position;
	l_ray.direction = Vector3{ cosf(l_theta), 0.f, sinf(l_theta) };
	C_RayCollision l_collision;
	l_collision.hit = false;
	l_collision.distance = HUGE_VALF;

	//Vector2 l_robotOrientation{ cosf(m_angle) + m_position.x, sinf(m_angle) + m_position.z };
	//l_robotOrientation = Vector2Normalize(l_robotOrientation);

	for (Obstacle& l_obstacle : *m_obstacleList)
	{
		if (l_obstacle.getObstacleType() == E_OBSTACLE_FLOOR) continue;

		const Vector3& l_obstaclePosition = l_obstacle.getPosition();
		//const Vector2 l_produitVectoriel = Vector2Subtract(l_robotOrientation, Vector2Normalize(Vector2{ l_obstaclePosition.x, l_obstaclePosition.z }));

		//if (l_produitVectoriel.x * l_produitVectoriel.y > 0) continue;

		const Model& l_model = l_obstacle.o_getModel();

		const Matrix l_rotationMatrix = MatrixRotateY(DEG2RAD * static_cast<float>(l_obstacle.o_getRotation()));
		const Matrix l_translationMatrix = MatrixTranslate(l_obstaclePosition.x, l_obstaclePosition.y, l_obstaclePosition.z);
		const Matrix l_transformMatrix = MatrixMultiply(l_rotationMatrix, l_translationMatrix);
		C_RayCollision l_tempRay = static_cast<C_RayCollision>(GetRayCollisionMesh(l_ray, l_model.meshes[0], l_transformMatrix));

		if (l_tempRay.hit && l_tempRay.distance < l_collision.distance)
		{
			l_tempRay.m_obstacleCollided = &l_obstacle;
			l_collision = l_tempRay;
		}
	}

	if (l_collision.hit)
	{
		m_rayList.push_back(l_collision);
	}
}
