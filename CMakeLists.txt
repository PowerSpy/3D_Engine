cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use pkg-config to find SDL2 and SDL2_ttf
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

# Include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

# Link directories (optional)
link_directories(
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_TTF_LIBRARY_DIRS}
)

# Add source files
set(SOURCES
    main.cpp
    Face.cpp
    Vec3.cpp
    Vec4.cpp
    OBJParser.cpp
    Camera.cpp
    Quaternion.cpp
)

# Create executable
add_executable(test ${SOURCES})

# Link libraries
target_link_libraries(test
    ${SDL2_TTF_LIBRARIES}
    # ${SDL2_LIBRARIES}
)
