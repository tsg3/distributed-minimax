## include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

## compiler flags
add_compile_options(-g)

## linking source files
add_executable(test 
    main.c 
    cJSON.c 
    configuration.c 
    chess/chess.c 
    minimax/minimax.c 
    evaluation/evaluation.c
    reports/reports.c)

## variables
set(LIBRARIES pthread)

## linking libraries
target_link_libraries(test ${LIBRARIES})

## install executables
install(TARGETS test DESTINATION bin)