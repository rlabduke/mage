# Mage

Dependencies on Linux:
* sudo apt install libmotif-dev

Dependencies on mac:
* You will need to install [CMake](https://cmake.org) to generate a working make.
* You will need to install [Homebrew](https://brew.sh) to be able to execute the following command and get Motif.
* brew install openmotif

Dependencies on Windows:
* MFC libraries are required for this project. Install them from the Visual Studio installer (Individual Components tab) for any toolsets and architectures being used.
* There is a missing header file and the need to compile with X windows, so this will not compile as of 1/7/2021.

# Building

Use CMake to compile this program.  It compiles and runs on Ubuntu 18.04 and MacOSX as of 1/6/2021.

Assuming that this code is checked out in ~/src/mage and all dependencies are available, the following will compile it
    mkdir -p ~/build/mage
    cd ~/build/mage
    cmake ~/src/mage
    make

# Running

On the mac, you will need the [XQuartz](https://xquartz.org) or another X Windows server running when mage is started.

Then, from the build directory you type `./mage` to run it.
