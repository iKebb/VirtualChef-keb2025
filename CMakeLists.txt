cmake_minimum_required(VERSION 3.14)
project(virtual_chef CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_DEBUG_TYPE Debug)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    src/main.cpp
    src/recipeManager.cpp
    src/recipe.cpp
    src/trim.cpp
)

add_executable(main ${SOURCES})

if (MSVC)
    target_compile_options(main PRIVATE /W4)
else()
    target_compile_options(main PRIVATE -Wall -Wextra)
endif()