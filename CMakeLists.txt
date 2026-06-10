cmake_minimum_required(VERSION 3.15)
project(PollardKangarooPuzzle71 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler options
if(MSVC)
    add_compile_options(/W4 /permissive- /MP)
    # Treat warnings as errors
    add_compile_options(/WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# FetchContent for libsecp256k1
include(FetchContent)
FetchContent_Declare(
    secp256k1
    GIT_REPOSITORY https://github.com/bitcoin-core/secp256k1.git
    GIT_TAG      v0.4.0 # Use a specific tag for stability
)
FetchContent_MakeAvailable(secp256k1)

# Add GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

# Include directories
include_directories(include)

# Add the executable
add_executable(PollardKangarooPuzzle71 src/main.cpp)

# Link libsecp256k1
target_link_libraries(PollardKangarooPuzzle71 PRIVATE secp256k1::secp256k1)

# Add tests
add_subdirectory(tests)

# Install rules (optional, for later use)
install(TARGETS PollardKangarooPuzzle71 DESTINATION bin)
install(FILES LICENSE README.md DESTINATION .)
