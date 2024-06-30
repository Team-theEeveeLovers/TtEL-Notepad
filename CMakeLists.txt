# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("TtEL SDL3 Notepad")

# Include sub-projects.
#

add_subdirectory("SDL-main")
# Find SDL3
#find_package(SDL3-shared)

add_subdirectory("SDL_ttf-main")

add_subdirectory("SDL_image-main")

add_subdirectory ("TtEL-NotepadSDL3")