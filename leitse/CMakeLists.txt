cmake_minimum_required(VERSION 3.12)

set(target "leitse")

file(GLOB_RECURSE src "*.cpp")
list(REMOVE_ITEM src "main.cpp")
list(FILTER src EXCLUDE REGEX ".test.cpp$")

add_library("${target}_base" "${src}")
add_executable("${target}" "main.cpp")
target_link_libraries("${target}" "${target}_base")

if (BUILD_TESTS)
    file(GLOB_RECURSE test_src "*.test.cpp")
    add_executable("${target}_test" "${test_src}")
    find_package("doctest" CONFIG REQUIRED)
    target_link_libraries("${target}_test" "${target}_base" "doctest::doctest")
    add_test(NAME "${target}_test" COMMAND "${target}_test")
endif ()


find_library(cpr_LIB NAMES "cpr")
find_package("CURL" REQUIRED)
find_package("ZLIB" REQUIRED)
find_package("unofficial-date" CONFIG REQUIRED)
find_package("nlohmann_json" CONFIG REQUIRED)

target_link_libraries("${target}_base"
    "config"
    "utils"

    "${cpr_LIB}"
    "CURL::libcurl"
    "ZLIB::ZLIB"
    "unofficial::date::date"
    "nlohmann_json::nlohmann_json"
)

if (WIN32)
    target_link_libraries("${target}_base" "Crypt32" "ws2_32")
else ()
    target_link_libraries("${target}_base" "stdc++fs")
endif ()
