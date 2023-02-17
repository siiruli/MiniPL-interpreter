
set(BINARY ${CMAKE_PROJECT_NAME})


include_directories("include")
include_directories("/usr/local/include")


file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})
message("Sources: ${SOURCES}")

add_executable(${BINARY}_run ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})

# add_executable(${CMAKE_PROJECT_NAME} main.cpp)
# target_link_libraries(${CMAKE_PROJECT_NAME} ${ARMADILLO_LIBRARIES})


#add_executable(foo foo.cc)

install(TARGETS ${BINARY}_run RUNTIME DESTINATION bin)

#set_property(GLOBAL PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
 