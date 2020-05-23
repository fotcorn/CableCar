from conans import ConanFile, CMake

class VoxelWorld(ConanFile):
  settings = 'os', 'compiler', 'build_type', 'arch'
  requires = [
    'sdl2/2.0.9@bincrafters/stable',
    'sdl2_image/2.0.5@bincrafters/stable',
    'glm/0.9.9.1@g-truc/stable',
    'entt/3.4.0@_/_',
  ]
  generators = 'cmake',
