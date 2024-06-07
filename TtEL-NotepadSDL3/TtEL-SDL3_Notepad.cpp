#include "TtEL-SDL3_Notepad.hpp"

SDL_Window* main_window = NULL; // The main window
SDL_Renderer* main_renderer = NULL; // The main renderering context

int scr_wid = 480, scr_hei = 640; // screen size variable
float scr_floatwid = 480.f, scr_floathei = 640.f; // screen size variable but converted to float because using a lot of static_cast looks messy.
const int init__scr_wid = 480, init__scr_hei = 640; // inital screen size
float v_scale = 1.0f; // vertical scale
float h_scale = 1.0f; // horizontal scale
float common_scale = 1.0f; // common scale, good for preserving aspect ratio
bool isWindowMinimized = false; // is the window minimized?

// The font Noto Sans Math
TTF_Font* NotoMath = NULL;

character text[256];
character FileTab[4];


SDL_Texture* textTextures[256];
vector2_int textTextureSizeVectors[256];
vector2_int textTextureSourceVectors[256];

SDL_FRect fileTabBKG = { 15.f, 13.f, 90.f, 40.f };
Uint8 FileBKG_R = 0xD6, FileBKG_G = 0xDC, FileBKG_B = 0xDE;

SDL_FRect TextBKG = { 15.f, 56.f, 420.f, 560.f };
float RightTextMargin = 425.f;

#ifdef WINDOWS

HMODULE MainModuleHandle;
HRSRC Close_SRC;









#endif


SDL_Surface* arrowCur = NULL;
SDL_Cursor* arrow = NULL;

SDL_Surface* ibeamCur = NULL;
SDL_Cursor* ibeam = NULL;

void exit(void); // Define exit function so the code can be placed at the bottom of the file (makes sense for code order)
// and still be callable from the main function.
bool loadAssets();


bool init(void) {
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		success = false;
		cout << "SDL3 failed to initalize. SDL_error: " << SDL_GetError() << endl;
	}
	else {
		cout << "SDL3 initalized successfully." << endl;
		main_window = SDL_CreateWindow("TtEL SDL3 GUI Notepad", init__scr_wid, init__scr_hei, SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT);
		if (main_window == NULL) {
			success = false;
			cout << "SDL3 window creation failed. SDL_error: " << SDL_GetError() << endl;
		}
		else {
			cout << "Window created." << endl;
#ifdef WINDOWS
			MainModuleHandle = GetModuleHandleA("TtEL-SDL3_Notepad.exe");
			if (MainModuleHandle == NULL) {
				success = false;
				cout << "Could not get a module handle from Windows. " << endl;
			}

#endif
			SDL_SetWindowMinimumSize(main_window, init__scr_wid, init__scr_hei);
			main_renderer = SDL_CreateRenderer(main_window, NULL, SDL_RENDERER_PRESENTVSYNC);
			if (main_renderer == NULL) {
				success = false;
				cout << "SDL3 renderer creation failed. SDL_error: " << SDL_GetError() << endl;
			}
			else {
				cout << "Render started." << endl;
				SDL_SetRenderDrawBlendMode(main_renderer, SDL_BLENDMODE_BLEND);
				if (TTF_Init() == -1) {
					success = false;
					cout << "SDL3_ttf failed to initalize. SDL_ttf error: " << TTF_GetError() << endl;
				}
				else {
					cout << "SDL3_ttf initalized successfully." << endl;
					int imgFlags = IMG_INIT_PNG;
					if (!(IMG_Init(imgFlags) & imgFlags)) {
						cout << "SDL3_image failed to initalize. SDL_image error: " << IMG_GetError() << endl;
						success = false;
					}
					else {
						cout << "SDL3_image initalized successfully." << endl;
						if (!loadAssets()) {
							success = false;
							cout << "Couldn't load assets! Check for error above" << endl;
						}
						else {
							cout << "Loaded assets successfully." << endl;
						}
					}
				}
			}
		}
	}
	return success;
}

