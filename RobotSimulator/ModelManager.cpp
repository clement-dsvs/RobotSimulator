#include "ModelManager.h"

ModelManager* ModelManager::ms_Instance = nullptr;

ModelManager* ModelManager::os_GetInstance()
{
	if (!ms_Instance)
	{
		ms_Instance = new ModelManager;
	}

	return ms_Instance;

}

const Model& ModelManager::o_GetModel(const std::string a_FileName)
{
	const auto l_iterator = m_ModelList.find(a_FileName);

	if (l_iterator != m_ModelList.end())
	{
		return l_iterator->second;
	}

	Model l_model = LoadModel(a_FileName.c_str());
	m_ModelList.emplace(a_FileName, l_model);

	return l_model;
}

ModelManager::ModelManager() = default;
