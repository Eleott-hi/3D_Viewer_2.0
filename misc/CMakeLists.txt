cmake_minimum_required(VERSION 3.5)

project(NewOpenGL VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3)

set(QT_LIBS
    Widgets
    OpenGL
    OpenGLWidgets
    Gui
    Core
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_LIBS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_LIBS})

add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/stb_image)
add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/assimp)
add_subdirectory(${CMAKE_SOURCE_DIR}/ECS_components)
add_subdirectory(${CMAKE_SOURCE_DIR}/ECS_systems)
add_subdirectory(${CMAKE_SOURCE_DIR}/ECS_events)
add_subdirectory(${CMAKE_SOURCE_DIR}/techniques)
add_subdirectory(${CMAKE_SOURCE_DIR}/ECS_core)

include_directories(${CMAKE_SOURCE_DIR}/dependencies/stb_image)
include_directories(${CMAKE_SOURCE_DIR}/ECS_components)
include_directories(${CMAKE_SOURCE_DIR}/ECS_systems)
include_directories(${CMAKE_SOURCE_DIR}/techniques)
include_directories(${CMAKE_SOURCE_DIR}/ECS_events)
include_directories(${CMAKE_SOURCE_DIR}/ECS_core)
include_directories(${CMAKE_SOURCE_DIR}/headers)
include_directories(${CMAKE_SOURCE_DIR}/others)
include_directories(${CMAKE_SOURCE_DIR}/sources)

set(DEPEND_LIBS
    Techniques
    stb_image
    opengl32
    assimp
    ECS_core
    ECS_events
    ECS_systems
    ECS_components
)

set(PROJECT_SOURCES
    sources/main.cpp

    # sources/model.cpp
    sources/backend.cpp

    # sources/renderer.cpp
    sources/main_window.cpp
    sources/framebuffer.cpp
    sources/stack_camera.cpp
    sources/stack_object.cpp
    sources/stack_light.cpp
    sources/TextureStorage.cpp
    sources/ModelParser.cpp

    others/main_window.ui

    headers/ModelParser.h
    headers/TextureStorage.h
    headers/common_settings.h

    # headers/model_settings.h
    headers/signal_handler.h
    headers/i_framebuffer.h
    headers/framebuffer.h
    headers/TextureStorage.h
    headers/main_window.h
    headers/controller.h
    headers/gl_widget.h
    headers/backend.h

    # headers/model.h
    # headers/mesh.h
    headers/axis.h
)

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
endforeach()

target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPEND_LIBS})

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/others/shaders
    ${PROJECT_BINARY_DIR}/shaders

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/others/resources
    ${PROJECT_BINARY_DIR}/resources
    COMMENT "Copy shaders/resources into binary diretory"
)

add_dependencies(${PROJECT_NAME} copy_resources)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
