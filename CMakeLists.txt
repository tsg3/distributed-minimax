## cmake version
cmake_minimum_required(VERSION 2.6)

## cross-compiling environment [Testing]
## set(CMAKE_C_COMPILER gcc)

## project name
project(distributed-minimax)

## linker flags
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

## include MPI include files
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

## add subdirectories
add_subdirectory(src)

## package info
set(MAJOR "1")
set(MINOR "1")
set(PATCH "0")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${MAJOR}.${MINOR}.${PATCH}")
set(CPACK_SOURCE_IGNORE_FILES "/build/;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)