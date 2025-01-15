cmake_minimum_required(VERSION 3.12)

project(
    labwork11
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(bin)
add_subdirectory(lib)

enable_testing()
add_subdirectory(tests)
