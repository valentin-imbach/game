
cmake_minimum_required(VERSION 3.0)

# set the project name
project(Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# add executables
set(SOURCES
src/framework/Window.cpp
src/Game.cpp
src/framework/TextureManager.cpp
src/framework/TextManager.cpp
src/framework/SoundManager.cpp
src/framework/Sprite.cpp
src/world/World.cpp
src/world/Map.cpp
src/world/Tile.cpp
src/world/Generation.cpp
src/world/Realm.cpp
src/Gui/GuiElement.cpp
src/Gui/GuiManager.cpp
src/item/Item.cpp
src/item/ItemTemplates.cpp
src/item/Loot.cpp
src/item/Crafting.cpp
src/ResourceTemplates.cpp
src/EntityFactory.cpp
src/Console.cpp
src/ParticleSystem.cpp
src/main.cpp)

# add libraries
find_library(SDL2_LIBRARY SDL2)
find_library(SDL2_IMAGE_LIBRARY SDL2_image)
find_library(SDL2_MIXER_LIBRARY SDL2_mixer)
find_library(SDL2_TTF_LIBRARY SDL2_ttf)

set(LIBRARIES
${SDL2_LIBRARY}
${SDL2_IMAGE_LIBRARY}
${SDL2_MIXER_LIBRARY}
${SDL2_TTF_LIBRARY})

#find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)

add_executable(Game ${SOURCES})

target_link_libraries(Game ${LIBRARIES})

#target_link_libraries(Game PRIVATE SDL2::SDL2)
#target_link_libraries(Game PRIVATE SDL2::SDL_image)

target_compile_options(Game PRIVATE)
target_include_directories(Game PRIVATE src src/utils src/gui src/framework src/ecs src/ecs/systems src/world src/item)