cmake_minimum_required(VERSION 3.10)
project(RPG_Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(rpg_game 
    src/main.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/Game.cpp
    src/Item.cpp
    src/NPC.cpp
    src/Relic.cpp
    src/Terrain.cpp
)

target_include_directories(rpg_game PRIVATE include) 