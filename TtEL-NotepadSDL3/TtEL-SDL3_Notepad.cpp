#include "TtEL-SDL3_Notepad.hpp"

SDL_Window* main_window = NULL; // The main window
SDL_Renderer* main_renderer = NULL; // The main renderering context
SDL_WindowID main_windowID = NULL; // The main window's ID

SDL_Window* debug_window = NULL; // A secondary popup window for debug
SDL_Renderer* debug_window_RENDER = NULL; // a rendering context for the debug popup
SDL_Surface* debug_window_SURFACE = NULL; // a window surface for the debug popup


SDL_Renderer* unsynced_renderer = NULL; // a rendering context without VSync

int scr_wid = 480, scr_hei = 640; // screen size variable
float scr_floatwid = 480.f, scr_floathei = 640.f; // screen size variable but converted to float because using a lot of static_cast looks messy.
const int init__scr_wid = 480, init__scr_hei = 640; // inital screen size
float v_scale = 1.0f; // vertical scale
float h_scale = 1.0f; // horizontal scale
float common_scale = 1.0f; // common scale, good for preserving aspect ratio
bool isWindowMinimized = false; // is the window minimized?

// The font Noto Sans Math
TTF_Font* NotoMath = NULL;

character LOAD[4]; // the LOAD text
character text[256]; // all the text in the 'document'
character FileTab[4]; // the letters for the file menu button
character FileMenu[12]; // the letters for the options in the file menu

SDL_Texture* textTextures[256];
vector2_int textTextureSizeVectors[256];
vector2_int textTextureSourceVectors[256];

SDL_FRect TextBKG = { 15.f, 56.f, 420.f, 560.f };
float RightTextMargin = 425.f;


TextFile currentFile;


SDL_FRect fileTabBKG = { 15.f, 13.f, 90.f, 40.f };
Uint8 FileBKG_R = 0xD6, FileBKG_G = 0xDC, FileBKG_B = 0xDE;

SDL_FRect filetabOptionBKGs[4] = { {15.f, 53.f, 120.f, 40.f}, {15.f, 53.f+40.f, 120.f, 40.f}, {15.f, 53.f + 80.f, 120.f, 40.f}, {15.f, 53.f + 120.f, 120.f, 40.f} };

#ifdef WINDOWS

HMODULE MainModuleHandle;
HRSRC Close_SRC;

#endif

float Scroll = 0.0f;
bool NaturalScrolling = false;
const float ScrollUpperBound = 1600.f;

SDL_Surface* arrowCur = NULL;
SDL_Cursor* arrow = NULL;

SDL_Surface* ibeamCur = NULL;
SDL_Cursor* ibeam = NULL;



#ifdef DRAW_DEBUG
bool DRAW_DBG = true; // debug rectangles active?
bool INCREASE_SPACING = false; // increase the spacing between lines
#endif

float lineSpacing = 50.f;

void exit(void); // Define exit function so the code can be placed at the bottom of the file (makes sense for code order)
// and still be callable from the main function.
bool loadAssets();

// define "Initalizing" as a char for easier localization
constexpr auto INITALIZING = "Initalizing " ;



// clears the screen using the default background color 
int drawClear(void* data) {
	int returnVal = 0;
	returnVal = SDL_SetRenderDrawColor(main_renderer, 0xDB, 0xD7, 0xB6, 0x14);
	returnVal |= SDL_RenderClear(main_renderer);

	returnVal |= SDL_RenderPresent(main_renderer);
	return returnVal;
}