bool loadAssets() {
	NotoMath = TTF_OpenFont("assets/NotoSansMath-Regular.ttf", 48);
	if (NotoMath == NULL) {
		cout << "SDL3_ttf failed to load font. SDL_ttf error: " << TTF_GetError() << endl;
		return false;
	}
	else {
		cout << endl << "Initalizing Special Characters" << endl;
		for (int i = 0; i < 0xF; i++) {
			int currentspecChar = 0x20 + i;
			char specchar = static_cast<char>(currentspecChar);
			string numberString(1, specchar); // The random conversion to string helps with corrupted text somehow
			int width;
			int height;

			textTextures[currentspecChar] = loadTextureFromChar(NotoMath, specchar, { 0,0,0 }, &height, &width);
			textTextureSizeVectors[currentspecChar] = vector2_int(width, height);
			textTextureSourceVectors[currentspecChar] = vector2_int(0, 0);
			cout << specchar;
			//char specCharTable[1] = { specchar };
			//text[i].loadChar(&specCharTable[0], textTextures[currentspecChar], textTextureSizeVectors[currentspecChar]);

		}
		cout << endl << "Initalizing Numbers" << endl;
		for (int i = 0; i < 10; i++) {
			int currentNumber = 0x30 + i;
			char number = static_cast<char>(currentNumber);
			string numberString(1, number); // The random conversion to string helps with corrupted text somehow
			int width;
			int height;

			textTextures[currentNumber] = loadTextureFromChar(NotoMath, number, {0,0,0}, &height, &width);

			textTextureSizeVectors[currentNumber] = vector2_int(width, height);
			textTextureSourceVectors[currentNumber] = vector2_int(0,0);

			cout << number;
			char numberTable[1] = { number };
			text[i].loadChar(&numberTable[0], textTextures[currentNumber], textTextureSizeVectors[currentNumber]);
			
		}
		cout << endl << "Initalizing Letters" << endl;
		for (int i = 0; i < 26; i++) {
			
			int currentLetter = 0x41 + i;
			char letter = static_cast<char>(currentLetter);
			string letterString(1, letter); // The conversion to string helps with corrupted text
			int width;
			int height;

			textTextures[currentLetter] = loadTextureFromChar(NotoMath, letter, { 0,0,0 }, &height, &width);

			textTextureSizeVectors[currentLetter] = vector2_int(width, height);
			textTextureSourceVectors[currentLetter] = vector2_int(0, 0);

			cout << letter;
			char letterTable[1] = { letter };
			text[i+10].loadChar(&letterTable[0], textTextures[currentLetter], textTextureSizeVectors[currentLetter]);
		}
		cout << endl;
		for (int i = 0; i < 26; i++) {
			int currentLetter = 0x61 + i;
			char letter = static_cast<char>(currentLetter);
			string letterString(1, letter); // The conversion to string helps with corrupted text
			int width;
			int height;

			textTextures[currentLetter] = loadTextureFromChar(NotoMath, letter, { 0,0,0 }, &height, &width);

			textTextureSourceVectors[currentLetter] = vector2_int(0, 0);
			textTextureSizeVectors[currentLetter] = vector2_int(width, height);

			cout << letter;
			char letterTable[1] = { letter };
			text[i + 36].loadChar(&letterTable[0], textTextures[currentLetter], textTextureSizeVectors[currentLetter]);
		}
		cout << endl;
		char FileTabText[4] = { 'f', 'i', 'l', 'e' };

		FileTab[0] = loadCharFromChar(&FileTabText[0]);
		FileTab[0].x = 20.f;

		FileTab[1] = loadCharFromChar(&FileTabText[1]);
		FileTab[1].x = 40.f;

		FileTab[2] = loadCharFromChar(&FileTabText[2]);
		FileTab[2].x = 55.f;

		FileTab[3] = loadCharFromChar(&FileTabText[3]);
		FileTab[3].x = 68.f;

		arrowCur = IMG_Load("assets/cur/arrow.cur");
		if (arrowCur == NULL) {
			cout << "Failed to load cursor. SDL_image error: " << IMG_GetError() << endl;
			return false;
		}
		else {
			arrow = SDL_CreateColorCursor(arrowCur, 0, 0);
			ibeamCur = IMG_Load("assets/cur/i_beam.cur");
			if (ibeamCur == NULL) {
				cout << "Failed to load cursor. SDL_image error: " << IMG_GetError() << endl;
				return false;
			}
			else {
				ibeam = SDL_CreateColorCursor(ibeamCur, 16, 15);
			}
		}
		return true;
	}
}

