cmake_minimum_required(VERSION 3.10..3.17)

project(Libceng391 VERSION 1.0
               DESCRIPTION "Image processing library and tools for stiching"
               LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
add_subdirectory(app)
