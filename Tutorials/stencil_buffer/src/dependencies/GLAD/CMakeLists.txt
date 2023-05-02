cmake_minimum_required(VERSION 3.5)
project(glad VERSION 1.1 LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set(CMAKE_C_CREATE_STATIC_LIBRARY "<CMAKE_AR> -ar qc <TARGET> <LINK_FLAGS> <OBJECTS>")
include_directories(${CMAKE_SOURCE_DIR})

add_library(glad STATIC
    glad/glad.h
    KHR/khrplatform.h
    glad.c
)

# set_source_files_properties(GLAD/glad.c PROPERTIES LANGUAGE C)

# set_target_properties(
# glad
# PROPERTIES LINKER_LANGUAGE C)

# add_subdirectory(glfw)

# target_link_libraries(glfw PRIVATE glad)
