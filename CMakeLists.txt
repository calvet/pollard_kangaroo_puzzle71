cmake_minimum_required(VERSION 3.15)

project(PollardKangarooPuzzle71 LANGUAGES C CXX)

# ============================================================================
# C++ Configuration
# ============================================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# Compiler Options
# ============================================================================

if(MSVC)
    add_compile_options(
        /W4
        /permissive-
        /MP
        /WX
    )
else()
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
endif()

# ============================================================================
# Dependencies
# ============================================================================

include(FetchContent)

# ----------------------------------------------------------------------------
# libsecp256k1
# ----------------------------------------------------------------------------

FetchContent_Declare(
    secp256k1
    GIT_REPOSITORY https://github.com/bitcoin-core/secp256k1.git
    GIT_TAG v0.4.0
)

# Configure secp256k1 before downloading/building
set(SECP256K1_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SECP256K1_BUILD_BENCHMARK OFF CACHE BOOL "" FORCE)
set(SECP256K1_BUILD_EXHAUSTIVE_TESTS OFF CACHE BOOL "" FORCE)
set(SECP256K1_ENABLE_MODULE_ECDH ON CACHE BOOL "" FORCE)
set(SECP256K1_ENABLE_MODULE_EXTRAKEYS ON CACHE BOOL "" FORCE)
set(SECP256K1_ENABLE_MODULE_SCHNORRSIG ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(secp256k1)

# ----------------------------------------------------------------------------
# GoogleTest
# ----------------------------------------------------------------------------

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# ============================================================================
# Includes
# ============================================================================

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# ============================================================================
# Main Executable
# ============================================================================

add_executable(
    PollardKangarooPuzzle71
    src/main.cpp
)

# Link secp256k1
#
# IMPORTANTE:
# Na versão v0.4.0 o target correto é "secp256k1"
# e NÃO "secp256k1::secp256k1".
#
target_link_libraries(
    PollardKangarooPuzzle71
    PRIVATE
    secp256k1
)

# ============================================================================
# Tests
# ============================================================================

include(CTest)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# ============================================================================
# Installation
# ============================================================================

install(
    TARGETS PollardKangarooPuzzle71
    RUNTIME DESTINATION bin
)

install(
    FILES
        LICENSE
        README.md
    DESTINATION .
)