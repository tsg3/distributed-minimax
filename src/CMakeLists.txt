## include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

## compiler flags
add_compile_options(-g)

## variables
set(LIBRARIES pthread)

## Test network
# add_executable(test_network 
#     test_network.c 
#     network/network.c 
#     network/messages.c 
#     chess/chess.c
#     cJSON.c)

# target_link_libraries(test_network ${MPI_C_LIBRARIES} ${LIBRARIES})

# install(TARGETS test_network DESTINATION bin)

## linking source files
add_executable(test 
    main.c 
    cJSON.c 
    configuration.c 
    chess/chess.c 
    minimax/minimax.c 
    evaluation/evaluation.c
    reports/reports.c
    network/network.c 
    network/messages.c)

## linking libraries
target_link_libraries(test ${MPI_C_LIBRARIES} ${LIBRARIES})

## install executables
install(TARGETS test DESTINATION bin)