bool init(void) {
	bool success = true;

	cout << INITALIZING << "SDL3" << "..." << endl;
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		success = false;
		cout << "SDL3" << " failed to initalize. " << "SDL_error: " << SDL_GetError() << endl;
	}
	else {
		cout << "SDL3" << " initalized successfully." << endl << endl;
		cout << "Creating window" << "..." << endl;
		main_window = SDL_CreateWindow("TtEL SDL3 GUI Notepad", init__scr_wid, init__scr_hei, SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT);

		if (main_window == NULL) {
			success = false;
			// Store SDL_error here to avoid making too many API calls
			const char* SDL_ERR = SDL_GetError();
			//cout << "SDL3" << " window creation failed." << "SDL_error: " << SDL_ERR << endl;
			SDL_LogError(575, "SDL3 window creation failed. SDL_error: %s", SDL_ERR);
			ASSERT("Couldn't open window " && (main_window != NULL));
		}
		else {
			cout << "Window created." << endl << endl;
#ifdef WINDOWS
			MainModuleHandle = GetModuleHandleA("TtEL-SDL3_Notepad.exe");
			if (MainModuleHandle == NULL) {
				success = false;
				cout << "Could not get a module handle from Windows. " << endl;
			}

#endif
			main_windowID = SDL_GetWindowID(main_window);
			SDL_SetWindowMinimumSize(main_window, init__scr_wid, init__scr_hei);

			cout << "Starting render" << "..." << endl;
			main_renderer = SDL_CreateRenderer(main_window, NULL, SDL_RENDERER_PRESENTVSYNC);
			if (main_renderer == NULL) {
				success = false;
				cout << "SDL3" << " renderer creation failed. " << "SDL_error: " << SDL_GetError() << endl;
			}
			else {
				unsynced_renderer = SDL_CreateRenderer(main_window, NULL, SDL_RENDERER_ACCELERATED);
				cout << "Render started on: " << SDL_GetRendererName(main_renderer) << endl << endl;
				SDL_SetRenderDrawBlendMode(main_renderer, SDL_BLENDMODE_BLEND);
				SDL_SetRenderDrawBlendMode(unsynced_renderer, SDL_BLENDMODE_BLEND);

				SDL_SetRenderDrawColor(main_renderer, 0xDB, 0xD7, 0xB6, 0x14);
				SDL_RenderClear(main_renderer);

				SDL_RenderPresent(main_renderer);
				cout << INITALIZING << "SDL3_ttf" << "..." << endl;

				// Test opening popups
				debug_window = SDL_CreatePopupWindow(main_window, init__scr_wid - 322, init__scr_hei - 202, 320, 200, SDL_WINDOW_POPUP_MENU | SDL_WINDOW_TRANSPARENT);
				if (debug_window == NULL) {
					success = false;
					// Store SDL_error here to avoid making too many API calls
					const char* SDL_ERR = SDL_GetError();
					SDL_LogError(575, "SDL3 secondary window creation failed. SDL_error: %s", SDL_ERR);
					ASSERT("Couldn't open secondary window " && (debug_window != NULL) && SDL_ERR);
				}
				else {
					// Test done, window no longer needed
					SDL_DestroyWindow(debug_window);
					debug_window = NULL;

					if (TTF_Init() == -1) {
						success = false;
						cout << "SDL3_ttf" << " failed to initalize. " << "SDL_ttf" << " error: " << TTF_GetError() << endl;
					}
					else {
						SDL_SetRenderDrawColor(unsynced_renderer, 0xDB, 0xD7, 0xB6, 0x24);
						SDL_RenderClear(unsynced_renderer);

						SDL_RenderPresent(unsynced_renderer);
						cout << "SDL3_ttf" << " initalized successfully." << endl << endl;
						cout << INITALIZING << "SDL3_image" << "..." << endl;
						int imgFlags = IMG_INIT_PNG;
						if (!(IMG_Init(imgFlags) & imgFlags)) {
							cout << "SDL3_image" << " failed to initalize. " << "SDL_image" << " error: " << IMG_GetError() << endl;
							success = false;
						}
						else {
							SDL_SetRenderDrawColor(unsynced_renderer, 0xDB, 0xD7, 0xB6, 0x34);
							SDL_RenderClear(unsynced_renderer);
							SDL_FRect drawingRect;
							// loop through rectangles
							SDL_SetRenderDrawColor(unsynced_renderer, 0xD6, 0xDC, 0xDE, SDL_ALPHA_OPAQUE - 0x44);

							// we are subtracting the border size here from the screen size in the width value to prevent overdraw from right rect
							drawingRect = { 0.f, 0.f, scr_floatwid - 10.f, 10.f };
							SDL_RenderFillRect(unsynced_renderer, &drawingRect);
							// we are subtracting the border size here from the screen size in the height value to prevent overdraw from bottom rect
							drawingRect = { 0.f, 0.f, 10.f, scr_floathei - 10.f };
							SDL_RenderFillRect(unsynced_renderer, &drawingRect);

							SDL_SetRenderDrawColor(unsynced_renderer, 0xAD, 0xAF, 0xA4, SDL_ALPHA_OPAQUE - 0x44);

							drawingRect = { scr_floatwid - 10.f, 0.f, 10.f, scr_floathei }; // Right Rect
							SDL_RenderFillRect(unsynced_renderer, &drawingRect);
							drawingRect = { 0.f, scr_floathei - 10.f, scr_floatwid, 10.f }; // Bottom Rect
							SDL_RenderFillRect(unsynced_renderer, &drawingRect);

							SDL_RenderPresent(unsynced_renderer);
							cout << "SDL3_image" << " initalized successfully." << endl << endl;
							cout << "Loading assets" << "..." << endl;
							if (!loadAssets()) {
								success = false;
								cout << "Couldn't load assets! Check for error above" << endl;
							}
							else {
								cout << "Loaded assets successfully." << endl << endl;
							}
						}
					}
				}
			}
		}
	}
	return success;
}

