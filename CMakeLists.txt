cmake_minimum_required(VERSION 3.28)

project(Structured-Computer-Organization VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

### Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
    GoogleTestSource
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(GoogleTestSource)

### Tests
file(
    GLOB_RECURSE
    TESTS_SOURCES
    src/**.cpp
)
add_executable(Tests ${TESTS_SOURCES})
target_link_libraries(Tests GTest::gtest_main)
# Includes GoogleTest utilities for CMake
include(GoogleTest)
gtest_discover_tests(Tests)