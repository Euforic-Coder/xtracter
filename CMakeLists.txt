cmake_minimum_required (VERSION 2.8.11)
project (xtracter)

add_subdirectory(uforia)
add_executable(xtracter main.cpp)

target_link_libraries(xtracter uforia)

file(COPY ${CMAKE_SOURCE_DIR}/english.dic DESTINATION ${CMAKE_BINARY_DIR})
