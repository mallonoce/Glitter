cmake_minimum_required(VERSION 3.19)
set(PROJECT_NAME sample_test)
project(${PROJECT_NAME})

file(GLOB PROJECT_HEADERS Headers/*.hpp
                          Headers/*.h)
file(GLOB PROJECT_SOURCES Sources/*.cpp)
file(GLOB PROJECT_SHADERS Shaders/*.comp
                          Shaders/*.frag
                          Shaders/*.geom
                          Shaders/*.vert
                          Shaders/*.vs
                          Shaders/*.fs)

add_executable(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES} ${PROJECT_SHADERS})

target_link_directories(${PROJECT_NAME} glitter)