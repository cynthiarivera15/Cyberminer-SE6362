cmake_minimum_required(VERSION 3.10)

# Project name
project(CppHttpServer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add executable
add_executable(server server.cpp Cyberminer.cpp Master_Control.cpp Input.cpp Line_Storage.cpp Circular_Shift.cpp Alphabetizer.cpp Output.cpp)

# Include httplib
include_directories(include)

# Link any necessary libraries (for httplib, none are required)
