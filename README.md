# **Chaos: Morden C++ Library**
***

## **What is `Chaos`?**
  **Chaos is a basic library of C++11 components.**

  * A collection of C++ class libraries.
  * Written in efficient, mordern, 100% `ANSI/ISO Standand C++`.
  * Based on and complementing the `C++ Standand Library/STL`.
  * Highly portable and available on many different platforms.
  * Open Source, licensed under [The FreeBSD License](https://www.freebsd.org/copyright/freebsd-doc-license.html).

## **Building**
  Chaos use following software as its dependencies:

  * [CMake](https://cmake.org/) as a general build tool.

  To build and run unittests please proceed with the steps below:

  * Create directory called `build` or `cmake-build` in Chaos source directory.
  * Change to `build` or `cmake-build` directory and run `cmake ..` command to configure your build.
  * On Windows, build the solution found in the build directory. On Linux or macOS run `make` from the build directory.
```shell
  # On Linux or macOS
  $ mkdir cmake-build && cd cmake-build
  $ cmake ..
  $ make
  # Run unittests on Linux or macOS
  $ ./bin/chaos.test

  # On Windows
  # Setting the building environment of VS2015 or VS2017, such as:
  \> %VS2015PATH%\VC\vavarsall.bat # just for x86
  \> %VS2015PATH%\VC\vavarsall.bat amd64 # just for x64
  # Building Chaos using command
  \> mkdir cmake-build & cd cmake-build
  \> cmake -G "NMake Makefiles" ..
  \> nmake
  # Run unittests on Windows
  \> .\bin\chaos.test.exe

  # Building Chaos using VS2015 solution
  \> mkdir cmake-build & cd cmake-build
  \> cmake .. # for x86
  \> cmake -A x64 .. # for x64
  \> msbuild ChaosPROJ.sln
```