bool loadAssets() {
	cout << "Loading font..." << endl;
	NotoMath = TTF_OpenFont("assets/NotoSansMath-Regular.ttf", 48);
	if (NotoMath == NULL) {
		cout << "SDL3_ttf" << " failed to load font. " << "SDL_ttf" << " error: " << TTF_GetError() << endl;
		return false;
	}
	else {
		SDL_SetRenderDrawColor(unsynced_renderer, 0xDB, 0xD7, 0xB6, 0x44);
		SDL_RenderClear(unsynced_renderer);
		// loop through rectangles
		SDL_SetRenderDrawColor(unsynced_renderer, 0xD6, 0xDC, 0xDE, SDL_ALPHA_OPAQUE - 0x44);
		SDL_FRect drawingRect;
		// we are subtracting the border size here from the screen size in the width value to prevent overdraw from right rect
		drawingRect = { 0.f, 0.f, scr_floatwid - 10.f, 10.f };
		SDL_RenderFillRect(unsynced_renderer, &drawingRect);
		// we are subtracting the border size here from the screen size in the height value to prevent overdraw from bottom rect
		drawingRect = { 0.f, 0.f, 10.f, scr_floathei - 10.f };
		SDL_RenderFillRect(unsynced_renderer, &drawingRect);

		SDL_SetRenderDrawColor(unsynced_renderer, 0xAD, 0xAF, 0xA4, SDL_ALPHA_OPAQUE - 0x44);

		drawingRect = { scr_floatwid - 10.f, 0.f, 10.f, scr_floathei }; // Right Rect
		SDL_RenderFillRect(unsynced_renderer, &drawingRect);
		drawingRect = { 0.f, scr_floathei - 10.f, scr_floatwid, 10.f }; // Bottom Rect
		SDL_RenderFillRect(unsynced_renderer, &drawingRect);

		SDL_SetRenderDrawColor(unsynced_renderer, FileBKG_R, FileBKG_G, FileBKG_B, SDL_ALPHA_OPAQUE - 0x44);

		SDL_RenderFillRect(unsynced_renderer, &fileTabBKG);
		SDL_RenderFillRect(unsynced_renderer, &TextBKG);

		SDL_RenderPresent(unsynced_renderer);


		cout << endl << INITALIZING << "replacement character" << "...";

		int width;
		int height;
		textTextures[0] = loadTextureFromUnicodeCodepoint(NotoMath, 0xFFFD, { 0,0,0 }, &height, &width);
		textTextureSizeVectors[0] = vector2_int(width, height);
		textTextureSourceVectors[0] = vector2_int();



		cout << " Done!"; 

		cout << endl << INITALIZING << "Special Characters" << endl;
		
		cout << "Line breaks" << "...";
		int currentlbChar = 0x0A;
		char LBchar = static_cast<char>(currentlbChar);
		string numberString(1, LBchar); // The random conversion to string helps with corrupted text somehow

		textTextures[currentlbChar] = loadTextureFromChar(NotoMath, LBchar, { 0,0,0 }, &height, &width);
		textTextureSizeVectors[currentlbChar] = vector2_int(width, height);
		textTextureSourceVectors[currentlbChar] = vector2_int();

		currentlbChar = 0x0D;
		LBchar = static_cast<char>(currentlbChar);
		string numberString2(1, LBchar); // The random conversion to string helps with corrupted text somehow

		textTextures[currentlbChar] = loadTextureFromChar(NotoMath, LBchar, { 0,0,0 }, &height, &width);
		textTextureSizeVectors[currentlbChar] = vector2_int(width, height);
		textTextureSourceVectors[currentlbChar] = vector2_int();

		// FREE STUFF
		width = NULL;
		height = NULL;

		cout << " Done!" << endl;

		for (int i = 0; i <= 0xF; i++) {

			int currentspecChar = 0x20 + i;
			char specChar = static_cast<char>(currentspecChar);
			string numberString(1, specChar); // The random conversion to string helps with corrupted text somehow
			int width;
			int height;

			textTextures[currentspecChar] = loadTextureFromChar(NotoMath, specChar, { 0,0,0 }, &height, &width);
			textTextureSizeVectors[currentspecChar] = vector2_int(width, height);
			textTextureSourceVectors[currentspecChar] = vector2_int();

			cout << specChar;
			//char specCharTable[1] = { specChar };
			text[i].loadChar(&specChar);

		}
		for (int i = 0; i < 7; i++) {
			int currentspecChar = 0x3A + i;
			char specChar = static_cast<char>(currentspecChar);
			string numberString(1, specChar); // The random conversion to string helps with corrupted text somehow
			int width;
			int height;

			textTextures[currentspecChar] = loadTextureFromChar(NotoMath, specChar, { 0,0,0 }, &height, &width);
			textTextureSizeVectors[currentspecChar] = vector2_int(width, height);
			textTextureSourceVectors[currentspecChar] = vector2_int();

			cout << specChar;
			//char specCharTable[1] = { specChar };
			text[i + 16].loadChar(&specChar);
		}
		for (int i = 0; i < 6; i++) {
			int currentspecChar = 0x5B + i;
			char specChar = static_cast<char>(currentspecChar);
			string numberString(1, specChar); // The random conversion to string helps with corrupted text somehow
			int width;
			int height;

			textTextures[currentspecChar] = loadTextureFromChar(NotoMath, specChar, { 0,0,0 }, &height, &width);
			textTextureSizeVectors[currentspecChar] = vector2_int(width, height);
			textTextureSourceVectors[currentspecChar] = vector2_int();

			cout << specChar;
			//char specCharTable[1] = { specChar };
			text[i + 23].loadChar(&specChar);
		}

		cout << endl << INITALIZING << "Numbers" << endl;
		for (int i = 0; i < 10; i++) {

			int currentNumber = 0x30 + i;
			char number = static_cast<char>(currentNumber);
			string numberString(1, number); // The random conversion to string helps with corrupted text somehow
			int width;
			int height;

			textTextures[currentNumber] = loadTextureFromChar(NotoMath, number, {0,0,0}, &height, &width);
			SDL_SetTextureAlphaModFloat(textTextures[currentNumber], 0.85f);

			textTextureSizeVectors[currentNumber] = vector2_int(width, height);
			textTextureSourceVectors[currentNumber] = vector2_int();

			cout << number;
			//char numberTable[1] = { number };
			text[i+29].loadChar(&number);

			
		}

		SDL_SetRenderDrawColor(main_renderer, 0xDB, 0xD7, 0xB6, 0x44);
		SDL_RenderClear(main_renderer);

		// loop through rectangles
		SDL_SetRenderDrawColor(main_renderer, 0xD6, 0xDC, 0xDE, SDL_ALPHA_OPAQUE - 0x44);

		// we are subtracting the border size here from the screen size in the width value to prevent overdraw from right rect
		RD::FillFRectFromInput(0.f, 0.f, scr_floatwid - 10.f, 10.f);
		// we are subtracting the border size here from the screen size in the height value to prevent overdraw from bottom rect
		RD::FillFRectFromInput(0.f, 0.f, 10.f, scr_floathei - 10.f);

		SDL_SetRenderDrawColor(main_renderer, 0xAD, 0xAF, 0xA4, SDL_ALPHA_OPAQUE - 0x44);

		RD::FillFRectFromInput(scr_floatwid - 10.f, 0.f, 10.f, scr_floathei); // Right Rect
		RD::FillFRectFromInput(0.f, scr_floathei - 10.f, scr_floatwid, 10.f); // Bottom Rect

		cout << endl << INITALIZING << "Letters" << endl;
		for (int i = 0; i < 26; i++) {
			
			int currentLetter = 0x41 + i;
			char letter = static_cast<char>(currentLetter);
			string letterString(1, letter); // The conversion to string helps with corrupted text
			int width;
			int height;

			textTextures[currentLetter] = loadTextureFromChar(NotoMath, letter, { 0,0,0 }, &height, &width);
			SDL_SetTextureAlphaModFloat(textTextures[currentLetter], 0.85f);

			textTextureSizeVectors[currentLetter] = vector2_int(width, height);
			textTextureSourceVectors[currentLetter] = vector2_int();

			cout << letter;
			//char letterTable[1] = { letter };
			text[i+39].loadChar(&letter);
		}
		for (int i = 0; i < 4; i++) {
			char LOADtable[4] = { 'L', 'O', 'A', 'D' };
			LOAD[i] = loadCharFromChar(&LOADtable[i], vector2_float(25.f * static_cast<float>(i), 20.f));

			LOAD[i].drawCharacter();
		}
		cout << endl;
		SDL_RenderPresent(main_renderer);

		for (int i = 0; i < 26; i++) {
			int currentLetter = 0x61 + i;
			char letter = static_cast<char>(currentLetter);
			string letterString(1, letter); // The conversion to string helps with corrupted text
			int width;
			int height;

			textTextures[currentLetter] = loadTextureFromChar(NotoMath, letter, { 0,0,0 }, &height, &width);
			SDL_SetTextureAlphaModFloat(textTextures[currentLetter], 0.85f);

			textTextureSourceVectors[currentLetter] = vector2_int();
			textTextureSizeVectors[currentLetter] = vector2_int(width, height);

			cout << letter;
			//char letterTable[1] = { letter };
			text[i + 65].loadChar(&letter);
		}
		cout << endl;

		SDL_SetRenderDrawColor(unsynced_renderer, 0xDB, 0xD7, 0xB6, 0x44);
		SDL_RenderClear(unsynced_renderer);
		// loop through rectangles
		SDL_SetRenderDrawColor(unsynced_renderer, 0xD6, 0xDC, 0xDE, SDL_ALPHA_OPAQUE - 0x44);
		// we are subtracting the border size here from the screen size in the width value to prevent overdraw from right rect
		drawingRect = { 0.f, 0.f, scr_floatwid - 10.f, 10.f };
		SDL_RenderFillRect(unsynced_renderer, &drawingRect);
		// we are subtracting the border size here from the screen size in the height value to prevent overdraw from bottom rect
		drawingRect = { 0.f, 0.f, 10.f, scr_floathei - 10.f };
		SDL_RenderFillRect(unsynced_renderer, &drawingRect);

		SDL_SetRenderDrawColor(unsynced_renderer, 0xAD, 0xAF, 0xA4, SDL_ALPHA_OPAQUE - 0x44);

		drawingRect = { scr_floatwid - 10.f, 0.f, 10.f, scr_floathei }; // Right Rect
		SDL_RenderFillRect(unsynced_renderer, &drawingRect);
		drawingRect = { 0.f, scr_floathei - 10.f, scr_floatwid, 10.f }; // Bottom Rect
		SDL_RenderFillRect(unsynced_renderer, &drawingRect);

		SDL_SetRenderDrawColor(unsynced_renderer, FileBKG_R, FileBKG_G, FileBKG_B, SDL_ALPHA_OPAQUE - 0x44);

		SDL_RenderFillRect(unsynced_renderer, &fileTabBKG);


		SDL_RenderPresent(unsynced_renderer);

		char FileTabText[4] = { 'F', 'i', 'l', 'e' };

		FileTab[0] = loadCharFromChar(&FileTabText[0], vector2_float(20.f, 0.f));
		LOAD[0].destroyCharacter();

		FileTab[1] = loadCharFromChar(&FileTabText[1], vector2_float(44.f, 0.f));
		LOAD[1].destroyCharacter();

		FileTab[2] = loadCharFromChar(&FileTabText[2], vector2_float(55.f, 0.f));
		LOAD[2].destroyCharacter();

		FileTab[3] = loadCharFromChar(&FileTabText[3], vector2_float(68.f, 0.f));
		LOAD[3].destroyCharacter();


		char FileMenuText[12] = 
		{ 
			'O','p','e','n', 
			'S','a','v','e', 
			'E','x','i','t' 
		};
		float filetabOptionTextXPositions[12] = 
		{ 
			20.f, 53.f, 78.f, 102.f, 
			20.f, 42.f, 65.f, 84.f, 
			20.f, 44.f, 64.f, 76.f 
		};
		for (int i = 0; i < 12; i++) {
			int h = floorf((static_cast<float>(i) / 4.f));
			FileMenu[i] = loadCharFromChar(&FileMenuText[i], vector2_float(filetabOptionTextXPositions[i], filetabOptionBKGs[h].y-14.f));
		}

		arrowCur = IMG_Load("assets/cur/arrow.cur");
		if (arrowCur == NULL) {
			cout << "Failed to load cursor. SDL_image error: " << IMG_GetError() << endl;
			return false;
		}
		else {
			ibeamCur = IMG_Load("assets/cur/i_beam.cur");
			if (ibeamCur == NULL) {
				cout << "Failed to load cursor. SDL_image error: " << IMG_GetError() << endl;
				return false;
			}
			else {
				arrow = SDL_CreateColorCursor(arrowCur, 0, 0);
				ibeam = SDL_CreateColorCursor(ibeamCur, 16, 14);

				// destroy temporary surfaces
				SDL_DestroySurface(arrowCur);
				SDL_DestroySurface(ibeamCur);
			}
		}
		return true;
	}
}

