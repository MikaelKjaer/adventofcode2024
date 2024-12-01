set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_LINKER_TYPE MOLD)

list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:C,ASM,CXX>:-Wall>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:C,ASM,CXX>:-Wextra>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:C,ASM,CXX>:-Werror>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:C,ASM>:-pedantic>")

list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:CXX>:-Wextra-semi>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:CXX>:-Wpedantic>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:CXX>:-Wcast-qual>")
#list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:CXX>:-Wfloat-equal>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:CXX>:-Wunreachable-code>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:CXX>:-Wnon-virtual-dtor>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:CXX>:-Wshadow>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:CXX>:-Wold-style-cast>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:CXX>:-Wunused>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:CXX>:-Woverloaded-virtual>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:CXX>:-Wconversion>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:CXX>:-Wsign-conversion>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:CXX>:-Wnull-dereference>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:CXX>:-Wdouble-promotion>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:CXX>:-Wformat=2>")
list(APPEND WARNING_FLAGS_LIST "$<$<COMPILE_LANGUAGE:CXX>:-Wno-psabi>")
set(WARNING_FLAGS ${WARNING_FLAGS_LIST} CACHE INTERNAL "Warning flags for beef" FORCE)

set(CMAKE_CXX_FLAGS "-march=skylake -pipe -fvisibility=hidden -fvisibility-inlines-hidden -fno-strict-aliasing -stdlib=libstdc++" CACHE INTERNAL "C++ compiler options" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--sort-section=alignment -Wl,--as-needed" CACHE INTERNAL "Linker options" FORCE)

# Options for DEBUG build
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb -DDEBUG" CACHE INTERNAL "C++ Compiler options for debug build type" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-Wl,-O0" CACHE INTERNAL "Linker options for debug build type" FORCE)

# Options for RELEASE with debug information build
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -ggdb -flto -DNDEBUG" CACHE INTERNAL "C++ Compiler options for release with debug info build type" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "-Wl,-O1 -flto=auto -static" CACHE INTERNAL "Linker options for release with debug info build type" FORCE)

# Options for RELEASE build
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -flto -DNDEBUG" CACHE INTERNAL "C++ Compiler options for release build type" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-Wl,-O1 -flto=auto -static" CACHE INTERNAL "Linker options for release build type" FORCE)