int main(int argc, char *argv[]) {
	cout << "TtEL SDL3-based Notepad\nStartup..." << endl << endl;
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
		float mouseX = 0.f, mouseY = 0.f;
		bool mouseClicked = false;
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
				case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
					SDL_GetWindowSize(main_window, &scr_wid, &scr_hei); // get the new size of the window
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
					break;
				case SDL_EVENT_WINDOW_RESIZED:
					break;
					// Mouse Moved
				case SDL_EVENT_MOUSE_MOTION:
					SDL_GetMouseState(&mouseX, &mouseY);
					break;
					// Mouse button pressed
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
					mouseClicked = true;
					break;
					// Mouse button released
				case SDL_EVENT_MOUSE_BUTTON_UP:
					mouseClicked = false;
					break;
				case SDL_EVENT_KEY_DOWN:
					const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
					bool capital = false;
					if (currentKeyStates[SDL_SCANCODE_LSHIFT] || currentKeyStates[SDL_SCANCODE_RSHIFT]) {
						capital = true;
					}
					switch (e.key.keysym.sym) {	
					case SDLK_SPACE:
						for (int i = 0; i <= 256; i++) {
							if (text[i].letter[0] == '\0') {
								char letter = ' ';
								text[i] = loadCharFromChar(&letter);
								break;
							}
						}
						break;
					case SDLK_BACKSPACE:
						for (int i = 255; i >= 0; i--) {
							if (text[i].letter[0] != '\0') {
								text[i].letter[0] = '\0';
								break;
							}
						}
						break;
					case SDLK_a:
						for (int i = 0; i <= 256; i++) {
							if (text[i].letter[0] == '\0') {
								char letter;
								if (capital) {
									letter = 'A';
								}
								else {
									letter = 'a';
								}
								text[i] = loadCharFromChar(&letter);
								break;
							}
						}
						break;
					case SDLK_b:
						for (int i = 0; i <= 256; i++) {
							if (text[i].letter[0] == '\0') {
								char letter;
								if (capital) {
									letter = 'B';
								}
								else {
									letter = 'b';
								}
								text[i] = loadCharFromChar(&letter);
								break;
							}
						}
						break;
					case SDLK_c:
						for (int i = 0; i <= 256; i++) {
							if (text[i].letter[0] == '\0') {
								char letter;
								if (capital) {
									letter = 'C';
								}
								else {
									letter = 'c';
								}
								text[i] = loadCharFromChar(&letter);
								break;
							}
						}
						break;
					case SDLK_d:
						for (int i = 0; i <= 256; i++) {
							if (text[i].letter[0] == '\0') {
								char letter;
								if (capital) {
									letter = 'D';
								}
								else {
									letter = 'd';
								}
								text[i] = loadCharFromChar(&letter);
								break;
							}
						}
						break;
					}
					break;
				}

			}
			if (!exiting) {
				SDL_SetCursor(arrow);
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
				SDL_SetRenderDrawColor(main_renderer, FileBKG_R, FileBKG_G, FileBKG_B, SDL_ALPHA_OPAQUE-0x44);

				RD::FillFRectFromInputRect(fileTabBKG);
				FileTab[0].drawCharacter();
				FileTab[1].drawCharacter();
				FileTab[2].drawCharacter();
				FileTab[3].drawCharacter();


				if (isFMouseInFRectangle(mouseX, mouseY, &TextBKG)) {
					SDL_SetCursor(ibeam);
				}
				SDL_SetRenderDrawColor(main_renderer, 0xD6, 0xDC, 0xDE, SDL_ALPHA_OPAQUE-0x22);
				SDL_SetRenderScale(main_renderer, 1.f, 1.f);
				RD::FillFRectFromInputRect(TextBKG);
				for (int i = 0; i <= 255; i++) {
					if (text[i].letter[0] != '\0') {

						if (i == 0) {
							text[i].x = TextBKG.x + 10.f;
							text[i].y = TextBKG.y + 4.f;
						}
						else {
							text[i].x = (text[i - 1].x + text[i - 1].w) + 2.5f;
							text[i].y = text[i - 1].y;
						}

						
						if (text[i].x + text[i].w > RightTextMargin) {
							while (text[i].x + text[i].w > RightTextMargin) {
								text[i].x = text[i].x - (RightTextMargin - 20.f);
								text[i].y = text[i].y + 50.f;
							}
						}
						if (i > 0) {
							if (text[i - 1].y < text[i].y) {
								text[i].x = (TextBKG.x + 10.f);
							}
						}
						
						text[i].drawCharacter();
					}
				}
				SDL_SetRenderScale(main_renderer, common_scale, common_scale);
				SDL_RenderPresent(main_renderer);
			}
		}
		exit();
	}
	return 0;
}

