#set the min version
# for my ref  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build  is used for setting up documentation
cmake_minimum_required(VERSION 3.10)
#set the standard 99 for raylib
set(CMAKE_STANDARD_C 99)
#added for cmake autocompletion (getting rid of some squiggles around the lib)
set(DCMAKE_EXPORT_COMPILE_COMANDS ON)
#creates the project and what langauge
project(RaylibPractice C)
#get the
#this finds raylib
find_package(raylib REQUIRED)
#adds an exe in build folder
add_executable(RayLibPractice main.c)
#links the lib
target_link_libraries(RayLibPractice raylib)
target_include_directories(RayLibPractice PRIVATE /opt/homebrew/include)

if(UNIX AND NOT APPLE)

endif()
