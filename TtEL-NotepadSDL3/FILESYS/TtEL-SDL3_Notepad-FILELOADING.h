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

const char* selectedFile;

static const SDL_DialogFileFilter filters[] = {
    { "Text documents",  "txt" },
    { "All files",   "*" },
    { NULL, NULL }
};

static void SDLCALL callback(void* userdata, const char* const* filelist, int filter)
{
    if (!filelist) {
        SDL_Log("An error occured: %s", SDL_GetError());
        return;
    }
    else if (!*filelist) {
        SDL_Log("The user did not select any file.");
        SDL_Log("Most likely, the dialog was canceled.");
        return;
    }

    while (*filelist) {
        SDL_Log("Full path to selected file: '%s'", *filelist);
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
    selectedFile = *filelist;
}

class TextFile {
public:
    SDL_IOStream* fileStream;
    const char* fileName;
    char File_text[256];
    const char openFileDialog();
    bool loadFile(const char* filePath);
};
const char TextFile::openFileDialog() {
    SDL_ShowOpenFileDialog(&callback, NULL, main_window, filters, NULL, 0);
    return *selectedFile;
}
bool TextFile::loadFile(const char* filePath) {
    std::filesystem::path FS_filePath = filePath;
    std::filesystem::path fileNamePath = FS_filePath.filename();
    string fileNameString;
    // for some reason the std::filesystem::path .c_str() function
    // doesn't actually return a c_str so this code is here
    // it stupidly returns const std::filesytem::path::value_type*
    fileNameString = fileNamePath.string();
    fileName = fileNameString.c_str(); 

    fileStream = SDL_IOFromFile(filePath, "r+");
    for (int i = 0; i < 256; i++) {
        text[i].freeCharacter();
    }
    Sint64 fileSize = SDL_GetIOSize(fileStream);

    if (fileSize > 256) {
        fileSize = 256;
    }
    for (int i = 0; i < fileSize; i++) {
        SDL_ReadIO(fileStream, &text[i], sizeof(char));
    }
}