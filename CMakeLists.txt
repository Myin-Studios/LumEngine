cmake_minimum_required(VERSION 3.26)
project(CMEngine)

set(CMAKE_CXX_STANDARD 14)

include_directories(Engine)
include_directories(Engine/EventSystem)
include_directories(Engine/LogSystem)
include_directories(Engine/Math)
include_directories(Engine/Types)
include_directories(GUI)

# include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/um")
# include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/shared")
# include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/winrt")
# include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/ucrt")
# include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/cppwinrt")
# include_directories("C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.29.30133/include")
# set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64")

add_executable(CMEngine
        Engine/EventSystem/CMEvents.h
        Engine/EventSystem/CMEventSystem.h
        Engine/LogSystem/LogSystem.h
        Engine/Math/CMMath.h
        Engine/Types/CMTypes.h
        GUI/CMItem.h
        GUI/CMWindow.h
        GUI/EngineStyle.cpp
        GUI/EngineStyle.h
        main.cpp
        GUI/CMSlider.h)

# Trova le librerie DWM
find_library(DWM_LIBRARY dwmapi)

# Se le librerie DWM sono state trovate, includile nel progetto
if(DWM_LIBRARY)
    target_link_libraries(CMEngine PRIVATE ${DWM_LIBRARY})
else()
    message(FATAL_ERROR "Libreria DWM non trovata.")
endif()