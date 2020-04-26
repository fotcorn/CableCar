cmake_minimum_required (VERSION 3.8)

project ("CableCar")

set(CONAN_SYSTEM_INCLUDES ON)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory ("CableCar")
