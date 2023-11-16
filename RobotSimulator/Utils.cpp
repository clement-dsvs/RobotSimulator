#include "Utils.h"

#include <cstring>
#include <xstring>

cJSON* getJSONChild(const cJSON* a_parentElement, std::string a_childName)
{
	if (a_parentElement == nullptr) return nullptr;

	cJSON* l_firstChild = a_parentElement->child;
	std::string l_ChildName = l_firstChild->string;

	if (l_ChildName == a_childName)
	{
		return  l_firstChild;
	}

	cJSON* l_nextChild = l_firstChild->next;
	while (l_nextChild != nullptr)
	{
		l_ChildName = l_nextChild->string;
		if (l_ChildName == a_childName)
		{
			return l_nextChild;
		}
		l_nextChild = l_nextChild->next;
	}

	return nullptr;
}
