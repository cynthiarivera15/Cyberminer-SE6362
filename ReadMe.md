/*------------------------------------------------------------------------------
Name: ReadMe.md
Description: This is the equivalent to the User Manual
Course: SE6362
------------------------------------------------------------------------------*/
## Introduction
This repository contains source code for the Cyberminer search engine which uses a Key Word in Context (KWIC) index software system.

- Save the `httplib.h` file to `include/httplib.h` directory. See [here](https://github.com/yhirose/cpp-httplib/blob/master/httplib.h)

### Setup
- Create build directory: `mkdir build`
- cd into build directory: `cd build`
- Compile: `cmake .. && make`
- Run server: `./server`
- Visit `http://localhost:8080`