void exit() {

	Uint64 startTime = SDL_GetTicks();
	Uint64 currentTime = SDL_GetTicks() - startTime;

	cout << endl << "Exiting..." << endl << endl;

	while (currentTime < 150) {
		currentTime = SDL_GetTicks() - startTime;
	}
	SDL_SetRenderDrawColor(main_renderer, 0xEB, 0xD7, 0x96, 0xFF);
	SDL_RenderClear(main_renderer);
	SDL_RenderPresent(main_renderer);

	currentTime = SDL_GetTicks() - startTime;

	cout << "Freeing Fonts..." << endl;
	TTF_CloseFont(NotoMath);
	NotoMath = NULL;
	cout << "Fonts freed." << endl << endl;

	while (currentTime < 200) {
		currentTime = SDL_GetTicks() - startTime;
	}
	SDL_SetRenderDrawColor(main_renderer, 0xDB, 0xC7, 0x86, 0xFF);
	SDL_RenderClear(main_renderer);
	SDL_RenderPresent(main_renderer);

	currentTime = SDL_GetTicks() - startTime;

	
	cout << "SDL_ttf is exiting..." << endl;
	TTF_Quit();
	cout << "SDL_ttf shutdown complete." << endl << endl;
	


	while (currentTime < 250) {
		currentTime = SDL_GetTicks() - startTime;
	}

	cout << "Freeing Assets..." << endl;
	for (int i = 0; i < 256; i++) {
		SDL_DestroyTexture(textTextures[i]);
		textTextures[i] = nullptr;
		textTextureSizeVectors[i].x = NULL;textTextureSizeVectors[i].y = NULL;
		textTextureSourceVectors[i].x = NULL; textTextureSourceVectors[i].y = NULL;
	}
	cout << "Assets freed." << endl << endl;

	cout << "SDL_image is exiting..." << endl;
	IMG_Quit();
	cout << "SDL_image shutdown complete." << endl << endl;

	cout << "Destroying renderer..." << endl;
	SDL_DestroyRenderer(main_renderer);
	main_renderer = NULL;
	cout << "Destroyed renderer!" << endl << endl;

	cout << "Destroying window..." << endl;
	SDL_DestroyWindow(main_window);
	main_window = NULL;
	cout << "Destroyed window!" << endl << endl;

	cout << "SDL is exiting..." << endl;
	SDL_Quit();
	cout << "SDL shutdown complete." << endl << endl;

	cout << "Goodbye." << endl;
}