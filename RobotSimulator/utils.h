#pragma once

#include <cjson/cJSON.h>

cJSON* getJSONChild(const cJSON* a_parentElement, const char* a_childName);
