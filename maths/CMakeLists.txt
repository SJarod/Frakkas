cmake_minimum_required(VERSION 3.9...3.21)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

set(CMAKE_CXX_STANDARD 20 CACHE STRING "Language standard" FORCE)
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "Use the C++ standard" FORCE)
set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "" FORCE)

project(Maths
        LANGUAGES CXX C
        VERSION 1.0
        DESCRIPTION "Math Toolbox"
)

option(MATHS_BUILD_EXECUTABLE "Build an executable to test maths library" ON)

add_library(Mathlib INTERFACE)

target_include_directories(Mathlib
                           INTERFACE include
                           INTERFACE inline)

if(MATHS_BUILD_EXECUTABLE)
    add_executable(Maths main.cpp)
    target_link_libraries(Maths PRIVATE Mathlib)
endif()
