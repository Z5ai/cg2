# Computer Graphics 2 - exercise repository

This repository contains the code which you are going to work with over the course of the CG2 exercises. It contains the CGV Framework as a submodule, so be sure to clone the repository using the `--recursive` flag, like so:
>`git clone https://bitbucket.org/cgvtud/cg2.git --recursive`

If you forget adding the flag when cloning, you can run
>`git submodule init`

>`git submodule update --remote`

from within your local repository root later on.

## Development environment

Currently, the CGV Framework only supports building on Windows and Visual Studio. A CMake-based build system is included in the Framework repository, but it does not yet cover many of the newer modules and has to be considered pre-alpha at this point. Thus, no CMake build files are included in the exercise.

## Building the exercises

Refer to the file HowToBuild.pdf in the repository root for illustrated instructions on how to setup the build environment and build the exercise code.