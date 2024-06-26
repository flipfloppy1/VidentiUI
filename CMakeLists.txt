﻿cmake_minimum_required (VERSION 3.25)
include(FetchContent)
include(ExternalProject)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("VidentiUI")

# Set properties
add_compile_definitions(GLFW_INCLUDE_NONE GL_GLEXT_PROTOTYPES)
set(BUILD_SHARED_LIBS OFF)
set_property(GLOBAL PROPERTY CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


FetchContent_Declare(
	json 
	GIT_REPOSITORY https://github.com/nlohmann/json.git
	GIT_TAG 9cca280a4d0ccf0c08f47a99aa71d1b0e52f8d03 # v3.11.3
)
FetchContent_Declare(
	angle
	URL https://github.com/google/angle/archive/4f23429a1f094d34b7878ba3cb3e3327f2bfa142.zip # latest as of 01/07/2024
)
FetchContent_Declare(
	glfw
	URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
)
FetchContent_Declare(
	stb
	GIT_REPOSITORY https://github.com/nothings/stb.git
	GIT_TAG 013ac3beddff3dbffafd5177e7972067cd2b5083 # latest as of 01/07/2024
)
FetchContent_MakeAvailable(json stb)

FetchContent_GetProperties(angle)
if (NOT angle_POPULATED AND VIDENTI_TESTS)
	FetchContent_Populate(angle)
endif()

FetchContent_GetProperties(glfw)
if (NOT glfw_POPULATED AND VIDENTI_TESTS)
	FetchContent_Populate(glfw)
endif()

# Add source
add_library (VidentiUI "src/VidentiUI.cpp" "src/VidentiUI.h" "src/VidentiParse.cpp" "src/VidentiParse.h" "src/VidentiRenderer.h"  "src/VidentiMath.h")
target_include_directories(VidentiUI PUBLIC ${json_SOURCE_DIR}/single_include)
set_property(TARGET VidentiUI PROPERTY CXX_STANDARD 20)

# For tests
if (VIDENTI_TESTS)
	set (VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/tests)
	add_executable(Tests "tests/src/main.cpp" "tests/src/main.h" "tests/src/Tests.cpp" "tests/src/Tests.h" "renderers/angle/VidentiAngleRenderer.cpp" "renderers/angle/VidentiAngleRenderer.h" "renderers/stb.cpp")

	target_include_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/src)
	target_include_directories(Tests PUBLIC ${angle_SOURCE_DIR}/include)
	target_include_directories(Tests PUBLIC ${stb_SOURCE_DIR})
	target_include_directories(Tests PUBLIC ${glfw_SOURCE_DIR}/include)
	target_include_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/renderers/angle)
	target_include_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/tests/include)
	
	if (WIN32)
		target_link_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/lib/windows)
	elseif (APPLE)
		target_link_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/lib/apple)
	elseif (LINUX)
		target_link_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/lib/linux)
	endif()
	
	target_link_libraries(Tests PUBLIC glfw3)
	target_link_libraries(Tests PUBLIC EGL)
	target_link_libraries(Tests PUBLIC GLESv2)
	target_link_libraries(Tests PUBLIC VidentiUI)

	set_property(TARGET Tests PROPERTY CXX_STANDARD 20)
endif()