#Day 09 problem
set(DAY "day10")
configure_file("${DAY}.txt" "${DAY}.txt" COPYONLY)

add_executable(${DAY} "main.cpp")
target_include_directories(${DAY} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/.)
target_compile_options(${DAY} PRIVATE ${WARNING_FLAGS})
target_link_libraries(${DAY} adventofcode_utility_library fmt::fmt scn::scn)
