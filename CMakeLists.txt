
cmake_minimum_required(VERSION 3.0)

# set the project name
project(Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# set(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib")
# set(CMAKE_FIND_FRAMEWORK NEVER) 

# add executables
set(SOURCES
src/framework/Window.cpp
src/Game.cpp
src/framework/TextureManager.cpp
src/framework/TextManager.cpp
src/framework/SoundManager.cpp
src/framework/Sprite.cpp
src/framework/FileWatcher.cpp
src/world/RealmManager.cpp
src/world/World.cpp
src/world/Tile.cpp
src/world/Generation.cpp
src/world/Realm.cpp
src/world/Environment.cpp
src/World/Chunk.cpp
src/World/ChunkManager.cpp
src/Gui/GuiElement.cpp
src/Gui/GuiManager.cpp
src/Gui/Console.cpp
src/Gui/Minimap.cpp
src/Gui/Menu.cpp
src/item/Item.cpp
src/item/ItemTemplates.cpp
src/item/Loot.cpp
src/item/Crafting.cpp
src/templates/ResourceTemplates.cpp
src/templates/AnimalTemplates.cpp
src/templates/StructureTemplates.cpp
src/templates/ClusterTemplates.cpp
src/EntityFactory.cpp
src/gui/Command.cpp
src/ParticleSystem.cpp
src/Distribution.cpp
src/main.cpp)

# find_package(SDL3 REQUIRED)

# add libraries
# find_library(SDL3_LIBRARY NAMES SDL3 FRAMEWORK PATHS /Library/Frameworks/)
# find_library(SDL3_IMAGE_LIBRARY SDL3_image FRAMEWORK PATHS /Library/Frameworks/)
# find_library(SDL3_MIXER_LIBRARY SDL3_mixer FRAMEWORK)
# find_library(SDL3_TTF_LIBRARY SDL3_ttf FRAMEWORK)

# set(LIBRARIES
# ${SDL3_LIBRARY}
# ${SDL3_IMAGE_LIBRARY}
# ${SDL3_MIXER_LIBRARY}
# ${SDL3_TTF_LIBRARY})

#find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)

add_executable(Game ${SOURCES})
# add_library(Game ${SOURCES})

# set_target_properties(Game PROPERTIES INSTALL_RPATH "@executable_path")

target_include_directories(Game PRIVATE include src src/utils src/gui src/framework src/ecs src/ecs/systems src/world src/item src/templates)

link_directories(${PROJECT_SOURCE_DIR}/libs)

# target_link_libraries(Game PRIVATE ${PROJECT_SOURCE_DIR}/libs/SDL3.framework/Versions/A/SDL3)
# target_link_libraries(Game PRIVATE ${PROJECT_SOURCE_DIR}/libs/SDL3_image.framework/Versions/A/SDL3_image)
# target_link_libraries(Game PRIVATE ${PROJECT_SOURCE_DIR}/libs/SDL3_mixer.framework/Versions/A/SDL3_mixer)
# target_link_libraries(Game PRIVATE ${PROJECT_SOURCE_DIR}/libs/SDL3_ttf.framework/Versions/A/SDL3_ttf)

target_link_libraries(Game PRIVATE ${PROJECT_SOURCE_DIR}/libs/libSDL3.0.dylib)
target_link_libraries(Game PRIVATE ${PROJECT_SOURCE_DIR}/libs/libSDL3_image.0.dylib)
target_link_libraries(Game PRIVATE ${PROJECT_SOURCE_DIR}/libs/libSDL3_mixer.0.dylib)
target_link_libraries(Game PRIVATE ${PROJECT_SOURCE_DIR}/libs/libSDL3_ttf.0.dylib)

# target_link_libraries(Game PRIVATE SDL3_IMAGE_LIBRARY)





#target_link_libraries(Game PRIVATE SDL2::SDL2)
#target_link_libraries(Game PRIVATE SDL2::SDL_image)

target_compile_options(Game PRIVATE)
