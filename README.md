# Unix
```
mkdir build
cd build
conan install .. --build=missing
cmake -G Ninja ..
ninja
```

# Visual Studio 2019
```
mkdir build
cd build
conan install .. -s compiler="Visual Studio" -s compiler.runtime=MDd -s compiler.version=16 --build=missing -s arch=x86_64
cmake -G "Visual Studio 16 2019" -A x64 ..
```
Open solution file in build/ directory.
