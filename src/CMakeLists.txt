cmake_minimum_required(VERSION 3.5)

find_program(CMAKE_CXX_COMPILER NAMES g++)

if(CMAKE_CXX_COMPILER)
    message(STATUS "Found g++ compiler: ${CMAKE_CXX_COMPILER}")
else()
    message(FATAL_ERROR "Could not find g++ compiler")
    exit()
endif()

project(3D_Viewer VERSION 2.0 LANGUAGES CXX)

add_compile_options(-O3)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS forms)

set(QT_LIBS
    Widgets
    OpenGL
    OpenGLWidgets
    Gui
    Core
)
set(EXCLUDE_DIRS tests)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_LIBS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_LIBS})

include_directories(shared common view techniques model ECS)

file(GLOB_RECURSE PROJECT_SOURCES *.cc *.h *.ui)

include(FetchContent)
FetchContent_Declare(
  assimp
  URL https://github.com/assimp/assimp/archive/master.zip
)

FetchContent_GetProperties(assimp)

if(NOT assimp_POPULATED)
  FetchContent_Populate(assimp)
  set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
  set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(ASSIMP_NO_EXPORT ON CACHE BOOL "")
  set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "")
  set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "")
  set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "")
  add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR})

  if(WIN32)
    set(ASSIMP_OPT_BUILD_PACKAGES ON CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
  endif()
endif()

foreach(i ${EXCLUDE_DIRS})
    message("Excluding dir: " ${i})
    list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*${i}.*")
endforeach()

foreach(i ${PROJECT_SOURCES})
    message("Project file: " ${i})
endforeach()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_SOURCES})
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
    else()
        add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
    endif()
endif()

foreach(i ${QT_LIBS})
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::${i})
endforeach(i)

target_link_libraries(${PROJECT_NAME} PRIVATE assimp)

IF(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
ENDIF()

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/others/resources
    ${PROJECT_BINARY_DIR}/resources
    COMMENT "Copying resources into binary directory"
)

add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/others/shaders
    ${PROJECT_BINARY_DIR}/shaders
    COMMENT "Copying shaders into binary directory"
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
