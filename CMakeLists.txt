cmake_minimum_required(VERSION 3.10)
project(Wandering_Peasant)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(wandering_peasant 
    src/main.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/Game.cpp
    src/Item.cpp
    src/NPC.cpp
    src/Relic.cpp
    src/Terrain.cpp
    src/Blessing.cpp
    src/Village.cpp
)

target_include_directories(wandering_peasant PRIVATE include) 