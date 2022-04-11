# Computer Graphics 2 - exercise repository

This repository contains the code which you are going to work with over the course of the CG2 exercises. It contains the CGV Framework as a submodule, so be sure to clone the repository using the `--recursive` flag, like so:
>`git clone https://bitbucket.org/cgvtud/cg2.git --recursive`

If you forget adding the flag when cloning, you can run
>`git submodule init`

>`git submodule update --remote`

from within your local repository root later on.

## Development environment

The CGV Framework is cross-platform and supports Windows and Linux (MacOS is supported in theory, but completely untested and likely to not work without adjustments). For the exercise however, only building on Windows and Visual Studio is officially supported. CMake build files are included in the exercise and you are free to develop on the OS of your choice, but due to the vast multitude of different configurations and setups, we _cannot_ and _*will not*_ provide assistance if you encounter trouble building or running on anything other than Windows.

## Building the exercises

Refer to the file HowToBuild.pdf in the repository root for illustrated instructions on how to set up the official Windows build environment and build the exercise code.

## Linux pointers

To solve the exercise in a productive way under Linux, we do recommend using an IDE. Many current offerings (like the open source Visual Studio Code editor) support opening a CMake-enabled source tree directly. However, you will have to manually create the launch and debug configurations for your IDE of choice. The CMake files of the exercise will create shell scripts in the CMake build directory for launching each of the exercises, named `CG2_exercise1.sh`, `CG2_exercise2.sh` and `CG2_exercise45.sh`, respectively. You can inspect them to find out how to launch the `cgv_viewer` binary and provide it with the necessary command line arguments for each exercise, which you can then adapt for your IDE.