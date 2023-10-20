#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include "obstacles.h"
#include "dialogs.h"

char* CreateObstacleJSON(Obstacle* obstacle);

void StartMapImport()
{
	
}

void StartMapExport(ObstacleList* environment)
{
	char* fileName = OpenExportDialog();
    FILE* file;
    file = fopen(fileName, "w");
    Obstacle test = (Obstacle){(Vector2){700, 100}, (Vector2){50, 50}, false, BLUE};
    char* text = CreateObstacleJSON(&test);
    fprintf(file, text);
    fclose(file);
}

char* CreateObstacleJSON(Obstacle* obstacle)
{
	cJSON* json_obstacle = cJSON_CreateObject();
    cJSON_AddNumberToObject(json_obstacle, "x", obstacle->position.x);
    cJSON_AddNumberToObject(json_obstacle, "y", obstacle->position.y);
    char* string;
    string = cJSON_Print(json_obstacle);
    return string;
}