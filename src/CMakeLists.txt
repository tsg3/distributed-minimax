include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(test main.c cJSON.c)

install(TARGETS test DESTINATION bin)