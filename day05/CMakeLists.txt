#Day 05 problem
set(DAY "day05")
configure_file("${DAY}_1.txt" "${DAY}_1.txt" COPYONLY)
configure_file("${DAY}_2.txt" "${DAY}_2.txt" COPYONLY)

add_executable(${DAY} "main.cpp")
target_include_directories(${DAY} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/.)
target_compile_options(${DAY} PRIVATE ${WARNING_FLAGS})
target_link_libraries(${DAY} adventofcode_utility_library fmt::fmt scn::scn)
