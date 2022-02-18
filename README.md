# Best Epic GD Mods
website here: https://geometrydash.eu/mods/

The source code is probably a nice example of how not to do stuff.

# Compiling
I stole the build instructions from [https://github.com/matcool/small-gd-mods](mat) and the rest of the build system too. And `mapped-hooks.hpp` and `utils.hpp` too.

1. Clone the repo (make sure to do it recursively `--recursive`)
2. Configure CMake
```
cmake -G "Visual Studio 16 2019" -B build -DCMAKE_BUILD_TYPE=Release -T host=x86 -A win32
```
3. Build
```bash
cmake --build build --config Release --target ALL_BUILD
# you can switch out ALL_BUILD for any specific mod you want to compile
```