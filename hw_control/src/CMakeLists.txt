include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(INCLUDE_FILES ${INCLUDE_DIR}/HWController.h)

add_library(HWController SHARED HWController.c ${INCLUDE_FILES})

install(TARGETS HWController DESTINATION lib)
install(FILES ${INCLUDE_FILES} DESTINATION include)