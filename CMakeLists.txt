cmake_minimum_required(VERSION 3.10)
project(MiniPL_interpreter)


set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
#set_property(DIRECTORY PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
message("PROJECT NAME ${PROJECT_NAME}") 

message("PROJECT SOURCE DIRECTORY ${PROJECT_SOURCE_DIR}") 
# # Compile flags
# SET(GCC_COVERAGE_COMPILE_FLAGS "-DARMA_DONT_USE_WRAPPER -std=c++17 -ggdb -Wall -Wextra -g -O3 -march=native -MMD -MP -fopenmp")
# SET(GCC_COVERAGE_LINK_FLAGS    "-DARMA_DONT_USE_WRAPPER -larmadillo -lopenblas -llapack -fopenmp -lnlopt -lm")
SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -Wall")
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
 

# option(BUILD_DOC "Build documentation" ON)
# find_package(Doxygen)

# if (DOXYGEN_FOUND)
#   message("Doxygen found")
#   set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
#   set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
#   message("${DOXYGEN_IN}")
#   message("${DOXYGEN_OUT}")
#   # request to configure the file
#   configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
#   message("Doxygen build started")
#   # note the option ALL which allows to build the docs together with the application
#   add_custom_target( doc_doxygen ALL
#   COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
#   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#   COMMENT "Generating API documentation with Doxygen"
#   VERBATIM )

# else (DOXYGEN_FOUND)
#   message("Doxygen need to be installed to generate the doxygen documentation")
# endif (DOXYGEN_FOUND)
