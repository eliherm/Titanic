cmake_minimum_required(VERSION 3.14)
project(titanic)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR})

add_executable(titanic src/controller.cpp src/physics.cpp src/view.cpp src/keyboardInput.cpp src/sprite.cpp)

# add_compile_definitions(WINDOWS)
target_link_libraries(titanic ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARY})
