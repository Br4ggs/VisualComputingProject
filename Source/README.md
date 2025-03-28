## Generating project files for development
**Visual Studio (tested with version 2019)**  
Visual Studio has support for Cmake projects, and will automatically generate a Visual Studio solution:  
1. Open the `Source` folder in Visual Studio. This should automatically generate directories `.vs` and `out`.
2. In Visual Studio, go to `Solution Explorer`, right-click `main.cpp` in the file browser and click `Set as startup item`.
3. Run `main.cpp`. If all goes well, you should see the output of the program.  

**Clion**  
1. Make sure that you have all the prerequisite programs:
- Make or Ninja should be present and available through the command line (on your path somewhere).
- CMake should be installed.
- You should have a compiler installed for c++, such as g++ or clang
2. Open the project with CLion.
3. Right click on the Source/CMakeLists.txt file and click `Load CMake Project`
4. Usually, a directory is created that is called something like `cmake-build-debug` or `cmake-build-release`. Make sure to ignore this directory in version control as it is generated based on your machine's settings and other people have no use for this.

**Linux [untested]**  
1. Ensure Cmake is installed: `sudo apt install cmake`
2. In the `Source` folder, create a new build directory using `mkdir build`.
3. Navigate into the build directory using `cd build`, and deploy the Cmake project inside this directory using `cmake ../`. This should generate several Cmake-related files in the `build` directory.
4. Run `cmake --build .` in the `build` directory to build the executable. If all goes well, this should output an executable build of the program.

## Dependencies used
[glad](https://glad.dav1d.de/)  
A loader for OpenGL (used for API bindings).

[glfw3](https://www.glfw.org/)  
OpenGL library that provides a simplified API for creating windows, contexts, surfaces and handling IO.

[glm](https://github.com/g-truc/glm)  
OpenGL library that provides mathematical operations and structures such as matrices.

[ImGui](https://github.com/ocornut/imgui)  
OpenGL library that provides a bloat-free API for creating graphical user interfaces.
