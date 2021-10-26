set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)

include_directories(${INCLUDE_DIR})

set(INCLUDE_FILES ${INCLUDE_DIR}/HWController.h)

set(LIBRARY_NAME HWController)

add_library(${LIBRARY_NAME} SHARED HWController.c ${INCLUDE_FILES})

install(TARGETS ${LIBRARY_NAME} DESTINATION lib)
install(FILES ${INCLUDE_FILES} DESTINATION include)

install(FILES Makefile push_button_mod.c DESTINATION mod)