int main(int argc, char *argv[]) {
	cout << "TtEL SDL3-based Notepad\nStartup" << "..." << endl << endl;
	if (!init()) {
		cout << "Failed to start." << endl;
		return 575;
	}
	else {
		cout << "Startup successful." << endl << endl;
		SDL_Event e;
		bool exiting = false;
		float BorderSize = 10.0f;
		SDL_FRect currentDrawRect = { 0,0,scr_floatwid,scr_floathei };
		float mouseX = 0.f, mouseY = 0.f; // 
		float popupMouseX = 14314941448192.f, popupMouseY = 14245685100544.f; // Mouse positioning variables
		bool mouseClicked = false;
		bool fileMenuOpen = false;
		float fileMenuY_Offset = 40.f;
		
		bool fileDialogOpen = false;
		while (!exiting) {
			while (SDL_PollEvent(&e)) {
				switch (e.type) {
				case SDL_EVENT_QUIT:
					exiting = true;
					break;
				case SDL_EVENT_WINDOW_SHOWN:
					isWindowMinimized = false;
					break;
				case SDL_EVENT_WINDOW_HIDDEN:
					isWindowMinimized = true;
					break;
				case SDL_EVENT_WINDOW_MINIMIZED:
					isWindowMinimized = true;
					break;
				case SDL_EVENT_WINDOW_MAXIMIZED:
					isWindowMinimized = false;
					break;
				case SDL_EVENT_WINDOW_RESTORED:
					isWindowMinimized = false;
					SDL_SetWindowInputFocus(main_window);
					break;
				case SDL_EVENT_WILL_ENTER_BACKGROUND:
					SDL_SetRenderDrawColor(main_renderer, 0xDB, 0xD7, 0xB6, 0x14);
					SDL_RenderClear(main_renderer);

					SDL_RenderPresent(main_renderer);
					break;
				case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
					// Make sure the resize event is not a dud from the popup window
					if (e.window.windowID == main_windowID) {
						SDL_SetRenderDrawColor(main_renderer, 0xDB, 0xD7, 0xB6, 0x14);
						SDL_RenderClear(main_renderer);
						// get the new size of the window
						scr_wid = e.window.data1, scr_hei = e.window.data2; //SDL_GetWindowSize(main_window, &scr_wid, &scr_hei); 

						scr_floatwid = static_cast<float>(scr_wid); // convert to float
						scr_floathei = static_cast<float>(scr_hei); // convert to float

						// set scale values for proper scaling
						v_scale = scr_floathei / static_cast<float>(init__scr_hei);
						h_scale = scr_floatwid / static_cast<float>(init__scr_wid);
						common_scale = ME::LowestFloat(v_scale, h_scale);

						/// Scaling things					

						// Border scaling
						BorderSize = 10.f * common_scale;
						// Text Background Scaling
						TextBKG.x = 15.f * common_scale;
						TextBKG.w = scr_floatwid - 60.f;
						RightTextMargin = TextBKG.x + TextBKG.w - 10.f;

						TextBKG.y = 56.f * common_scale;
						TextBKG.h = scr_floathei - TextBKG.y - 24.f;



						SDL_RenderPresent(main_renderer);
					}
					break;
				case SDL_EVENT_WINDOW_RESIZED:
					break;
					// Mouse Moved
				case SDL_EVENT_MOUSE_MOTION:
					// Handle mouse event if it didn't happen in the popup.
					if (e.window.windowID == main_windowID) {
						mouseX = e.motion.x;
						mouseY = e.motion.y;
					}
					// We are assuming any other window is the popup as we don't open any other windows (except file dialog but that isn't owned by us)
					else {
						popupMouseX = e.motion.x;
						popupMouseY = e.motion.y;
					}
					//SDL_GetMouseState(&mouseX, &mouseY);
					break;
					// Mouse button pressed
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
					mouseClicked = true;
					if (isFMouseInFRectangle(mouseX, mouseY, &fileTabBKG)) {
						fileMenuOpen = !fileMenuOpen;
					}
					break;
					// Mouse button released
				case SDL_EVENT_MOUSE_BUTTON_UP:
					mouseClicked = false;
					break;
				case SDL_EVENT_MOUSE_WHEEL:
					if (NaturalScrolling) {
						Scroll += 4.f * e.wheel.y;
					}
					else {
						Scroll -= 4.f * e.wheel.y;
					}
					// Keep the scroll value from being negative
					if (Scroll < 0.f) {
						Scroll = 0.f;
					}
					else if (Scroll > ScrollUpperBound) {
						Scroll = ScrollUpperBound;
					}
					break;
				case SDL_EVENT_KEY_DOWN:
					const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
					bool capital = false;
					if (currentKeyStates[SDL_SCANCODE_LSHIFT] || currentKeyStates[SDL_SCANCODE_RSHIFT]) {
						capital = true;
					}
					switch (e.key.keysym.sym) {	
#ifdef DRAW_DEBUG
					case SDLK_F1:
						DRAW_DBG = !DRAW_DBG;
						break;
					case SDLK_F2:
						INCREASE_SPACING = !INCREASE_SPACING;
						break;
#endif
					case SDLK_SPACE:
						// Iterate through screen buffer to find unpopulated space 
						for (int i = 0; i <= 256; i++) {
							// Is empty space
							if (text[i].isEmptyChar()) {
								char space = ' '; // make space char
								text[i] = loadCharFromChar(&space); // add space to the buffer
								break; // Leave this loop
							}
						}
						break;
					case SDLK_BACKSPACE:
						// Iterate through screen buffer to find populated space 
						for (int i = 255; i >= 0; i--) {
							// Is filled space
							if (text[i].isFilledChar()) {
								text[i].letter[0] = '\0'; // Fill area with empty space
								break; // Leave this loop
							}
						}
						break;
					case SDLK_DOWN:
						Scroll += 20.f;
						if (Scroll > ScrollUpperBound) {
							Scroll = ScrollUpperBound;
						}
						break;
					case SDLK_UP:
						// Keep the scroll value from being or becoming negative
						// It is possible for the scroll value to be too high causing float imprecision
						// Which will make the scroll value not a multiple of 20
						if (Scroll >= 20.f) {
							Scroll -= 20.f;
						}
						else {
							Scroll = 0.f; 
						}
						break;
					default:
						handleKey(e.key.keysym.sym, capital);
						break;
					}
					break;
				}

			}
			if (!exiting) {
				if (isFMouseInFRectangle(mouseX, mouseY, &TextBKG)) {
					if (fileMenuOpen) {
						bool FileMenuHover = false;
						for (int i = 0; i < 4; i++) {
							if (isFMouseInFRectangle(mouseX, mouseY, &filetabOptionBKGs[i])) {
								FileMenuHover = true;
							}
						}
						if (FileMenuHover) {
							SDL_SetCursor(arrow);
						}
						else {
							SDL_SetCursor(ibeam);
						}
					}
					else {
						SDL_SetCursor(ibeam);
					}
				}
				else {
					SDL_SetCursor(arrow);
				}
				SDL_SetRenderDrawColor(main_renderer, 0xDB, 0xD7, 0xB6, 0x44);
				SDL_RenderClear(main_renderer);

				SDL_SetRenderScale(main_renderer, 1.f, 1.f);

				// loop through rectangles
				SDL_SetRenderDrawColor(main_renderer, 0xD6, 0xDC, 0xDE, SDL_ALPHA_OPAQUE-0x44);

				// we are subtracting the border size here from the screen size in the width value to prevent overdraw from right rect
				RD::FillFRectFromInput(0.f, 0.f, scr_floatwid - BorderSize, BorderSize);
				// we are subtracting the border size here from the screen size in the height value to prevent overdraw from bottom rect
				RD::FillFRectFromInput(0.f, 0.f, BorderSize, scr_floathei - BorderSize);

				SDL_SetRenderDrawColor(main_renderer, 0xAD, 0xAF, 0xA4, SDL_ALPHA_OPAQUE-0x44);

				RD::FillFRectFromInput(scr_floatwid - BorderSize, 0.f, BorderSize, scr_floathei); // Right Rect
				RD::FillFRectFromInput(0.f, scr_floathei - BorderSize, scr_floatwid, BorderSize); // Bottom Rect



				SDL_SetRenderScale(main_renderer, common_scale, common_scale);



				if (isFMouseInFRectangle(mouseX, mouseY, &fileTabBKG)) {
					if (FileBKG_R > 0xAC) {
						FileBKG_R--;
					}
					if (FileBKG_G > 0xAE) {
						FileBKG_G -= 2;
					}
					if (FileBKG_B > 0xA4) {
						FileBKG_B--;
					}
				}
				else {
					if (FileBKG_R < 0xD6) {
						FileBKG_R += 2;
					}
					if (FileBKG_G < 0xDC) {
						FileBKG_G += 4;
					}
					if (FileBKG_B < 0xDE) {
						FileBKG_B += 2;
					}
				}
				if (fileMenuY_Offset == 40.f || fileMenuY_Offset <= 0.f) {
					SDL_SetRenderDrawColor(main_renderer, FileBKG_R, FileBKG_G, FileBKG_B, SDL_ALPHA_OPAQUE - 0x44);

					RD::FillFRectFromInputRect(fileTabBKG);
					for (int i = 0; i < 4; i++) {
						FileTab[i].drawCharacter();
					}

				}

#ifdef DRAW_DEBUG 
				if (INCREASE_SPACING)
					lineSpacing = 70.f;
				else
					lineSpacing = 50.f;
#endif

				SDL_SetRenderDrawColor(main_renderer, 0xD6, 0xDC, 0xDE, SDL_ALPHA_OPAQUE-0x22);
				SDL_SetRenderScale(main_renderer, 1.f, 1.f);
				RD::FillFRectFromInputRect(TextBKG);
				for (int i = 0; i <= 255; i++) {
					if (text[i].isFilledChar()) {

						if (i == 0) {
							text[i].x = TextBKG.x + 10.f;
							text[i].y = TextBKG.y + 14.f;
						}
						else {
							text[i].x = (text[i - 1].x + text[i - 1].w) + 0.3125f;
							text[i].y = text[i - 1].y;
						}

						
						if (text[i].x + text[i].w > RightTextMargin) {
							while (text[i].x + text[i].w > RightTextMargin) {
								text[i].x = text[i].x - (RightTextMargin - 20.f);
								text[i].y = text[i].y + lineSpacing;
							}
						}
						if (i > 0) {
							if (text[i - 1].y < text[i].y) {
								text[i].x = (TextBKG.x + 10.f);
							}
						}
						// If the text got scrolled above the textbox (with added 20.f because of the empty space)
						if (text[i].y - Scroll + 20.f < TextBKG.y) {
							// how far above are we?
							float above = text[i].y + Scroll;
							float below = text[i].y - Scroll;
							// If we are completely above
							if (text[i].y - Scroll + text[i].h + 20.f < TextBKG.y) {
								// allan please add code
							}
							else {
								text[i].drawCharacter(vector2_float(0.f, 0.f - Scroll + below), vector4_float(0.f, 0.f-((above/3.5f)), 0.f, 0.f-below + text[i].y), vector2_float(0.f, 0.f - below));

								// keep this for scrolling text from below the textbox
								// float above = text[i].y - Scroll;
								// text[i].drawCharacter(vector2_float(0.f, 0.f - Scroll), vector4_float(0.f, 0.f-(above/2.f), 0.f, 0.f-above), vector2_float(0.f, 0.f-above));
								
							}
						}
						else {
							text[i].drawCharacter(vector2_float(0.f, 0.f - Scroll));
						}
					}
				}
				SDL_SetRenderScale(main_renderer, common_scale, common_scale);

				if (fileMenuOpen) {
					if (debug_window == NULL) {
						debug_window = SDL_CreatePopupWindow(main_window, init__scr_wid - 230, init__scr_hei + 10, 320, 200, SDL_WINDOW_POPUP_MENU | SDL_WINDOW_TRANSPARENT);
						if (debug_window == NULL) {
							// Store SDL_error here to avoid making too many API calls
							const char* SDL_ERR = SDL_GetError();
							SDL_LogError(575, "SDL3 secondary window creation failed. SDL_error: %s", SDL_ERR);
							ASSERT("Couldn't open secondary window " && !(debug_window == NULL) && SDL_ERR);
						}
						debug_window_SURFACE = SDL_GetWindowSurface(debug_window);
						if (debug_window_SURFACE == NULL) {
							// Store SDL_error here to avoid making too many API calls
							const char* SDL_ERR = SDL_GetError();
							SDL_LogError(575, "SDL3 secondary window surface getting failed. SDL_error: %s", SDL_ERR);
							ASSERT("Couldn't get window surface of secondary window" && !(debug_window_SURFACE == NULL) && SDL_ERR);
						}
						SDL_FillSurfaceRect(debug_window_SURFACE, NULL, SDL_MapRGBA(debug_window_SURFACE->format, 0x33, 0x33, 0x33, 0x33));

						SDL_UpdateWindowSurface(debug_window);

						debug_window_RENDER = SDL_CreateSoftwareRenderer(debug_window_SURFACE);
						if (debug_window_RENDER == NULL) {
							// Store SDL_error here to avoid making too many API calls
							const char* SDL_ERR = SDL_GetError();
							SDL_LogError(575, "SDL3 secondary window renderer creation failed. SDL_error: %s", SDL_ERR);
							ASSERT("Couldn't create secondary window renderer. " && !(debug_window_RENDER == NULL) && SDL_ERR);
						}
					}
					else {
						SDL_FillSurfaceRect(debug_window_SURFACE, NULL, SDL_MapRGBA(debug_window_SURFACE->format, 0x33, 0x33, 0x33, 0x33));
						SDL_SetRenderDrawColor(debug_window_RENDER, 0xF0, 0xF0, 0xF0, 0x33);
						SDL_RenderClear(debug_window_RENDER);
						
						SDL_RenderPresent(debug_window_RENDER);
						SDL_UpdateWindowSurface(debug_window);
					}
					if (fileMenuY_Offset < 0.f) {
						fileMenuY_Offset = 0.f;
					}
					else if (fileMenuY_Offset > 0.f) {
						if (fileMenuY_Offset <= 10.f) {
							fileMenuY_Offset -= 1.f;
							if (fileMenuY_Offset <= 0.f) {
								SDL_SetRenderDrawColor(main_renderer, FileBKG_R, FileBKG_G, FileBKG_B, SDL_ALPHA_OPAQUE - 0x44);

								RD::FillFRectFromInputRect(fileTabBKG);
								FileTab[0].drawCharacter();
								FileTab[1].drawCharacter();
								FileTab[2].drawCharacter();
								FileTab[3].drawCharacter();
							}
						}
						else if (fileMenuY_Offset <= 25.f) {
							fileMenuY_Offset -= 2.f;
						}
						else {
							fileMenuY_Offset -= 4.f;
						}
					}
					for (int i = 0; i < 4; i++) {
						if (ME::isIntEven(i)) {
							SDL_SetRenderDrawColor(main_renderer, 0xAA, 0xAC, 0xA2, SDL_ALPHA_OPAQUE - 0x44);
						}
						else {
							SDL_SetRenderDrawColor(main_renderer, 0xAD, 0xAF, 0xA5, SDL_ALPHA_OPAQUE - 0x44);
						}
						if (fileMenuY_Offset > 0.f) {
							RD::FillFRectFromInput(
								filetabOptionBKGs[i].x,
								filetabOptionBKGs[i].y - fileMenuY_Offset, 
								filetabOptionBKGs[i].w, 
								filetabOptionBKGs[i].h
							);
							for (int j = 0; j < 3; j++) {
								int h = j + (i * 3);
								FileMenu[h].drawCharacter(vector2_float(0.f, 0.f-fileMenuY_Offset));
							}
#ifdef DRAW_DEBUG
							if (DRAW_DBG) {
								SDL_SetRenderDrawColor(main_renderer, 0xFF, 0x00, 0x00, 0xAA);
								RD::StrokeFRectFromInput(
									filetabOptionBKGs[i].x, 
									filetabOptionBKGs[i].y - fileMenuY_Offset, 
									filetabOptionBKGs[i].w, 
									filetabOptionBKGs[i].h
								);
							}
#endif
							if (fileMenuY_Offset != 40.f && i == 0) {
								SDL_SetRenderDrawColor(main_renderer, FileBKG_R, FileBKG_G, FileBKG_B, SDL_ALPHA_OPAQUE - 0x44);

								RD::FillFRectFromInputRect(fileTabBKG);
								FileTab[0].drawCharacter();
								FileTab[1].drawCharacter();
								FileTab[2].drawCharacter();
								FileTab[3].drawCharacter();
							}
						}
						else {
							if (isFMouseInFRectangle(mouseX, mouseY, &filetabOptionBKGs[i])) { 
								if (mouseClicked) {
									SDL_SetRenderDrawColor(main_renderer, 0x98, 0x9C, 0x90, SDL_ALPHA_OPAQUE - 0x24);
									switch (i) {
									case 0:
										if (currentFile.isFileDialogOpen()) {
											cout << "Main: FILE DIALOG ALREADY OPEN" << endl;
										}
										else {
											cout << "Main: Opening file dialog..." << endl;
											currentFile.openFileDialog();
										}
										break;
									case 1:
										break;
									case 2:
										exiting = true;
										break;
									case 3:
										break;
									}
								}
								else {
									SDL_SetRenderDrawColor(main_renderer, 0xA8, 0xAC, 0xA0, SDL_ALPHA_OPAQUE - 0x24);
								}
							}
							RD::FillFRectFromInputRect(filetabOptionBKGs[i]);
#ifdef DRAW_DEBUG
							if (DRAW_DBG) {
								SDL_SetRenderDrawColor(main_renderer, 0xFF, 0x00, 0x00, 0xAA);
								RD::StrokeFRectFromInputRect(filetabOptionBKGs[i]);
							}
#endif
							for (int j = 0; j < 3; j++) {
								int h = j + (i * 3);
								FileMenu[h].drawCharacter();
							}
						}
					}
				}
				else {
					fileMenuY_Offset = 40.f;
					if (debug_window != NULL) {
						SDL_DestroyWindow(debug_window); 
						debug_window = NULL;

						// Capture the mouse outside the window position since the popup is positioned outside the main window but is now closed
						SDL_CaptureMouse(SDL_TRUE); // By default SDL only captures within a window
						SDL_GetMouseState(&mouseX, &mouseY); // Get the mouse cursor position since popup windows have their own mouse positioning
						SDL_CaptureMouse(SDL_FALSE); // We don't need this feature anymore
					}
				}
				if (currentFile.isFileDialogOpen()) {
					fileDialogOpen = true;
				}
				else {
					if (fileDialogOpen) {
						// make temporary variable 
						string currentFilepath = currentFile.getSelectedFile();
						fileDialogOpen = false;
						if (currentFilepath == "INVALID") {
							cout << "No file selected." << endl;
						}
						else {
							if (!currentFile.loadFile(currentFilepath)) {
								cout << "FAILED TO LOAD FILE" << endl;
							}
							else {
								fileMenuOpen = false;
								Scroll = 0.0f; // Reset scroll value
							}
						}
					}
				}
				SDL_RenderPresent(main_renderer);
			}
		}
		exit();
	}
	return 0;
}

