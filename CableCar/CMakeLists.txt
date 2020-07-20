cmake_minimum_required (VERSION 3.8)

add_executable (CableCar "Main.cpp" "Color.cpp" "Color.h" "Texture.cpp" "Texture.h" "Renderer.cpp" "Renderer.h"
                         "FilesystemAssetManager.cpp" "FilesystemAssetManager.h" "AssetManager.h" "CachingAssetManager.h" "CachingAssetManager.cpp"
                         "Game.cpp" "Game.h" "Input.cpp" "Input.h" "Level.h" "Level.cpp" "Simulation.h" "Simulation.cpp" "GlobalConstants.h" "Box2DDebugDraw.cpp" "Box2DDebugDraw.h"
                         "external/SDL2_gfxPrimitives.h" "external/SDL2_gfxPrimitives.c" "external/SDL2_rotozoom.c" "external/SDL2_rotozoom.h" "external/SDL2_gfxPrimitives_font.h" )

set_property(TARGET CableCar PROPERTY CXX_STANDARD 17)

target_link_libraries(CableCar ${CONAN_LIBS})
target_include_directories(CableCar PRIVATE "external")

if(MSVC)
  target_compile_options(CableCar PRIVATE /W4)
endif()
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER MATCHES "clang")
  target_compile_options(CableCar PRIVATE -Wall -Wextra -pedantic)
endif()
