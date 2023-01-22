cmake_minimum_required(VERSION 3.0.0)
project(test_gl VERSION 0.1.0)

include(CTest)
enable_testing()

set(GLAD_C "./include/glad/glad.c")

add_executable(test_gl main.cpp ${GLAD_C})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_compile_definitions(GLFW_INCLUDE_GLU)
target_link_libraries(test_gl libglu32.a libglfw3.a libopengl32.a libgdi32.a libglfw3dll.a)



file(COPY "shaders/." DESTINATION "./shaders")
file(COPY "resources/." DESTINATION "./resources")


