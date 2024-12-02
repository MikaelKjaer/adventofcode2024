#Day 01 problem
configure_file("day02.txt" "day02.txt" COPYONLY)

add_executable(day02 "main.cpp")
target_include_directories(day02 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/.)
target_compile_options(day02 PRIVATE ${WARNING_FLAGS})
target_link_libraries(day02 adventofcode_utility_library fmt::fmt scn::scn)
