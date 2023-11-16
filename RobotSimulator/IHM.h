#pragma once
#include "Map.h"

class IHM
{
public:
	IHM(Map* a_map, Camera* a_camera);

	void o_draw();
private:

	void o_listFileInDir(const char* a_path);

	Map* m_map;
	Camera* m_camera;

	bool m_afficherListeObstacles;
	bool m_afficherEditeurCarte;
	bool m_afficherFPS;
};

