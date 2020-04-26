from conans import ConanFile, CMake

class VoxelWorld(ConanFile):
  settings = 'os', 'compiler', 'build_type', 'arch'
  requires = [
    'sdl2/2.0.12@bincrafters/stable',
  ]
  generators = 'cmake',
