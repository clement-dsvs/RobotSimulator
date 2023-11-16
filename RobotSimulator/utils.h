#pragma once

#include <string>
#include <cjson/cJSON.h>

cJSON* getJSONChild(const cJSON* a_parentElement, std::string a_childName);
