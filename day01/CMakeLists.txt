#Day 01 problem
configure_file("day01.txt" "day01.txt" COPYONLY)

add_executable(day01 "main.cpp")
target_include_directories(day01 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/.)
target_compile_options(day01 PRIVATE ${WARNING_FLAGS})
target_link_libraries(day01 adventofcode_utility_library fmt::fmt scn::scn)
