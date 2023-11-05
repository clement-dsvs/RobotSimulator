#include "dialogs.h"

#include <windows.h>

extern "C" {

	char* OpenExportDialog()
	{
		OPENFILENAMEA ofn;       // common dialog box structure
		char szFile[260];       // buffer for file name
		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "Fichier JSON\0*.json\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_DONTADDTORECENT | OFN_NOCHANGEDIR;

		// Display the Open dialog box. 
		if (GetSaveFileNameA(&ofn)==TRUE)
		{
			char* fileName = (char*) malloc(260 * sizeof(char));
			strcpy(fileName, szFile);
			return fileName;
		}
		return NULL;
	}

	char* OpenImportDialog()
	{
	    OPENFILENAMEA ofn;       // common dialog box structure
		char szFile[260];       // buffer for file name
		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "Fichier JSON\0*.json\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_DONTADDTORECENT | OFN_NOCHANGEDIR;

		// Display the Open dialog box. 
		if (GetOpenFileNameA(&ofn)==TRUE)
		{
			char* fileName = (char*) malloc(260 * sizeof(char));
			strcpy(fileName, szFile);
			return fileName;
		}
		return NULL;
	}
}