cmake_minimum_required(VERSION 3.12)

set(target "config")

file(GLOB_RECURSE src "*.cpp")
list(FILTER src EXCLUDE REGEX ".test.cpp$")

add_library("${target}" "${src}")

if (BUILD_TESTS)
    file(GLOB_RECURSE test_src "*.test.cpp")
    add_executable("${target}_test" "${test_src}")
    target_link_libraries("${target}_test" "${target}")
    add_test(NAME "${target}_test" COMMAND "${target}_test")
endif ()


find_package("doctest" CONFIG REQUIRED)
find_package("fmt" CONFIG REQUIRED)

target_link_libraries("${target}"
    "doctest::doctest"
    "fmt::fmt"
)

if (UNIX)
    find_package("Threads" REQUIRED)
    target_link_libraries("${target}" "Threads::Threads" "stdc++fs")
endif ()
