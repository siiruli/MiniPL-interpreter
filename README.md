
# MiniPL interpreter

Course project for the University of Helsinki course Compilers.

This project is an interpreter for a small toy language
called MiniPL. 
The interpreter was implemented in C++.
It performs all the main phases of interpretation: 
scanning, parsing, semantic analysis (including type checking), 
and execution. 
Documentation can be found in the file
[`doc/report.pdf`](doc/report.pdf).


## Dependencies

The project is built with CMake and tested 
with GoogleTest. 

CMake can be installed on Ubuntu with:

```
sudo apt install cmake
```

GoogleTest should be included in 
the folder `libraries` to enable testing. 
It has been added as a submodule to the project.

To clone GoogleTest along with this project, use
```
git clone --recursive https://github.com/siiruli/MiniPL-interpreter
```
or if you already cloned the repository, use
```
git submodule update --init
```
to add the submodule.


## Usage

Build and run the project as follows:
```
# create build directory
mkdir build
cd build
cmake ..

# build and run source target
make MiniPL_interpreter_run
bin/MiniPL_interpreter_run <filename>

# build and run test target
make MiniPL_interpreter_tst
bin/MiniPL_interpreter_tst
```
