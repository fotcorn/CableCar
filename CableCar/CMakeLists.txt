cmake_minimum_required (VERSION 3.8)

add_executable (CableCar "CableCar.cpp" "Color.h" "Texture.cpp" "Texture.h")

set_property(TARGET CableCar PROPERTY CXX_STANDARD 17)

target_link_libraries(CableCar ${CONAN_LIBS})
