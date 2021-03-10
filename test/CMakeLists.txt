# gtest
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
add_subdirectory("../googletest" "googletest")
if (MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
include(CTest)
enable_testing()

find_package(Boost REQUIRED COMPONENTS system program_options)
set(Boost_USE_STATIC_LIBS ON)
include_directories(${Boost_INCLUDE_DIR})


set(This bayan_test)
set(Source
    test_file_list.cpp
    test_wildcard.cpp
    test_file_dup.cpp
)

add_executable(${This} ${Source} )


target_include_directories(${This}  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../bayan"
)

target_link_libraries(${This} PRIVATE
gtest 
gtest_main 
bayan_lib
)

set_default_opts(${This})

file(COPY test_dirs DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_test(
    NAME ${This}
    COMMAND ${This}
)
c