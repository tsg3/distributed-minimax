## variables
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(INCLUDE_FILES ${INCLUDE_DIR}/HWController.h)
set(LIBRARY_NAME HWController)

## include directories
include_directories(${INCLUDE_DIR})

## add source code to library
add_library(${LIBRARY_NAME} SHARED HWController.c ${INCLUDE_FILES})

## install library's source and include files
install(TARGETS ${LIBRARY_NAME} DESTINATION lib)
install(FILES ${INCLUDE_FILES} DESTINATION include)

## install kernel module
install(FILES Makefile push_button_mod.c DESTINATION mod)