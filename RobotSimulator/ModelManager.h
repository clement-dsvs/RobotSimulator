#pragma once
#include <map>
#include <raylib.h>
#include <string>

class ModelManager
{
public:
	static ModelManager* os_GetInstance();

	const Model& o_GetModel(std::string a_FileName);

private:
	ModelManager();

	static ModelManager* ms_Instance;

	std::map<std::string, Model> m_ModelList;

};

