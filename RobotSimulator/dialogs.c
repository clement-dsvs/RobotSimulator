#include "dialogs.h"

#include <windows.h>
#include <string.h>

char* OpenExportDialog()
{
	OPENFILENAME ofn;
	char szFile[260] = { 0 };
	char* string = malloc(260 * sizeof(char));

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFile = szFile;
    ofn.hwndOwner = NULL;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"Fichier JSON\0*.json\0";
    ofn.lpstrTitle = L"Sauvegarder Carte";
    ofn.Flags = OFN_DONTADDTORECENT | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn) == TRUE) {
        // The selected file path will be in szFile
        MessageBox(NULL, szFile, L"Selected File", MB_OK);
        wcstombs(string, ofn.lpstrFile, 260);
		return string;
    } else {
        MessageBoxA(NULL, "File selection canceled", "Info", MB_OK);
    }
}

char* OpenImportDialog()
{
	OPENFILENAME ofn;
	char szFile[260] = { 0 };

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = szFile;
	ofn.hwndOwner = NULL;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Fichier JSON\0*.json\0";
	ofn.lpstrTitle = L"Select a File";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE) {
	    // The selected file path will be in szFile
	    MessageBoxA(NULL, szFile, "Selected File", MB_OK);
	} else {
	    MessageBoxA(NULL, "File selection canceled", "Info", MB_OK);
	}
}