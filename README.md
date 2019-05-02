# Computer Graphics 2 - exercise repository

This repository contains the code which you are going to work with over the course of the CG2 exercises. It contains the CGV framework as a submodule, so be sure to clone the repository using the `--recursive` flag, like so:
>`git clone https://bitbucket.org/cgvtud/cg2.git --recursive`

If you forget adding the flag when cloning, you can run
>`git submodule init`

>`git submodule update --recursive --remote`

from within your local repository root later on.

## CMake and debugging

In order to actually build the exercises, you have to configure and generate a local build environment using [CMake](https://cmake.org/) or a CMake-aware IDE. There are three officially supported ways of setting up your build and debug environment, depending on what platform you are on. Unfortunately, there is currently no support for building and running on Mac OS.

### Windows / Visual Studio
On Windows, generate a multi-configuration Visual Studio solution in a directory outside of your local clone of the repository. This is most easily accomplished using the CMake GUI included in official CMake Windows distributions.
The exercises will appear as individual projects under "Application Plugins" in the solution explorer. You can set any of them as Startup project and simply hit F5 to start the exercise with a debugger attached. The necessary command line for loading all Framework plugins, setting the shader path and loading config files will be pre-set by CMake.

### Linux / Visual Studio Code
Visual Studio Code is a multi-platform, free and open source IDE with CMake support. We recommend using this IDE under Linux as the exercise CMake system can auto-generate debugging configurations for this IDE, similar to Visual Studio on Windows. For this, do not explicitly generate a build system using CMake directly - just direct VS Code to the root folder of your local repository clone. After letting VS Code configure the built system once, debug targets for every exercise will be available in the Debug tab. If you selected "Debug" as the configuration type, you can then just select any exercise from the launch configuration drop-down menu and hit F5 to start debugging.

### Unsupported configurations - do not ask your supervisors for help!
If you cannot use VS Code, you have a number of options available. However, these are not officially sanctioned ways to work on the exercises. Do so at your own risk.

##### Command line / terminal
When configuring the exercises, two shell scripts are being generated per exercise that let you run a release build or debug a debug build - provided your platform uses the GNU compiler suite and the GNU debugger. For this, use CMake to create a built system outside of your repository. In your build directory you will then find the shell scripts `run1.sh`,`debug1.sh`,`run2.sh`,`debug2.sh` and so forth for running or debugging exercise 1, 2 and so forth. The `debug*.sh` scripts will only work if you pass `-DCMAKE_BUILD_TYPE=Debug` to CMake, while the `run*.sh` scripts will only work of you omit this option.

##### Other IDEs
In theory, any CMake-aware IDE can be used to directly open the root folder of the exercises repository. While this will almost certainly work for simply compiling the exercises, debugging inside any IDE other than Visual Studio or VS Code will require you to create your own Launch/Debug configurations, using whatever mechanism your IDE provides for this. Look at the `run*.sh`/`debug*.sh` scripts in your build folder or the `.vscode/launch.json` in your local repository clone to learn about the correct command line for each exercise.
