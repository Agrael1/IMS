# CMakeList.txt : CMake project for IMS, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# set the project name
project(IMS21 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(CMAKE_AUTORCC ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_compile_definitions(_HAS_DEPRECATED_RESULT_OF=1)

# Add source to this project's executable.
add_executable ("${PROJECT_NAME}"  "src/EntryMain.cpp" "include/Grid.h" "include/Cell.h" "include/Resources.h" "include/Tree.h" "src/Grid.cpp" "src/Cell.cpp" "src/App.cpp" "include/App.h" "include/MainWindow.h" "src/MainWindow.cpp" "rc/resources.qrc" "include/Scene.h" "src/Scene.cpp" "include/View.h" "src/View.cpp" "include/Node.h" "src/Node.cpp")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")
# TODO: Add tests and install targets if needed.

target_include_directories("${PROJECT_NAME}" PUBLIC "${Qt5Widgets_INCLUDE_DIRS}")
target_link_libraries("${PROJECT_NAME}" Qt5::Widgets)

# This part is adapted from StackOverflow:
# https://stackoverflow.com/questions/40564443/copying-qt-dlls-to-executable-directory-on-windows-using-cmake
if(WIN32)
    if($<CONFIG:Debug>)
        get_target_property(WIDGETDLL Qt5::Widgets IMPORTED_LOCATION_DEBUG)
    else($<CONFIG:Debug>)
        get_target_property(WIDGETDLL Qt5::Widgets IMPORTED_LOCATION_RELEASE)
    endif($<CONFIG:Debug>)
    add_custom_command(
        TARGET "${PROJECT_NAME}" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Widgets>
        $<TARGET_FILE:Qt5::Core>
        $<TARGET_FILE:Qt5::Gui>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif(WIN32)
