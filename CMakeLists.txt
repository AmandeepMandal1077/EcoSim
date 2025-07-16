# --- Basic Project Setup ---
# Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.14)

# Define the project name, version, and language
project(Ecosim VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard to C++17 (good for modern features)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# --- Google Test Integration ---
# Enable fetching content from the web
include(FetchContent)

# Declare the Google Test project from its GitHub repository
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG    release-1.14.0 # Use a specific, stable version
)

# Make the googletest source code available to our project
FetchContent_MakeAvailable(googletest)


# --- Main Application Target ---
# Add our main executable, named "Ecosim"
# It is built from the source file(s) listed
add_executable(Ecosim src/main.cpp)


# --- Testing Setup ---
# Enable the testing infrastructure for CMake
enable_testing()

# Add our test runner executable, named "RunTests"
add_executable(RunTests tests/test_initial.cpp)

# Link our test runner against the Google Test libraries
# gtest_main includes a main() function, so we don't need our own
target_link_libraries(RunTests PRIVATE gtest_main)

# Automatically discover all tests defined in our test runner
include(GoogleTest)
gtest_discover_tests(RunTests)