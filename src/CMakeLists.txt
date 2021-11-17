include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_compile_options(-g)

add_executable(test_network test_network.c network/network.c)

target_link_libraries(test_network ${MPI_C_LIBRARIES} pthread)

install(TARGETS test_network DESTINATION bin)