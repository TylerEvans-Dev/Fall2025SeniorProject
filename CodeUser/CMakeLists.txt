#Tyler Evans Used GPT to create cmake file
#set the standard version of cmake
cmake_minimum_required(VERSION 3.16)
#create project
project(LPSLAM C)
#set the standard
set(CMAKE_C_STANDARD 99)
#create a sub dir.
add_subdirectory(lib/raylib)


#sets the sources for the code
set(SOURCES
    src/main.c
)

#enables documentation to be in the code
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})


# Include directories for raylib and raygui
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/raylib/src
    ${CMAKE_SOURCE_DIR}/lib/raygui/src
)

# Link raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# On Linux, link system libs that raylib needs
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE m pthread dl GL X11)
endif()
