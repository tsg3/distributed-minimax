include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
# include_directories($ENV{LD_LIBRARY_PATH}/../include)

add_compile_options(-g)

# add_executable(test main.c cJSON.c chess/chess.c minimax/minimax.c)
add_executable(test main.c network/network.c)

# find_package(MPI)
# include_directories(SYSTEM "/home/gsmuga3/packages/openmpi-4.0.1/include")
# set(MPI_C_LIBRARIES $ENV{LD_LIBRARY_PATH}/mpi)
target_link_libraries(test ${MPI_C_LIBRARIES})

# set(LIBRARY_NAME mpi)
# target_link_libraries(test ${LIBRARY_NAME})

install(TARGETS test DESTINATION bin)