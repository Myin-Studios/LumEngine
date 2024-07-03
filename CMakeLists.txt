cmake_minimum_required(VERSION 3.30.0)
project(CMEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/Zc:__cplusplus /permissive-)
endif()

include_directories(Engine)
include_directories(Engine/EventSystem)
include_directories(Engine/LogSystem)
include_directories(Engine/Math)
include_directories(Engine/Types)
include_directories(GUI)
include_directories($ENV{GLFW_PATH}) # <<< you should setup your env var to be able to build this project.
# If you can't include GLFW headers, just reload CMake (on CLion).
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${Qt6Widgets_INCLUDE_DIRS} ${Qt6OpenGL_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

# ensure you have all the dependencies below in your device before building project! :)
#file(TO_CMAKE_PATH "$ENV{GLFW_LIB_PATH}" GLFW_LIB_PATH) # <<< you should setup your env var to be able to build this project.
#set(GLFW_LIBS "${GLFW_LIB_PATH}/glfw3.lib")

#set(CMAKE_PREFIX_PATH "C:/Qt/6.6.0/msvc2019_64/lib/cmake/")

# Function to find the latest Qt version
function(find_latest_qt)
    if (NOT DEFINED ENV{QT_DIR})
        message(FATAL_ERROR "QT_DIR environment variable not set.")
    endif()

    file(GLOB QT_VERSIONS LIST_DIRECTORIES true "$ENV{QT_DIR}/[0-9].[0-9]*")

    if (QT_VERSIONS)
        list(SORT QT_VERSIONS)
        list(GET QT_VERSIONS -1 QT_LATEST_DIR) # -1 to get the last element (latest version)

        # Append the compiler directory if it exists
        file(GLOB QT_COMPILER_DIRS LIST_DIRECTORIES true "${QT_LATEST_DIR}/*")
        foreach(QT_COMPILER_DIR ${QT_COMPILER_DIRS})
            if(IS_DIRECTORY ${QT_COMPILER_DIR})
                set(QT_LATEST_DIR ${QT_COMPILER_DIR})
                break()
            endif()
        endforeach()

        set(QT_FOUND_DIR "${QT_LATEST_DIR}" CACHE INTERNAL "Path to the latest Qt directory")
    else()
        message(FATAL_ERROR "No Qt versions found under $ENV{QT_DIR}.")
    endif()
endfunction()


# Call the function to find the latest Qt version
find_latest_qt()

# Set the CMake prefix path to point to the found Qt directory
set(CMAKE_PREFIX_PATH "${QT_FOUND_DIR}/lib/cmake")

#include(FindQt.cmake)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets OpenGL)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(CMEngine
        Engine/Resources/res.qrc
        Engine/EventSystem/CMEvents.h
        Engine/EventSystem/CMEventSystem.h
        Engine/LogSystem/LogSystem.h
        Engine/Math/CMMath.h
        Engine/Types/CMTypes.h
        main.cpp
        Engine/GameBehaviour/CMGameBehaviour.h
        Engine/GameBehaviour/CMGameBehaviour.h
        Engine/GameBehaviour/CMTime.h
        Engine/GameBehaviour/CMTime.h Engine/Core/CMCore.h
        Engine/Math/Transforms.h
        Engine/GUI/MainWindow/MainWindow.h
        Engine/BaseObject/PropertyManager.h
        Engine/GUI/MainWindow/mainwindow.cpp
        Engine/GUI/MainWindow/mainwindow.h
        Engine/GUI/MainWindow/mainwindow.ui
        Engine/GUI/MainWindow/3DVisualizer/renderer.cpp
        Engine/GUI/MainWindow/3DVisualizer/renderer.h
        Engine/GUI/MainWindow/3DVisualizer/renderer.ui
        Engine/GUI/MainWindow/3DVisualizer/camera.h
        Engine/GUI/MainWindow/3DVisualizer/camera.cpp
        Engine/GUI/MainWindow/3DVisualizer/light.h
        Engine/GUI/MainWindow/colorpicker.h)

target_link_libraries(CMEngine PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets OpenGL::GL)

message("Latest Qt version dir: ${QT_FOUND_DIR}")

add_custom_command(TARGET CMEngine POST_BUILD
        COMMAND "${QT_FOUND_DIR}/bin/windeployqt" $<TARGET_FILE:CMEngine>
        COMMENT "Deploying Qt dependencies with windeployqt"
        )