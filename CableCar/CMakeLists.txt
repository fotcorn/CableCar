cmake_minimum_required (VERSION 3.8)

add_executable (CableCar "CableCar.cpp" "CableCar.h" "SceneGraph.h" "SceneGraph.cpp" "Color.h")

target_link_libraries(CableCar ${CONAN_LIBS})
