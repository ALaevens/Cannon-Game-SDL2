cmake_minimum_required(VERSION 3.0.0)
project(cannon VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")

include(CTest)
enable_testing()

#https://github.com/aminosbh/sdl2-cmake-modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(LOCAL_INCLUDE_DIR include)
include_directories(${LOCAL_INCLUDE_DIR})


add_executable(${PROJECT_NAME} src/main.cpp src/game.cpp src/Engine/ECS.cpp src/Engine/Window.cpp src/Engine/util.cpp src/Engine/Clock.cpp src/Engine/TextureManager.cpp src/Engine/SFXManager.cpp src/Engine/FontManager.cpp)
# target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/third_party")

target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image SDL2::GFX SDL2::Mixer SDL2::TTF)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)
add_dependencies(${PROJECT_NAME} copy_assets)


