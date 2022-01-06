set(PROJ_NAME test1)
project(${PROJ_NAME})

message(STATUS "Adding project ${PROJ_NAME}")

include_directories(Headers/
                    ${GLITTER_DIR}/Vendor/assimp/include/
                    ${GLITTER_DIR}/Vendor/bullet/src/
                    ${GLITTER_DIR}/Vendor/glad/include/
                    ${GLITTER_DIR}/Vendor/glfw/include/
                    ${GLITTER_DIR}/Vendor/glm/
                    ${GLITTER_DIR}/Vendor/stb/)



file(GLOB VENDORS_SOURCES ${GLITTER_DIR}/Vendor/glad/src/glad.c)
file(GLOB PROJECT_HEADERS Headers/*.hpp
                          Headers/*.h)
file(GLOB PROJECT_SOURCES Sources/*.cpp)
file(GLOB PROJECT_SHADERS Shaders/*.comp
                          Shaders/*.frag
                          Shaders/*.geom
                          Shaders/*.vert
                          Shaders/*.vs
                          Shaders/*.fs)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE
    -DGLFW_INCLUDE_NONE 
    -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
               

target_link_libraries(${PROJECT_NAME} assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_SHADERS})
