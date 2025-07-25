# --- Basic Project Setup ---
cmake_minimum_required(VERSION 3.14)
project(EcoSim VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Google Test Integration ---
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  # Best practice: Pin to a specific, stable release tag instead of 'main'
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

# --- Create a Static Library for Core Logic ---
# This library will contain all your simulation code except main.cpp
add_library(EcoSimLib STATIC
    src/World.cpp
    src/Entity.cpp
    # Add new .cpp files here as you create them
)
# Make sure the library can find its own header files in the include directory
target_include_directories(EcoSimLib PUBLIC include)

# --- Main Application Target ---
# The executable is now very simple: just main.cpp
add_executable(EcoSim src/main.cpp)
# Link the executable against your simulation library
target_link_libraries(EcoSim PRIVATE EcoSimLib)

# --- Testing Setup ---
enable_testing()
# The test runner includes only the test source files
add_executable(RunTests
    tests/test_world.cpp
    tests/test_entity.cpp
    tests/test_animalconfig.cpp
    # Add new test files here
)
# Link the test runner against your simulation library AND Google Test
target_link_libraries(RunTests PRIVATE EcoSimLib gtest_main)

# --- Discover Tests ---
include(GoogleTest)
gtest_discover_tests(RunTests)