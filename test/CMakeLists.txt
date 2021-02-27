cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

# gtest
set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
add_subdirectory("../googletest" "googletest")
if (MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
include(CTest)
enable_testing()


set (This async_test)
set (Sources
    test_generator.cpp
    ../async_demo/generator.cpp
)

add_executable(${This} ${Sources} )



target_include_directories(${This}  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../async_demo"
)

target_link_libraries(${This} PRIVATE
gtest gtest_main)

set_default_opts(${This})

add_test(
    NAME ${This}
    COMMAND ${This}
)

## valgrind
find_program(CTEST_MEMORYCHECK_COMMAND NAMES valgrind)
INCLUDE(Dart)

