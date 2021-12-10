# CMakeList.txt : CMake project for IMS, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# set the project name
project(IMS21 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)


# Add source to this project's executable.
add_executable ("${PROJECT_NAME}"  "src/EntryMain.cpp" "include/Grid.h" "include/Cell.h" "include/Resources.h" "include/Tree.h")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")
# TODO: Add tests and install targets if needed.
