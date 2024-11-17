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
#include "UTF8_Checking.h"
extern SDL_Window* main_window;
extern character* Dtext; // all the text in the 'document'
extern int textBufferSize; // the size of the textual screen buffer

string selectedFile;
bool fileDialogOpen = false;

enum TextEncoding {
    CODEPAGE_7bASCII = 0x49435341,
    CODEPAGE_8bWIN1252 = 0x32353231,
    CODEPAGE_UTF8 = 0x38465455
};

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
    SDL_IOStream* fileStream; // The IOStream of the file
    const char* fileName; // The name of the file
    TextEncoding encoding = TextEncoding::CODEPAGE_7bASCII; // The text encoding of the document

    // Unused space (due to 8 byte alignment)
    char padding[1] = { '\x20' };
    char magic[3] = { 'T', 'X', 'T'};
    


    /**
    * @brief Open a file dialog
    */
    void openFileDialog();
    /**
     * @brief Check if the file dialog is currently open
     * @return true, if the file dialog is open
     */
    inline bool isFileDialogOpen() {
        return fileDialogOpen;
    };
    /**
    * @brief get the currently selected file's path
    *
    * @return string, path to the currently selected file
    */
    string getSelectedFile();
    /**
    * @brief Load a file from a path into the TextFile class
    * @param filePath - (string) path to the file to load
    * @returns True if file loaded successfully, or false if file was not loaded successfully.
    */
    bool loadFile(string filePath);
    /**
     * @brief Closes the io stream of the file and frees variables
     */
    void closeFile();
};

/**
 * @brief Open a file dialog
 */
void TextFile::openFileDialog() {
    fileDialogOpen = true;
    SDL_ShowOpenFileDialog(&callback, NULL, main_window, filters, NULL, 0);

}

/**
* @brief get the currently selected file's path
* 
* @return string, path to the currently selected file
*/
string TextFile::getSelectedFile() {
    return selectedFile;
}

/**
* @brief Load a file from a path into the TextFile class
* @param filePath - (string) path to the file to load
* @returns True if file loaded successfully, or false if file was not loaded successfully.
*/
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

        // for some reason the std::filesystem::path.c_str() function
        // doesn't actually return a c_str so this code is here
        // it stupidly returns const std::filesytem::path::value_type*
        // Also it's convienient for safer printing
        fileNameString = fileNamePath.string();
        fileName = fileNameString.c_str();

        cout << "FileLoader: LOADING FILE '" << fileNameString << "' ";
        cout << "in: '" << FS_filePath.parent_path().string() << "'" << endl;

        fileStream = SDL_IOFromFile(temp_filePath, "r+");
        ASSERT(fileStream != NULL && "SDL couldn't open the file!" && SDL_GetError());

        // Delete pre-existing text from the screen text buffer
        for (int i = 0; i < textBufferSize; i++) {
            Dtext[i].freeCharacter();
        }
        Sint64 fileSize = SDL_GetIOSize(fileStream);

        cout << "FileLoader: SIZE IS: " << fileSize << " BYTES";

        // if the file is too big
        if (fileSize > MAX_FILESIZE) {
            fileSize = MAX_FILESIZE;
            cout << " (trimmed to " << MAX_FILESIZE << " bytes)" << endl;
        } // if the file's size is not a multiple of the padding number
        else if (fileSize < FILE_PAD) {
            fileSize = FILE_PAD;
            cout << " (padded to " << FILE_PAD << " bytes)" << endl;
        }
        else if (fileSize % FILE_PAD != 0) {
            // add the remainder to make the filesize a multiple of the padding number
            fileSize += FILE_PAD-(fileSize % FILE_PAD);
            cout << " (padded to " << fileSize << " bytes)" << endl;
        }
        else {
            cout << endl;
        }
        char currentChar = '\0';

        // resize the buffer only if the new size is different from the old size
        if (fileSize != textBufferSize) {
            // delete the old buffer
            delete[] Dtext;
            // remake the buffer with the wanted size
            Dtext = new character[fileSize];
            textBufferSize = fileSize; // update the size variable
        }

        for (int i = 0; i < fileSize; i++) {
            // if we have gone past the end of the file due to padding
            if (SDL_ReadIO(fileStream, &currentChar, sizeof(char)) == 0)
                currentChar = '\0';

            Dtext[i].loadChar(&currentChar);
            this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    }
    return success;
}

/**
 * @brief Closes the io stream of the file and frees variables
 */
void TextFile::closeFile() {
    SDL_CloseIO(fileStream);
#ifdef _NATIVE_NULLPTR_SUPPORTED
    fileName = nullptr;
#else
    fileName = NULL;
#endif
}