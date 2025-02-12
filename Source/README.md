## Generating project files for development
**Visual Studio (tested with version 2019)**  
Visual Studio has support for Cmake projects, and will automatically generate a Visual Studio solution:  
1. Open the `Source` folder in Visual Studio. This should automatically generate directories `.vs` and `out`.
2. In Visual Studio, go to `Solution Explorer`, right-click `main.cpp` in the file browser and click `Set as startup item`.
3. Run `main.cpp`. If all goes well, you should see the output of the program.  

**Clion**  
Stub. TODO

**Linux**  
1. Ensure Cmake is installed: `sudo apt install cmake`
2. In the `Source` folder, create a new build directory using `mkdir build`.
3. Navigate into the build directory using `cd build`, and deploy the Cmake project inside this directory using `cmake ../`. This should generate several Cmake-related files in the `build` directory.
4. Run `cmake --build .` in the `build` directory to build the executable. If all goes well, this should output an executable build of the program.