cmake_minimum_required (VERSION 3.30)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project ("adventofcode2024" LANGUAGES CXX)

include(FetchContent)

if (MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif(MSVC)

FetchContent_Declare( fmt
GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
GIT_TAG 0c9fce2ffefecfdce794e1859584e25877b7b592
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare( scn
GIT_REPOSITORY "https://github.com/eliaskosunen/scnlib.git"
GIT_TAG e937be1a52588621b406d58ce8614f96bb5de747
)
set(SCN_TESTS OFF)
set(SCN_BENCHMARKS OFF)
set(SCN_DISABLE_FAST_FLOAT ON)
FetchContent_MakeAvailable(scn)

find_package(Threads REQUIRED)

file(GLOB TEST_INPUT_FILES LIST_DIRECTORIES false CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/input/*.txt)
file(COPY ${TEST_INPUT_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/input)

add_subdirectory("utility")
add_subdirectory("day01")
add_subdirectory("day02")
add_subdirectory("day03")
add_subdirectory("day04")
add_subdirectory("day05")
