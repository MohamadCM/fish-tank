# Fish Tank Rendering Project

This repository contains the files for the Fish Tank project, relted the "Principles of Computer Graphics" course at STUBA.

## Installation instructions

The recommended way to build and explore the examples is to use the [CLion IDE](https://www.jetbrains.com/clion/) which is available for macOS, Linux and Windows. It is free for 30days and you can register for a [FREE LICENSE](https://www.jetbrains.com/student/) using university mail.

Depending on your system of choice you will need to install the following dependencies:
* C++14 capable compiler - This is part of the CLion IDE installation (bundled MinGW compiler)
* [CMake](https://cmake.org) build system - This is part of the CLion IDE installation
---
These are included in the repository - Windows only
* [GLEW](http://glew.sourceforge.net) - The OpenGL Extension Wrangler Library for compatibility reasons
* [GLFW3](http://www.glfw.org) - Platform-independent API for creating windows, reading input, handling events, etc.
* [GLM](http://glm.g-truc.net) - C++ mathematics library for graphics software
* [ASSIMP](https://assimp.org/) - Open Asset Import Library is a library to load various 3d file formats.

You can also use CMake directly to generate project files for many other IDEs, see [Generic CMake instructions](#generic-instructions-for-using-cmake).

### Linux dependencies
On Linux install the following dependencies using your package manager, for example on Ubuntu open your terminal and do:

```bash
sudo apt-get install build-essential cmake libglew-dev libglfw3-dev libglm-dev libassimp-dev
```

### Apple macOS dependencies
On macOS install [Homebrew](http://brew.sh) package manager and install the dependencies using the following commands in terminal. Note that the installation of Homebrew expects __XCode__ to be installed, this can be simply downloaded from the app store.

```bash
brew install cmake glm glfw glew libomp assimp
```

You can also install CMake and use it to generate project files for your IDE of choice.

### Microsoft Windows dependencies
On Microsoft Windows you may try to open the `CMakeLists.txt` file with the latest Visual Studio with CMake support. The samples should work as is, binaries for the above mentioned libraries are included as part of the repository.

However, the recommended way is to avoid Visual Studio and install [CLion IDE](https://www.jetbrains.com/clion/) and a __GCC__ based compiler:
If you wish to proceed with VS Code or Visual Studio follow these instructions: 
* VS-Code Download and install [MinGW-w64](https://sourceforge.net/projects/mingw-w64/), this is the latest GCC compiler for Windows in a nice installer.
    * Visual Studio has its own compiler
* Make sure to install the __64bit version (select x86_64, NOT i686)__, you need to manually switch this in the installer process. 
* To avoid issues with malformed paths also edit the installation destination to C:/mingw-w64 instead of Program Files.
* Run CLion and select MinGW as the __toolchain__ when prompted (default is C:/mingw-w64/..)
* Setup the rest of the settings as you see fit.
* Open this directory and point the IDE towards the `CMakeLists.txt` file.

If CLion is too resource intensive for your system you can also try [QTCreator IDE](http://www.qt.io/ide/) that should work out of the box with MinGW and CMake.

Another alternative is to use Visual Studio 2017 with CMake support included, with this version you can open the CMakeLists.txt file directly as any other project.
