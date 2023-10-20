#ifndef RAYGUI_IMPLEMENTATION
#define RAYGUI_IMPLEMENTATION
#endif

#include "menu.h"
#include "map.h"

#include <raylib.h>
#include <raygui.h>


void DrawMenu(Menu* menu)
{
	if (menu->dropDownEditMode) GuiLock();
	GuiWindowBox((Rectangle){menu->anchor01.x + 0, menu->anchor01.y + 0, 200, 832}, "Menu");
	if (menu->menuActive)
	{
		//menu->menuActive = !GuiWindowBox((Rectangle){ menu->anchor01.x + 0, menu->anchor01.y + 0, 200, 832 }, "Menu");
		//GuiStatusBar((Rectangle){ menu->anchor01.x + 8, menu->anchor01.y + 800, 184, 24 }, "SAMPLE TEXT");
	}
	GuiGroupBox((Rectangle){menu->anchor02.x + 0, menu->anchor02.y + 0, 184, 168}, "Carte");
	if (GuiButton((Rectangle){menu->anchor02.x + 8, menu->anchor02.y + 48, 168, 32}, "Importer"))
	{
		// Importer Carte
		StartMapImport();
	}
	GuiLabel((Rectangle){menu->anchor02.x + 8, menu->anchor02.y + 16, 120, 24}, "Nom : Untitled.json *");
	*menu->edit_mode = GuiCheckBox((Rectangle){menu->anchor02.x + 8, menu->anchor02.y + 88, 24, 24}, "Mode edition",
	                               *menu->edit_mode);
	if (GuiButton((Rectangle){menu->anchor02.x + 8, menu->anchor02.y + 120, 168, 32}, "Sauvegarder"))
	{
		// Sauvegarder Carte
		StartMapExport(menu->environment);
	}
	GuiGroupBox((Rectangle){menu->anchor03.x + 0, menu->anchor03.y + 0, 184, 216}, "Obstacles");
	GuiGroupBox((Rectangle){menu->anchor04.x + 0, menu->anchor04.y + 24, 184, 344}, "Robot");
	GuiLabel((Rectangle){menu->anchor03.x + 8, menu->anchor03.y + 8, 120, 24}, "Ajouter :");
	if (GuiButton((Rectangle){menu->anchor03.x + 8, menu->anchor03.y + 72, 168, 32}, "Vider l'environnement"))
	{
		// Clear env
	}
	GuiLine((Rectangle){menu->anchor03.x + 8, menu->anchor03.y + 112, 168, 16}, "Obstacle Selectionne");
	//if (GuiSpinner((Rectangle){ menu->anchor03.x + 72, menu->anchor03.y + 144, 104, 32 }, "Hauteur", &menu->selected_obstacle->size.y, 0, 100, menu->spinnerHeightEditMode)) menu->spinnerHeightEditMode = !menu->spinnerHeightEditMode;
	//if (GuiSpinner((Rectangle){ menu->anchor03.x + 72, menu->anchor03.y + 176, 104, 32 }, "Largeur", &menu->selected_obstacle->size.x, 0, 100, menu->spinnerWidthEditMode)) menu->spinnerWidthEditMode = !menu->spinnerWidthEditMode;
	if (GuiSpinner((Rectangle){menu->anchor04.x + 8, menu->anchor04.y + 64, 168, 32}, NULL, &menu->robot->view_distance,
	               0, 200, menu->spinnerViewDistanceEditMode)) menu->spinnerViewDistanceEditMode = !menu->
		spinnerViewDistanceEditMode;
	GuiLabel((Rectangle){menu->anchor04.x + 8, menu->anchor04.y + 40, 120, 24}, "Distance de vue");
	GuiSlider((Rectangle){menu->anchor04.x + 8, menu->anchor04.y + 128, 168, 16}, NULL, NULL, menu->robot->angle_max, 0,
	          100);
	GuiLabel((Rectangle){menu->anchor04.x + 8, menu->anchor04.y + 104, 120, 24}, "Perimetre de mesure");
	if (GuiSpinner((Rectangle){menu->anchor04.x + 80, menu->anchor04.y + 152, 96, 32}, "Pas ",
	               &menu->robot->orientation, 0, 360,
	               menu->spinnerStepEditMode)) menu->spinnerStepEditMode = !menu->spinnerStepEditMode;
	menu->robot->color = GuiColorPicker((Rectangle){menu->anchor04.x + 8, menu->anchor04.y + 216, 144, 144}, NULL,
	                                    menu->robot->color);
	GuiLabel((Rectangle){menu->anchor04.x + 8, menu->anchor04.y + 192, 120, 24}, "Couleur");
	if (GuiDropdownBox((Rectangle){menu->anchor03.x + 8, menu->anchor03.y + 32, 168, 32}, "Rectangle;Cercle;Triangle",
	                   &menu->selected_shape, menu->dropDownEditMode))
	{
		menu->dropDownEditMode = !menu->dropDownEditMode;
		if (!menu->dropDownEditMode)
		{
			// Add obstacle
			printf("clicked\n");
		}
	}

	GuiUnlock();
}
