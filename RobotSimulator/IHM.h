#pragma once

#include "Map.h"

class IHM
{
public:
	IHM(Map* a_map, Camera* a_camera);

	void o_draw();

	void o_drawRenderTexture();

	void o_updateMiniMaps();
private:

	void o_listFileInDir(const char* a_path);

	Map* m_map;
	Camera* m_camera;
	Robot* m_robot;

	Image m_robotView;
	Texture m_robotViewTexture;

	Image m_progressionImage;
	Texture m_progressionTexture;

	RenderTexture m_renderTexture;
	Camera3D m_orthographicCamera;

	bool m_afficherListeObstacles;
	bool m_afficherEditeurCarte;
	bool m_afficherEditeurRobot;
	bool m_afficherFPS;

	bool m_afficherVueRobot;
	void o_afficherVueRobot();

	bool m_afficherProgression;
	void o_afficherProgression();
};

