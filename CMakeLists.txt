#
# author: w<w359405949@gmail.com>
# created: 2014-08-27

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(luv)

MESSAGE(STATUS "Project: ${PROJECT_NAME}")
MESSAGE(STATUS "Project Directory: ${PORJECT_SOURCE_DIR}")

SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_C_FLAGS_DEBUG "-g -Wall")
# SET(CMAKE_CXX_FLAGS "std=c++0x")
SET(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -Wall")

file(GLOB_RECURSE source src/*.c src/*.h)
message("  -> Prepared: luv")

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/deps/lua/
    ${PROJECT_SOURCE_DIR}/deps/lua/src/
    ${PROJECT_SOURCE_DIR}/deps/libuv/include/
)

link_directories(
    ${PROJECT_SOURCE_DIR}/deps/lua/
    ${PROJECT_SOURCE_DIR}/deps/libuv/.libs/
)


add_library(${PROJECT_NAME} SHARED ${source})



set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/)
target_link_libraries(${PROJECT_NAME}
    lua
    uv
)

#me_component add_subdirectory(src)
