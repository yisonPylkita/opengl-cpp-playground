project(triangle)

find_package(OpenGL)


find_package(SFML 2 COMPONENTS network audio graphics window system REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

add_executable(${PROJECT_NAME} triangle/triangle.cpp)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
