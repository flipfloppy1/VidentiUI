# CMakeLists.txt file for VidentiUI

cmake_minimum_required (VERSION 3.8)
include(FetchContent)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

FetchContent_Declare(
	json
	GIT_REPOSITORY https://github.com/nlohmann/json
	GIT_TAG        8c391e04fe4195d8be862c97f38cfe10e2a3472e # latest commit as of 25/06/2024
)

FetchContent_MakeAvailable(json)

project("VidentiUI")

# Add source to this project's executable.
add_library (VidentiUI "VidentiUI.cpp" "VidentiUI.h" "Parse.cpp" "Parse.h")
target_include_directories(VidentiUI PUBLIC ${json_SOURCE_DIR}/include)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VidentiUI PROPERTY CXX_STANDARD 20)
endif()


if (VIDENTI_TESTS)
add_executable(Tests "Tests.cpp" "Tests.h")
endif()