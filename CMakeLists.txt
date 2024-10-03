cmake_minimum_required(VERSION 3.22)
project(CMEngine)

include(ProcessorCount)
ProcessorCount(NUM_CORES)
set(CMAKE_BUILD_PARALLEL_LEVEL ${NUM_CORES})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        OpenGL
        OpenGLWidgets
        REQUIRED)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if(DEFINED ENV{GLEW_ROOT})
        set(GLEW_ROOT $ENV{GLEW_ROOT})
        # Aggiungi il percorso alle directory include e lib
        include_directories("${GLEW_ROOT}/include"
                            "$ENV{Path}")
        include_directories($ENV{GLM})
        link_directories("${GLEW_ROOT}/lib/Release/x64")
    else()
        message(FATAL_ERROR "GLEW_ROOT environment variable is not set.")
    endif()

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(Qt6_DIR $ENV{Qt6_DIR})

    find_package(Qt6 COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)
    find_package(GLEW REQUIRED)

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
endif ()

add_executable(CMEngine
        main.cpp
        Engine/GUI/Rendering/renderer.h
        Engine/GUI/Rendering/renderer.cpp
        Engine/GUI/Windows/mainwindow.h
        Engine/GUI/Windows/mainwindow.cpp
        Engine/GUI/Panels/panel.h
        Engine/GUI/Panels/panel.cpp
        Engine/GUI/guibuilder.h
        Engine/GUI/guibuilder.cpp
        Engine/GUI/Panels/properties.h
        Engine/GUI/Panels/properties.cpp
        Engine/GUI/Buttons/basebutton.h
        Engine/GUI/Buttons/utilsbuttons.h
        Engine/GUI/Buttons/basebutton.cpp
        Engine/GUI/Buttons/utilsbutton.cpp
        Engine/GUI/Buttons/tabsbutton.h
        Engine/GUI/Buttons/tabsbutton.cpp
        Engine/GUI/Rendering/mesh.h
        Engine/GUI/Rendering/shader.h
        Engine/GUI/Rendering/shader.cpp
        Engine/GUI/Rendering/mesh.cpp
        Engine/GUI/Rendering/rendererDebugger.h
        Engine/Core/CMAPICore.h
        Engine/Math/Transforms.h
        Engine/Math/Math.h
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Aggiungi le directory di inclusione e librerie per GLEW
    target_include_directories(CMEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE "${GLEW_ROOT}/include")
    target_link_directories(CMEngine PRIVATE "${GLEW_ROOT}/lib/Release/x64")

    target_link_libraries(CMEngine PRIVATE
            Qt::Core
            Qt::Gui
            Qt::Widgets
            Qt::OpenGL
            Qt::OpenGLWidgets
            glew32s
            opengl32
            "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/8.0.8/runtimes/win-x64/native/nethost.lib"
    )

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_link_libraries(CMEngine PRIVATE
            GLEW::GLEW
            Qt::Core
            Qt::Gui
            Qt::Widgets
            Qt::OpenGL
            Qt::OpenGLWidgets
    )
endif()

qt_add_resources(RESOURCES Engine/Resources/res.qrc)
target_sources(CMEngine PRIVATE ${RESOURCES})

if (MSVC)
    add_custom_command(TARGET CMEngine POST_BUILD
            COMMAND "${Qt6_DIR}/../../../bin/windeployqt.exe"
            ARGS --no-compiler-runtime $<TARGET_FILE:CMEngine>
            COMMENT "Deploying Qt libraries with windeployqt"
            VERBATIM)
endif()

add_definitions(-DGLEW_STATIC)