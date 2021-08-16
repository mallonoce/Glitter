cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME glitter)
project(${PROJECT_NAME})

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

set(INCLUDE_DIRS  Headers/
                    Vendor/assimp/include/
                    Vendor/bullet/src/
                    Vendor/glad/include/
                    Vendor/glfw/include/
                    Vendor/glm/
                    Vendor/stb/)

file(GLOB VENDORS_SOURCES Vendor/glad/src/glad.c)
file(GLOB PROJECT_HEADERS Headers/*.hpp
                          Headers/*.h)
file(GLOB PROJECT_SOURCES Sources/*.cpp)
file(GLOB PROJECT_SHADERS Shaders/*.comp
                          Shaders/*.frag
                          Shaders/*.geom
                          Shaders/*.vert
                          Shaders/*.vs
                          Shaders/*.fs)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_library(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${PROJECT_SHADERS} ${PROJECT_CONFIGS} ${VENDORS_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} assimp glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} BulletDynamics BulletCollision LinearMath)


install(TARGETS ${PROJECT_NAME} DESTINATION lib)
if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
   install(TARGETS ${PROJECT_NAME}d DESTINATION lib)
endif()
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION include)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Glitter/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_SHADERS})
