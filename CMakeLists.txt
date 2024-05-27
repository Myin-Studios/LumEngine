cmake_minimum_required(VERSION 3.25.2)
project(CMEngine)

set(CMAKE_CXX_STANDARD 20)

include_directories(Engine)
include_directories(Engine/EventSystem)
include_directories(Engine/LogSystem)
include_directories(Engine/Math)
include_directories(Engine/Types)
include_directories(GUI)


# find_package(GTK4)
# include_directories(${GTK4_INCLUDE_DIR})

add_executable(CMEngine
        Engine/EventSystem/CMEvents.h
        Engine/EventSystem/CMEventSystem.h
        Engine/LogSystem/LogSystem.h
        Engine/Math/CMMath.h
        Engine/Types/CMTypes.h
        main.cpp
        Engine/GameBehaviour/CMGameBehaviour.h
        Engine/GameBehaviour/CMGameBehaviour.h
        Engine/GameBehaviour/CMTime.h
        Engine/GameBehaviour/CMTime.h Engine/Core/CMCore.h)

