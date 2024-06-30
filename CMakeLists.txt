cmake_minimum_required (VERSION 3.12)
include(FetchContent)

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


FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# Add source
add_library (VidentiUI "src/VidentiUI.cpp" "src/VidentiUI.h" "src/VidentiParse.cpp" "src/VidentiParse.h" "src/VidentiRenderer.h" "src/VidentiRenderer.cpp" "src/VidentiMath.h")
target_include_directories(VidentiUI PUBLIC ${CMAKE_HOME_DIRECTORY}/include)
target_link_libraries(VidentiUI PRIVATE nlohmann_json::nlohmann_json)
set_property(TARGET VidentiUI PROPERTY CXX_STANDARD 20)

# For tests
if (VIDENTI_TESTS)
set (VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/tests)
add_executable(Tests "tests/src/main.cpp" "tests/src/main.h" "tests/src/Tests.cpp" "tests/src/Tests.h" "renderers/angle/VidentiAngleRenderer.cpp" "renderers/angle/VidentiAngleRenderer.h" "renderers/include/stb/stb.cpp")

target_include_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/src)
target_include_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/include)
target_include_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/renderers/include)
target_include_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/renderers/angle)
target_include_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/tests/include)
target_include_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/tests/include/angle)

target_link_directories(Tests PUBLIC ${CMAKE_HOME_DIRECTORY}/lib)

target_link_libraries(Tests PUBLIC glfw3)
target_link_libraries(Tests PUBLIC libEGL)
target_link_libraries(Tests PUBLIC libGLESv2)
target_link_libraries(Tests PUBLIC VidentiUI)

set_property(TARGET Tests PROPERTY CXX_STANDARD 20)
endif()