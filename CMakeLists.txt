cmake_minimum_required(VERSION 3.14)
project(virtual_chef CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_DEBUG_TYPE Debug)

# Configuración de flags por tipo de compilador
if (MSVC)
    add_compile_options(/W4 /WX)   
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror -std=c++17)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    src/main.cpp
    src/recipeManager.cpp
    src/recipe.cpp
    src/utils.cpp
)

add_executable(main ${SOURCES})
install(TARGETS main RUNTIME DESTINATION binaries)