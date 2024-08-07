﻿cmake_minimum_required (VERSION 3.22)
include(FetchContent)
include(ExternalProject)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("VidentiUI")

message("VidentiUI: Setting build properties...")
add_compile_definitions(GL_GLEXT_PROTOTYPES GLFW_INCLUDE_NONE)
set(BUILD_SHARED_LIBS OFF)
set_property(GLOBAL PROPERTY CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("VidentiUI: Set build properties")

message("VidentiUI: Adding dependencies...")
if (VIDENTI_TESTS)
	FetchContent_Declare(
		angle
		URL https://github.com/google/angle/archive/4f23429a1f094d34b7878ba3cb3e3327f2bfa142.zip # latest as of 01/07/2024
	)

	if (WIN32)
		FetchContent_Declare(
			glfwsrc
			URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
		)
	else()
		FetchContent_Declare(
			glfwsrc
			GIT_REPOSITORY https://github.com/glfw/glfw.git
			GIT_TAG 7b6aead9fb88b3623e3b3725ebb42670cbe4c579 # glfw 3.4, has to be compiled from source on linux
		)
	endif()
endif()

FetchContent_Declare(
	freetype
	GIT_REPOSITORY https://github.com/freetype/freetype.git
	GIT_TAG 920c5502cc3ddda88f6c7d85ee834ac611bb11cc # version 2.13.2
)

FetchContent_Declare(
	stb
	GIT_REPOSITORY https://github.com/nothings/stb.git
	GIT_TAG 013ac3beddff3dbffafd5177e7972067cd2b5083 # latest as of 01/07/2024
)

if (WIN32)
FetchContent_Declare(
	luajit
	URL https://github.com/LuaJIT/LuaJIT/archive/refs/tags/v2.0.5.zip
	BUILD_COMMAND ""
)
else()
FetchContent_Declare(
	luajit
	URL https://github.com/LuaJIT/LuaJIT/archive/refs/tags/v2.0.5.zip
	BUILD_COMMAND ""
)
endif()

FetchContent_GetProperties(angle)
if (NOT angle_POPULATED AND VIDENTI_TESTS)
	FetchContent_Populate(angle)
endif()

FetchContent_GetProperties(glfwsrc)
if (NOT glfwsrc_POPULATED AND VIDENTI_TESTS)
	FetchContent_Populate(glfwsrc)
endif()
message("VidentiUI: Added dependencies")

message("VidentiUI: Fetching/compiling dependencies...")

FetchContent_MakeAvailable(luajit freetype stb)

if (WIN32 AND NOT (EXISTS "${luajit_SOURCE_DIR}/src/lua51.lib"))
	execute_process(COMMAND cmd.exe /c "cd ${luajit_SOURCE_DIR}/src/ && msvcbuild.bat static") # this assumes visual studio build tools are available, as msvcbuild.bat requires it
	if (NOT (EXISTS "${luajit_SOURCE_DIR}/src/lua51.lib"))
		message("LuaJIT is missing or did not compile correctly, make sure visual studio is able to use developer command prompt")
	endif()
elseif(NOT WIN32 AND NOT (EXISTS "${luajit_SOURCE_DIR}/src/libluajit.a"))
	execute_process(
	COMMAND bash -c "cd ${luajit_SOURCE_DIR}/src/ && make")
	if (NOT (EXISTS "${luajit_SOURCE_DIR}/src/libluajit.a"))
		message("LuaJIT is missing or did not compile correctly")
	endif()
endif()


add_library(luajitlib STATIC IMPORTED GLOBAL)
if (WIN32)    
set_property(TARGET luajitlib PROPERTY IMPORTED_LOCATION ${luajit_SOURCE_DIR}/src/lua51.lib)
else()
set_property(TARGET luajitlib PROPERTY IMPORTED_LOCATION ${luajit_SOURCE_DIR}/src/libluajit.a)
endif()
message("VidentiUI: Fetched/compiled dependencies")

# Add source
add_library (VidentiUI "src/VidentiUI.cpp" "src/VidentiUI.h" "src/VidentiParse.cpp" "src/VidentiParse.h" "src/VidentiRenderer.h"  "src/VidentiMath.h" "src/VidentiRenderer.cpp")
target_include_directories(VidentiUI PUBLIC ${json_SOURCE_DIR}/single_include)
target_include_directories(VidentiUI PUBLIC ${luajit_SOURCE_DIR}/src)
target_include_directories(VidentiUI PUBLIC ${freetype_SOURCE_DIR}/include)
target_link_libraries(VidentiUI INTERFACE luajitlib)
target_link_libraries(VidentiUI PUBLIC freetype)
set_property(TARGET VidentiUI PROPERTY CXX_STANDARD 20)
message("VidentiUI: Finished build configuration")

# For tests
if (VIDENTI_TESTS)
	message("VidentiUI: VIDENTI_TESTS has been set, configuring test app...")
	set (VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/tests)
	add_executable(Tests "tests/src/main.cpp" "tests/src/main.h" "tests/src/Tests.cpp" "tests/src/Tests.h" "renderers/angle/VidentiAngleRenderer.cpp" "renderers/angle/VidentiAngleRenderer.h" "renderers/stb.cpp" "pollers/glfw/VidentiGLFWPoller.cpp" "pollers/glfw/VidentiGLFWPoller.h")

	target_include_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/src/)
	target_include_directories(Tests PUBLIC ${angle_SOURCE_DIR}/include/)
	target_include_directories(Tests PUBLIC ${stb_SOURCE_DIR})
	target_include_directories(Tests PUBLIC ${glfwsrc_SOURCE_DIR}/include/)
	target_include_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/renderers/angle/)
	target_include_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/tests/include/)
	target_include_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/pollers/glfw/)
	
	if (WIN32)
		target_link_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/lib/windows)
	elseif (APPLE)
		target_link_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/lib/apple)
	else()
		target_link_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/lib/linux)
	endif()
	
	if(WIN32)
	target_link_libraries(Tests PUBLIC EGL)
	target_link_libraries(Tests PUBLIC GLESv2)
	target_link_libraries(Tests PUBLIC glfw3) # Use pre-compiled lib
	else()
	target_link_libraries(Tests PUBLIC glfw.so.3.3) # Must be installed with apt-get if not already available
	target_link_libraries(Tests PUBLIC GLESv2)
	target_link_libraries(Tests PUBLIC EGL)
	endif()
	
	target_link_libraries(Tests PUBLIC VidentiUI)

	set_property(TARGET Tests PROPERTY CXX_STANDARD 20)
	message("VidentiUI: Configured test app")
endif()
