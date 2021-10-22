cmake_minimum_required(VERSION 2.6)

project(hw_controller)

set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_C_LINKER arm-linux-gnueabihf-ld)

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_subdirectory(src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


add_executable(test_controller main.c)
add_executable(test_shutdown test_shutdown.c)

target_link_libraries(test_controller HWController)
target_link_libraries(test_shutdown HWController)   

install(TARGETS test_controller DESTINATION bin)
install(TARGETS test_shutdown DESTINATION bin)

set(MAJOR "1")
set(MINOR "1")
set(PATCH "0")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${MAJOR}.${MINOR}.${PATCH}")
set(CPACK_SOURCE_IGNORE_FILES "/build/;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)