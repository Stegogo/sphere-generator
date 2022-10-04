cmake_minimum_required(VERSION 3.10)

# set the project name
project(sk_test)
set(EXECUTABLE_OUTPUT_PATH "bin")
# add the executable
add_executable(spheregen main.cpp spheregenerator.cpp)
