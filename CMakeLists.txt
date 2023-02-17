cmake_minimum_required(VERSION 3.10)
project(MiniPL_interpreter)

set(CMAKE_CXX_STANDARD 17)

#set_property(DIRECTORY PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

message("PROJECT SOURCE DIRECTORY ${PROJECT_SOURCE_DIR}") 
# # Compile flags
# SET(GCC_COVERAGE_COMPILE_FLAGS "-DARMA_DONT_USE_WRAPPER -std=c++17 -ggdb -Wall -Wextra -g -O3 -march=native -MMD -MP -fopenmp")
# SET(GCC_COVERAGE_LINK_FLAGS    "-DARMA_DONT_USE_WRAPPER -larmadillo -lopenblas -llapack -fopenmp -lnlopt -lm")
SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -g -O3 -march=native -MMD -MP -fopenmp")
SET(GCC_COVERAGE_LINK_FLAGS    "")

# # Add to CMake variables
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

message("${CMAKE_EXE_LINKER_FLAGS}")

include_directories(src/include)
include_directories("/usr/local/include")

add_subdirectory(src)

add_subdirectory(tests)

add_subdirectory(libraries/googletest)



set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")


enable_testing()
 