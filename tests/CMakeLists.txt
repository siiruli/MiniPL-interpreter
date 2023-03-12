
set(BINARY ${CMAKE_PROJECT_NAME}_tst)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})

target_compile_definitions(${BINARY} PRIVATE "ROOT_DIR=\"${PROJECT_SOURCE_DIR}\"")

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)

# Compile flags
SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -ggdb -g -O3 -march=native -MMD -MP -fopenmp") # -Wall -Wextra
SET(GCC_COVERAGE_LINK_FLAGS    "")
# Add to CMake variables
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")



include_directories("/usr/local/include")
include_directories("${PROJECT_SOURCE_DIR}/src/include")

message("${PROJECT_SOURCE_DIR}/src/include")

#set_property(DIRECTORY PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
  