# --- Basic Project Setup ---
cmake_minimum_required(VERSION 3.14)
project(EcoSim VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- SFML Integration ---
# Make sure to set SFML_DIR in your environment or CMake configuration
# if SFML is not in a standard location.
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# --- Google Test Integration ---
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

# --- Create a Static Library for Core Logic ---
add_library(EcoSimLib STATIC
    src/World.cpp
    src/Entity.cpp
)
target_include_directories(EcoSimLib PUBLIC include)
# The library itself needs to know about SFML includes
target_link_libraries(EcoSimLib PUBLIC sfml-graphics)


# --- Main Application Target ---
add_executable(EcoSim src/main.cpp)
# The executable links to our library, which now includes SFML
target_link_libraries(EcoSim PRIVATE EcoSimLib)

# --- Testing Setup ---
enable_testing()
add_executable(RunTests
    tests/test_world.cpp
    tests/test_entity.cpp
    tests/test_animalconfig.cpp
)
# Link the test runner against your simulation library, Google Test, AND SFML
target_link_libraries(RunTests PRIVATE EcoSimLib gtest_main sfml-graphics)

# --- Discover Tests ---
include(GoogleTest)
gtest_discover_tests(RunTests)
