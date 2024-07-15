cmake_minimum_required(VERSION 3.25.0)
project(CMEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_LIB_PATH $ENV{GLFW_LIB})
find_library(GLFW_LIB glfw3 HINTS ${GLFW_LIB_PATH})

set(GLEW_ROOT $ENV{GLEW_ROOT})
find_package(GLEW REQUIRED)

if(NOT GLEW_FOUND)
    message(FATAL_ERROR "GLEW not found. Set GLEW_ROOT to the directory containing GLEW.")
endif()

set(wx_ROOT_DIR $ENV{wxWidgets})

set(wxWidgets_LIB_DIR "${wx_ROOT_DIR}/lib")

set(wxWidgets_INCLUDE_DIRS
        "${wx_ROOT_DIR}/include/msvc"
        "${wx_ROOT_DIR}/include"
)

# Determina se stiamo costruendo in debug o in release
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(GLOB WX_LIBS "${wxWidgets_LIB_DIR}/vc_x64_lib/*.lib")

    set(wxWidgets_LIBRARIES
            ${WX_LIBS}
    )
else()
    file(GLOB WX_LIBS "${wxWidgets_LIB_DIR}/vc_lib/*.lib")

    set(wxWidgets_LIBRARIES
            ${WX_LIBS}
    )
endif()

find_package(OpenGL REQUIRED)

# Function to find the latest Qt version
#function(find_latest_qt)
#    if (NOT DEFINED ENV{QT_DIR})
#        message(FATAL_ERROR "QT_DIR environment variable not set.")
#    endif()
#
#    file(GLOB QT_VERSIONS LIST_DIRECTORIES true "$ENV{QT_DIR}/[0-9].[0-9]*")
#
#    if (QT_VERSIONS)
#        list(SORT QT_VERSIONS)
#        list(GET QT_VERSIONS -1 QT_LATEST_DIR) # -1 to get the last element (latest version)
#
#        # Append the compiler directory if it exists
#        file(GLOB QT_COMPILER_DIRS LIST_DIRECTORIES true "${QT_LATEST_DIR}/*")
#        foreach(QT_COMPILER_DIR ${QT_COMPILER_DIRS})
#            if(IS_DIRECTORY ${QT_COMPILER_DIR})
#                set(QT_LATEST_DIR ${QT_COMPILER_DIR})
#                break()
#            endif()
#        endforeach()
#
#        set(QT_FOUND_DIR "${QT_LATEST_DIR}" CACHE INTERNAL "Path to the latest Qt directory")
#    else()
#        message(FATAL_ERROR "No Qt versions found under $ENV{QT_DIR}.")
#    endif()
#endfunction()
#
## Call the function to find the latest Qt version
#find_latest_qt()

# Set the CMake prefix path to point to the found Qt directory
set(CMAKE_PREFIX_PATH "${QT_FOUND_DIR}/lib/cmake")

#find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets OpenGL)

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)

include_directories(Engine)
include_directories(Engine/EventSystem)
include_directories(Engine/LogSystem)
include_directories(Engine/Math)
include_directories(Engine/Types)
include_directories(GUI)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${Qt6Widgets_INCLUDE_DIRS} ${Qt6OpenGL_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
include_directories($ENV{GLFW_PATH})
include_directories($ENV{DIG_DIR})
include_directories(Engine/DearImGuiSrc)
include_directories(${wxWidgets_INCLUDE_DIRS_MSWC})
include_directories(${wxWidgets_INCLUDE_DIRS})
include_directories($ENV{GLAD_INC})
include_directories($ENV{GLEW_INC})
include_directories(C:/GLEW/glew-2.1.0/include)
include_directories($ENV{GLM})
link_directories(C:/GLEW/glew-2.1.0/lib/Release/x64)

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
        Engine/BaseObject/PropertyManager.h
        Engine/GUI/MainWindow/renderer.h
        Engine/GUI/MainWindow/camera.h)

find_library(OpenGL OpenGL32 HINTS "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.26100.0\\um\\x64")

message("OpenGL library found: ${OpenGL}")
message("GLEW Libs: $ENV{GLEW_LIB}")

target_link_directories(CMEngine PUBLIC
        "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.26100.0\\um\\x64"
        ${OPENGL_LIBRARIES}
        $ENV{GLFW_PATH}
        $ENV{GLEW_LIB})

target_link_libraries(CMEngine PRIVATE
        ${OpenGL}
        ${GLEW_STATIC_LIBRARIES}
        ${wxWidgets_LIBRARIES}
)

#add_custom_command(TARGET CMEngine POST_BUILD
#        COMMAND "${QT_FOUND_DIR}/bin/windeployqt" $<TARGET_FILE:CMEngine>
#        COMMENT "Deploying Qt dependencies with windeployqt"
#)

add_definitions(-D__WXMSW__)
add_definitions(-D_UNICODE)
add_definitions(-DGLEW_STATIC)
add_definitions(-DWIN32_LEAN_AND_MEAN)
add_definitions(-DNOMINMAX)

if(MSVC)
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
endif()