void exit() {
	SDL_CaptureMouse(SDL_TRUE); // Capture outside window
	SDL_GetMouseState(&Scroll, &lineSpacing); // Get the mouse state because SDL_Quit() may trigger a READ_ACCESS_VIOLATION with mouse state; We are using these seemingly unrelated variables as storage because they are no longer needed and it saves memory to not have to define a new one.

	
	Uint64 startTime = SDL_GetTicks();
	Uint64 currentTime = SDL_GetTicks() - startTime;

	cout << endl << "Exiting..." << endl << endl;

	startTime = SDL_GetTicks();
	currentTime = SDL_GetTicks() - startTime;
	while (currentTime < 150) {
		currentTime = SDL_GetTicks() - startTime;
	}
	SDL_SetWindowBordered(main_window, SDL_FALSE); // Remove the window border for the fadeout animation
	SDL_SetRenderDrawColor(main_renderer, 0xEB, 0xD7, 0x96, 0x66);
	SDL_RenderClear(main_renderer);
	SDL_RenderPresent(main_renderer);

	currentTime = SDL_GetTicks() - startTime;

	cout << "Freeing Fonts..." << endl;
	TTF_CloseFont(NotoMath);
	NotoMath = NULL;
	cout << "Fonts freed." << endl << endl;

	startTime = SDL_GetTicks();
	currentTime = SDL_GetTicks() - startTime;
	while (currentTime < 150) {
		currentTime = SDL_GetTicks() - startTime;
	}
	SDL_SetRenderDrawColor(main_renderer, 0xDB, 0xC7, 0x86, 0x56);
	SDL_RenderClear(main_renderer);
	SDL_RenderPresent(main_renderer);

	currentTime = SDL_GetTicks() - startTime;

	
	cout << "SDL_ttf" << " is exiting..." << endl;
	TTF_Quit();
	cout << "SDL_ttf" << " shutdown complete." << endl << endl;
	


	while (currentTime < 250) {
		currentTime = SDL_GetTicks() - startTime;
	}
	SDL_SetRenderDrawColor(main_renderer, 0xCB, 0xB7, 0x76, 0x46);
	SDL_RenderClear(main_renderer);
	SDL_RenderPresent(main_renderer);

	currentTime = SDL_GetTicks() - startTime;
	cout << "Freeing Assets..." << endl;
	for (int i = 0; i < 256; i++) {
		SDL_DestroyTexture(textTextures[i]);
		textTextures[i] = nullptr;
		textTextureSizeVectors[i].x = NULL;textTextureSizeVectors[i].y = NULL;
		textTextureSourceVectors[i].x = NULL; textTextureSourceVectors[i].y = NULL;
	}
	cout << "Assets freed." << endl << endl;

	while (currentTime < 450) {
		currentTime = SDL_GetTicks() - startTime;
	}
	SDL_SetRenderDrawColor(main_renderer, 0xBB, 0xA7, 0x66, 0x36);
	SDL_RenderClear(main_renderer);
	SDL_RenderPresent(main_renderer);

	currentTime = SDL_GetTicks() - startTime;
	cout << "Clearing text..." << endl;
	for (int i = 0; i < 256; i++) {
		text[i].freeCharacter();
	}
	cout << "Text cleared." << endl << endl;
	while (currentTime < 550) {
		currentTime = SDL_GetTicks() - startTime;
	}

	SDL_SetRenderDrawColor(main_renderer, 0xAB, 0x97, 0x56, 0x26);
	SDL_RenderClear(main_renderer);
	SDL_RenderPresent(main_renderer);

	currentTime = SDL_GetTicks() - startTime;
	cout << "Closing file..." << endl;
	currentFile.closeFile();
	cout << "File closed." << endl << endl;

	
	while (currentTime < 650) {
		currentTime = SDL_GetTicks() - startTime;
	}

	SDL_SetRenderDrawColor(main_renderer, 0x9B, 0x87, 0x46, 0x16);
	SDL_RenderClear(main_renderer);
	SDL_RenderPresent(main_renderer);

	currentTime = SDL_GetTicks() - startTime;
	cout << "SDL_image" << " is exiting..." << endl;
	IMG_Quit();
	cout << "SDL_image" << " shutdown complete." << endl << endl;

	while (currentTime < 750) {
		currentTime = SDL_GetTicks() - startTime;
	}
	SDL_SetRenderDrawColor(main_renderer, 0x9B, 0x87, 0x46, 0x06);
	SDL_RenderClear(main_renderer);
	SDL_RenderPresent(main_renderer);

	currentTime = SDL_GetTicks() - startTime;
	cout << "Destroying renderer..." << endl;
	SDL_DestroyRenderer(main_renderer);
	main_renderer = NULL;
	cout << "Destroyed renderer!" << endl << endl;
	currentTime = SDL_GetTicks() - startTime;

	while (currentTime < 850) {
		currentTime = SDL_GetTicks() - startTime;
	}
	SDL_CaptureMouse(SDL_FALSE); // We don't need this anymore
	SDL_GetMouseState(&Scroll, &lineSpacing); // Get the mouse state because SDL_Quit() may trigger a READ_ACCESS_VIOLATION with mouse state; We are using these seemingly unrelated variables as storage because they are no longer needed and it saves memory to not have to define a new one.

	// Quit the video subsystem so the events subsystem doesn't get quit by SDL_Quit() causing the error
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	// Disable the mouse event so it doesn't get processed triggering an READ_ACCESS_VIOLATION when SDL Quits
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	cout << "Destroying window..." << endl;
	if (debug_window != NULL) {
		// prevent READ_ACCESS_VIOLATION
		try {
			// USING THIS FUNKY MESS
			if ((&debug_window) == NULL) {
				throw("ERR");
			}
			else {
				SDL_DestroyRenderer(debug_window_RENDER);

				debug_window_RENDER = NULL;

				SDL_DestroyWindow(*&*&debug_window); // USING THIS FUNKY MESS

				debug_window = NULL;
			}
		}
		catch (int ERR) {
			DEBUG_BREAK();
		}

	}
	SDL_DestroyWindow(main_window);
	main_window = NULL;

	cout << "Destroyed window!" << endl << endl;

	
	cout << "SDL" << " is exiting..." << endl;
	SDL_Quit();
	cout << "SDL" << " shutdown complete." << endl << endl;

	cout << "Goodbye." << endl;
}