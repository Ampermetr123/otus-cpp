
set (This join_server_test)
set (Sources
    test_result.cpp
    test_commands.cpp
    test_storage.cpp
)

add_executable(${This} ${Sources} )

target_include_directories(${This}  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(${This} PRIVATE
gtest gtest_main gmock join_server_lib)

set_default_opts(${This})




# include(GoogleTest)
# gtest_discover_tests(${This})



add_test(
    NAME ${This}
    COMMAND ${This}
)

## valgrind
find_program(CTEST_MEMORYCHECK_COMMAND NAMES valgrind)
INCLUDE(Dart)

