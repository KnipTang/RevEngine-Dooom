cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 23)

project(ReVengine LANGUAGES C CXX)

include(FetchContent)

find_package(SDL2 CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(unofficial-omniverse-physx-sdk CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

FetchContent_Declare(
  soLoud
  GIT_REPOSITORY https://git.allpurposem.at/mat/SoLoud-CMake.git
  GIT_TAG 3b7eec82fcdbb0fb436862c402c37770b0604cd0
  GIT_PROGRESS TRUE
  OVERRIDE_FIND_PACKAGE TRUE
)
FetchContent_MakeAvailable(soLoud)

add_subdirectory ("ReVengine-Engine")
add_subdirectory ("ReVengine-Game")