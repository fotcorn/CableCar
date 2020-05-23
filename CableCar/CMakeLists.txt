cmake_minimum_required (VERSION 3.8)

add_executable (CableCar "Main.cpp" "Color.h" "Texture.cpp" "Texture.h" "Renderer.cpp" "Renderer.h" "FilesystemAssetManager.cpp" "FilesystemAssetManager.h" "AssetManager.h" "Services.h" "Services.cpp"
                         "Level.cpp" "Level.h" "GameLoop.cpp" "GameLoop.h")

set_property(TARGET CableCar PROPERTY CXX_STANDARD 17)

target_link_libraries(CableCar ${CONAN_LIBS})
target_include_directories(CableCar PRIVATE "external")
