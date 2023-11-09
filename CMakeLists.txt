cmake_minimum_required(VERSION 3.2) # Specify CMake version.
project(Homework2) # A project name. This can be anything.
set(CMAKE_CXX_STANDARD 20) # Specify C++ version.

# Dependencies
find_package(OpenGL REQUIRED)

include(FetchContent)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.28.5
)
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(SDL2)

# Our Project
set(SOURCE_FILES
     hw2_submission.cpp HW2_Visualizer.h HW2_Visualizer.cpp)
find_package(SDL2 REQUIRED)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2::SDL2main)

# Copy the dynamic library to the same folder as the executable file.
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "./_deps/sdl2-build/libSDL2-2.0.0.dylib" "${CMAKE_SOURCE_DIR}/map.bmp" $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)