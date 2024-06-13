#pragma once
/**
* @file TtEL-SDL3_Notepad-FILELOADING.h
* @brief A header file of the TtEL SDL3 GUI Notepad, containing file loading functions
* @author Team theEeveeLovers Sean
* @date June 8th, 2024
*/

#include <SDL.h>
#include <filesystem>
#include "../DRAWING/TextRenderer.h"
extern SDL_Window* main_window;
extern character text[256]; // all the text in the 'document'

string selectedFile;
bool fileDialogOpen = false;

static const SDL_DialogFileFilter filters[] = {
    { "Text documents",  "txt" },
    { "All files",   "*" },
    { NULL, NULL }
};

static void SDLCALL callback(void* userdata, const char* const* filelist, int filter)
{
    if (!filelist) {
        SDL_Log("An error occured: %s", SDL_GetError());
        fileDialogOpen = false;
        selectedFile = "INVALID";
        return;
    }
    else if (!*filelist) {
        SDL_Log("The user did not select any file.");
        SDL_Log("Most likely, the dialog was canceled.");
        fileDialogOpen = false;
        selectedFile = "INVALID";
        return;
    }

    while (*filelist) {
        SDL_Log("Full path to selected file: '%s'", *filelist);
        selectedFile = *filelist;
        fileDialogOpen = false;
        filelist++;
    }

    if (filter == -1) {
        SDL_Log("The current platform does not support fetching "
            "the selected filter.");
    }
    else if (filter < SDL_arraysize(filters)) {
        SDL_Log("The filter selected by the user is '%s' (%s).",
            filters[filter].pattern, filters[filter].name);
    }
    else {
        SDL_Log("The user did not select any filter.");
    }
}

class TextFile {
public:
    SDL_IOStream* fileStream;
    const char* fileName;
    char File_text[256];
    void openFileDialog();
    inline bool isFileDialogOpen() {
        return fileDialogOpen;
    };
    string getSelectedFile();
    bool loadFile(string filePath);
};
/**
 * @brief Open a file dialog
 */
void TextFile::openFileDialog() {
    fileDialogOpen = true;
    SDL_ShowOpenFileDialog(&callback, NULL, main_window, filters, NULL, 0);

}
string TextFile::getSelectedFile() {
    return selectedFile;
}
bool TextFile::loadFile(string filePath) {
    bool success = true;
    if (filePath == "INVALID") {
        success = false;
    }
    else {
        const char* temp_filePath = filePath.c_str();
        std::filesystem::path FS_filePath = filePath;
        std::filesystem::path fileNamePath = FS_filePath.filename();
        string fileNameString;
        // for some reason the std::filesystem::path .c_str() function
        // doesn't actually return a c_str so this code is here
        // it stupidly returns const std::filesytem::path::value_type*
        // Also it's convienient for safer printing
        fileNameString = fileNamePath.string();
        fileName = fileNameString.c_str();
        cout << "FileLoader: LOADING FILE '" << fileNameString << "' ";
        cout << "in: '" << FS_filePath.parent_path().string() << "'" << endl;
        fileStream = SDL_IOFromFile(temp_filePath, "r+");
        for (int i = 0; i < 256; i++) {
            text[i].freeCharacter();
        }
        Sint64 fileSize = SDL_GetIOSize(fileStream);
        cout << "FileLoader: SIZE IS: " << fileSize << " BYTES" << endl;

        if (fileSize > 256) {
            fileSize = 256;
        }
        for (int i = 0; i < fileSize; i++) {
            SDL_ReadIO(fileStream, &File_text[i], sizeof(char));
            text[i].loadChar(&File_text[i]);
        }
    }
    return success;
}