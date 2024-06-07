# CMakeList.txt : CMake project for SDL3-Calc, include source and define
# project specific logic here.
#

# Add source to this project's executable.

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_executable (TtEL-SDL3_Notepad "TtEL-SDL3_Notepad.cpp" "TtEL-SDL3_Notepad.hpp" "DRAWING/TtEL-SDL3_Notepad-DRAW.h" "TtEL-SDL3_Notepad-MATH.h" "DRAWING/TextRenderer.h" "DRAWING/VectorTypes.h" "TtEL-SDL3_Notepad-LOGIC.h" "TtEL-SDL3_Notepad.rc" "globals.h" "DRAWING/NotepadTexture.h")


# COPY DLLs
	# SDL3
	add_custom_command(
	TARGET TtEL-SDL3_Notepad POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_BINARY_DIR}/SDL-main/SDL3.dll
			${CMAKE_CURRENT_BINARY_DIR}/SDL3.dll
	)
	# SDL3_ttf
	add_custom_command(
	TARGET TtEL-SDL3_Notepad POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_BINARY_DIR}/SDL_ttf-main/SDL3_ttf.dll
			${CMAKE_CURRENT_BINARY_DIR}/SDL3_ttf.dll
	)
	# SDL3_image
	add_custom_command(
	TARGET TtEL-SDL3_Notepad POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_BINARY_DIR}/SDL_image-main/SDL3_image.dll
			${CMAKE_CURRENT_BINARY_DIR}/SDL3_image.dll
	)



	message(STATUS "Building for Windows.")	
else()
	add_executable (TtEL-SDL3_Notepad "TtEL-SDL3_Notepad.cpp" "TtEL-SDL3_Notepad.hpp" "TtEL-SDL3_Notepad-DRAW.h" "TtEL-SDL3_Notepad-MATH.h" "DRAWING/TextRenderer.h" "DRAWING/VectorTypes.h" "TtEL-SDL3_Notepad-LOGIC.h" "globals.h" "DRAWING/NotepadTexture.h")
	message(AUTHOR_WARNING "Building for ${CMAKE_SYSTEM_NAME}. Please note that I have not tested other OSes, so please report any bugs on the GitHub.")
endif()

# COPY ASSETS
add_custom_command(
	TARGET TtEL-SDL3_Notepad POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/assets
			${CMAKE_CURRENT_BINARY_DIR}/assets
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TtEL-SDL3_Notepad PROPERTY CXX_STANDARD 20)
endif()

# Link to the actual SDL3 library.
#target_link_libraries(TtEL-SDL3_Notepad PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

#target_link_libraries(TtEL-SDL3_Notepad PRIVATE SDL3::SDL3)

target_link_libraries(TtEL-SDL3_Notepad PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image)