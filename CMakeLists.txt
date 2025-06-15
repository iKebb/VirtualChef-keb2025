cmake_minimum_required(VERSION 3.14)
project(virtual_chef CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_DEBUG_TYPE Debug)

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -std=gnu++17")
# set(CMAKE_CXX_FLAGS_DEBUG "-pedantic -Wall -Wextra -std=gnu++17")
set(CMAKE_CXX_FLGAS_RELEASE "-03 -std=gnu++17")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    src/main.cpp
    src/recipeManager.cpp
    src/recipe.cpp
    src/utils.cpp
)

add_executable(main ${SOURCES})
install(TARGETS main RUNTIME DESTINATION binaries)