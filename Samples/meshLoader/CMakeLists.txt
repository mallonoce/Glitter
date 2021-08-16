cmake_minimum_required(VERSION 3.19)
set(PROJECT_NAME meshLoader)
project(${PROJECT_NAME})

file(GLOB PROJECT_HEADERS *.hpp
                          *.h)
file(GLOB PROJECT_SOURCES *.cpp)
file(GLOB PROJECT_SHADERS *.comp
                          *.frag
                          *.geom
                          *.vert
                          *.vs
                          *.fs)

add_executable(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES} ${PROJECT_SHADERS})

target_link_directories(${PROJECT_NAME} PUBLIC glitter)