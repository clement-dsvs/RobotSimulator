#include "Utils.h"

#include <cstring>

cJSON* getJSONChild(const cJSON* a_parentElement, const char* a_childName)
{
	cJSON* l_firstChild = a_parentElement->child;

	if (strcmp(l_firstChild->string, a_childName) == 0)
	{
		return  l_firstChild;
	}

	cJSON* l_nextChild = l_firstChild->next;
	while (l_nextChild != nullptr)
	{
		if (strcmp(l_nextChild->string, a_childName) == 0)
		{
			return l_nextChild;
		}
		l_nextChild = l_nextChild->next;
	}

	return nullptr;
}
