cmake_minimum_required(VERSION 3.5)

project(xtracter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(uforia)
add_executable(xtracter main.cpp)
target_link_libraries(xtracter uforia)

file(COPY ${CMAKE_SOURCE_DIR}/english.dic DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/test.txt DESTINATION ${CMAKE_BINARY_DIR})

install(TARGETS xtracter
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
