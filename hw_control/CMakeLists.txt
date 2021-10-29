## cmake version
cmake_minimum_required(VERSION 2.6)

## project name
project(hw_controller)

## cross-compiling environment
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_C_LINKER arm-linux-gnueabihf-ld)

## linker flags
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

## add subdirectories
add_subdirectory(src)

## add directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

## add executables
add_executable(test_ids test_ids.c)
add_executable(test_states test_states.c)
add_executable(test_shutdown test_shutdown.c)
add_executable(test_checking test_checking.c)

## link executables with the proper libraries
target_link_libraries(test_ids HWController)
target_link_libraries(test_states HWController)
target_link_libraries(test_shutdown HWController) 
target_link_libraries(test_checking HWController) 

## install the executables
install(TARGETS test_ids DESTINATION bin)
install(TARGETS test_states DESTINATION bin)
install(TARGETS test_shutdown DESTINATION bin)
install(TARGETS test_checking DESTINATION bin)

## package info
set(MAJOR "1")
set(MINOR "1")
set(PATCH "0")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${MAJOR}.${MINOR}.${PATCH}")
set(CPACK_SOURCE_IGNORE_FILES "/build/;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)